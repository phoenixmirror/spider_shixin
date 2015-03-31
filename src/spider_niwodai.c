#include <common.h>

#define HERF_URL_LIST "href.txt"
int  curl_flag = 0;

size_t CurlResponseProc( char *ptr, size_t size, size_t nmemb, void *userdata )
{
	strcat( userdata, ptr ) ;

	return size*nmemb ;
}

int GetHtmlByUrl( char *url, char *html )
{
	CURL *pCurl ;	
	char errmsg[512] ;
	CURLcode rc = CURLE_OK ;
	
	memset( errmsg, 0x00, sizeof( errmsg ) ) ;

	if( curl_flag == 0 )	
	{
		rc = curl_global_init( CURL_GLOBAL_ALL );
		if( rc != 0 )
		{
			fprintf( stdout, "curl_global_init error !\n" ) ;
			return -1 ;
		}

		curl_flag = 1 ;
	}

	pCurl = curl_easy_init();
	if( pCurl == NULL )
	{
		fprintf( stdout, "curl_easy_init error !\n" ) ;
		return -2 ;
	}

	curl_easy_setopt( pCurl, CURLOPT_URL, url ) ;
	curl_easy_setopt( pCurl, CURLOPT_WRITEFUNCTION, CurlResponseProc ) ;
	curl_easy_setopt( pCurl, CURLOPT_WRITEDATA, html ) ;
	curl_easy_setopt( pCurl, CURLOPT_ERRORBUFFER, errmsg );
	curl_easy_setopt( pCurl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:34.0) Gecko/20100101 Firefox/34.0");
	
	
	rc = curl_easy_perform( pCurl ) ;
	if( rc != CURLE_OK )
	{
		fprintf( stdout, "curl_easy_perform error ![%s]\n", errmsg ) ;
		return -3 ;
	}

	curl_easy_cleanup( pCurl );

	return 0 ;
}


int WriteHtmlToFile( char *filepath, char *html )
{
	FILE *fp ;

	if( filepath == NULL )
	{
		fprintf( stdout, "filepath is NULL\n" ) ;
		return -1 ;
	}

	fp = fopen( filepath, "w" ) ;	
	if( fp == NULL )
	{
		fprintf( stdout, "fopen error[%s]\n", filepath ) ;
		return -1 ;
	}
	
	fprintf( fp, "%s", html ) ;

	fclose( fp ) ;

	return 0 ;
}



int spider( int argc, char *argv[] )
{
	int nret = 0 ;
	int i = 0 ;
	int page = 0 ;
	int totalcount ;
	char url[256] ;
	char mainurl[100] ;
	char htmldata[1024*1024] ;
	char urllist_file[256];
	char talcnt[10];
	char filepath[512] ;
	char *ptrb = NULL ;
	char *ptre = NULL ;
	FILE *fp = NULL ;
	int j = 0 ;
	int lines = 0;
	int len = 0 ;
	char buff[256] ;
	char htmlname[256];
	char *p = NULL ;
	char content[200];
	DList	todoList;
	DList	doneList;
	DList_stepInfo  sTodoinfo;
	DList_stepInfo  sDoneinfo;

        DList_new(&todoList);
        DList_new(&doneList);


	/* preprocess */









	memset( mainurl, 0x00, sizeof( mainurl ) ) ;
	strcpy( mainurl, "https://member.niwodai.com" ) ;

	memset( url, 0x00, sizeof( url ) ) ;
	sprintf( url, "%s/%s", mainurl, "xiangmu" ) ;

	memset( urllist_file, 0x00, sizeof( urllist_file ) ) ;
	sprintf( urllist_file, "%s/tmp/item/%s", getenv("HOME"), HERF_URL_LIST ) ;

	if( !access( urllist_file, F_OK ) ) 
	{
		fprintf( stdout, "file exsit, del it\n" )  ;
		unlink( urllist_file ) ;
	}

	/* 取的初始页面 */
	fprintf(stdout, "url=%s\n",url );
	memset( htmldata, 0x00, sizeof( htmldata ) ) ;
	nret = GetHtmlByUrl( url, htmldata ) ;		
	if( nret < 0 )
	{
		fprintf( stdout, "GetHtmlByUrl error" ) ;
		return -1 ;
	}	

	/* 解析初始页面, 取得列表总条数 */
	ptrb = strstr( htmldata, "totalCount=" ) ;
	if( ptrb == NULL )
	{
		fprintf( stdout, "not found totalCount tag\n" ) ;
		return -2 ;
	}
	ptre = strstr( ptrb, ">" ) ;
	if( ptre == NULL )
	{
		fprintf( stdout, "not found '>' tag\n" ) ;
		return -3 ;
	}
	memset( talcnt, 0x00, sizeof( talcnt ) ) ;
	memcpy( talcnt, ptrb + 11, ptre - ptrb - 11 -1 ) ;  
	totalcount = atoi( talcnt ) ;
	
	/* 根据总条数，计算页码数 */
	if( totalcount % 10 == 0 )
		page = totalcount / 10 ; 
	else	
		page = totalcount / 10 + 1 ;

	/* 写初始页面文件 */
	memset( filepath, 0x00, sizeof( filepath ) ) ;
	sprintf( filepath, "%s/tmp/item/niwodai_%d_1.html", getenv("HOME"), page ) ;
	nret = WriteHtmlToFile( filepath, htmldata ) ;
	if( nret < 0 )
	{
		fprintf( stdout, "WriteHtmlToFile error [%d]\n", nret ) ;
		return -4 ;
	}

/*
	for( i=2; i<= page; i++ ) 
	{
		memset( url, 0x00, sizeof( url ) ) ;
		sprintf( url, "%s/loan/loan.do?pageNo=%d&totalCount=%d", mainurl, i, totalcount ) ;
		fprintf( stdout, "%s\n", url ) ;
		memset( htmldata, 0x00, sizeof( htmldata ) ) ;
		nret = GetHtmlByUrl( url, htmldata ) ;		
		if( nret < 0 )
		{
			fprintf( stdout, "GetHtmlByUrl error" ) ;
			return -1 ;
		}	

		memset( filepath, 0x00, sizeof( filepath ) ) ;
		sprintf( filepath, "%s/tmp/item/niwodai_%d_%d.html", getenv("HOME"), page, i ) ;
		nret = WriteHtmlToFile( filepath, htmldata ) ;
		if( nret < 0 )
		{
			fprintf( stdout, "WriteHtmlToFile error [%d]\n", nret ) ;
			return -4 ;
		}
	}
*/







	while( 1 ) 
	{
		if( access( urllist_file, F_OK ) != 0 )
		{
			sleep( 2 ) ;
			fprintf( stdout, "未发现[%s]文件\n" ,urllist_file ) ;
			continue ;
		}
		else
		{
			fp = fopen( urllist_file, "r" ) ;	
			if( fp == NULL )
			{
				fprintf( stdout, "fopen error[%s]\n", urllist_file ) ;
				return -1 ;
			}

			memset( buff, 0x00, sizeof(buff) );
			lines = 0;
			/* 读取文件列表 放入todoList */
			while( fgets( buff, 256, fp ) ) 
			{
				/* 去掉后面回车符 */
				len = strlen( buff ) ;
				buff[ len -1 ] = '\0' ;

				memset( url, 0x00, sizeof( url ) ) ;
				sprintf( url, "%s%s", mainurl, buff ) ;

				DList_add(&todoList,(char *)url,NULL);

				lines ++ ;
				memset( buff, 0x00, sizeof(buff) );
			}
			fclose( fp ) ;
			fp = NULL;
			fprintf( stdout, "文件读取[%d]行 \n" , lines ) ;
			fprintf( stdout, "待处理 cnt=[%d]\n" , todoList.cnt ) ;

			/* 获取未解析文件 */
			DList_openStep( &todoList, &sTodoinfo );
			while ( DList_fetchStep( &todoList,&sTodoinfo,(char *)url ) == 0 )
			{
				fprintf( stdout, "begin --------url[%s]\n" , url ) ;
				memset( htmldata, 0x00, sizeof( htmldata ) ) ;
				nret = GetHtmlByUrl( url, htmldata ) ;
				if( nret < 0 )
				{
					fprintf( stdout, "GetHtmlByUrl error [%s]", url ) ;
					continue;
				}	
				fprintf( stdout, "end --------url[%s]\n" , url ) ;

				/* 写文件 */
				memset( filepath, 0x00, sizeof( filepath ) ) ;
				memset( htmlname, 0x00, sizeof( htmlname ) ) ;

				memset( content, 0x00, sizeof(content) );
				strcpy( content, url );
				p = strtok( content, "/" ) ;
				while( p )
				{
					strcpy( htmlname, p ) ;
					p = strtok( NULL , "/" ) ;
				}

				sprintf( filepath, "%s/tmp/cstm/%s", getenv("HOME"), htmlname ) ;
				fprintf( stdout,"----------filepath[%s]\n", filepath ) ;
				nret = WriteHtmlToFile( filepath, htmldata ) ;
				if( nret < 0 )
				{
					fprintf( stdout, "WriteHtmlToFile error [%d]\n", nret ) ;
					/* return -4 ; */
				}


				/* 把获取后的链接放入已完成 */
				DList_del( &todoList, (char *)url, NULL );
				DList_add(&doneList,(char *)url,NULL);

				memset( url, 0x00, sizeof(url));

			}
			DList_closeStep( &todoList,&sTodoinfo );

			sleep( 2 ) ;
			fprintf( stdout, "休眠2秒\n" ) ;
		}

		DList_openStep( &todoList, &sTodoinfo );
		while ( DList_fetchStep( &todoList,&sTodoinfo,(char *)url ) == 0 )
		{
			fprintf( stdout, "未完成收集 [%d][%s]\n", j, url ) ;
			memset( url, 0x00, sizeof(url) );
		}
		DList_closeStep( &todoList,&sTodoinfo );

		/*DList_openStep( &doneList, &sDoneinfo );
		while ( DList_fetchStep( &doneList,&sDoneinfo,(char *)url ) == 0 )
		{
			fprintf( stdout, "完成收集 [%d][%s]\n", j, url ) ;
			memset( url, 0x00, sizeof(url) );
		}
		DList_closeStep( &doneList,&sDoneinfo );*/

		fprintf( stdout, "共收集 [%d]\n", doneList.cnt ) ;
		if( todoList.cnt == 0 ) 
			break ;
	}

	curl_global_cleanup() ;

	fprintf( stdout, "网页下载完成\n" ) ;

	return 0 ;
}









int spider1()
{
	int	nret = 0;

/* 1 get url list 
1.1 analyse url list
1.2 wirte list
*/
	get_htmllist();		












/* 2 write html data  common process */
	get_htmldata_process();














	return	nret;
}






int get_htmllist()
{
	int	nret = 0;	





	return	nret;
}







int get_htmldata_process()
{

	int	nret = 0;	


	return nret;
}



