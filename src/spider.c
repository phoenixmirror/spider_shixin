#include <common.h>

stDefSharedMemory stmem;
char *donelist[URL_NUM]={0};
extern int giQidOut;
iconv_t iconv_toinchar;
CURL *pCurl;
int curl_flag = 0;
int totalcnt = 0;
int travel_depth = 0;
int icontentfilter = 0;
int iurlfilter = 0;
int icontentinvolve = 0;
int iurlinvolve = 0;
int icontrol = 1;
int ifilecontentinvolve = 0;
int ifileurlinvolve = 0;
int idownload = 0;
int iflag = 0;
int ifileflag = 0;
int icontinue = 0;
long llt = 0;
long lgt = 0;
long lt_flag = 0;
long gt_flag = 0;
long and_flag = 0;
DList t_todoList[THREAD_CNT];
DList t_doneList[THREAD_CNT];
DList_stepInfo  t_sTodoinfo[THREAD_CNT];
char seedurl[URL_LEN];
char *contentfilter[MAX_CNT] = {0};
char *urlfilter[MAX_CNT] = {0};
char *contentinvolve[MAX_CNT] = {0};
char *urlinvolve[MAX_CNT] = {0};
char *filecontentinvolve[MAX_CNT] = {0};
char *fileurlinvolve[MAX_CNT] = {0};
char basepath[URL_LEN];
pthread_t threadId[THREAD_CNT];
t_env env[THREAD_CNT];

int  wr_index=0;

size_t CurlResponseProc( char *ptr, size_t size, size_t nmemb, void *userdata )
{
	char *p = (char *)userdata;
	int segsize = nmemb*size;
	/*strcat( userdata, ptr ) ;*/

        /* Copy the data from the curl buffer into our buffer */
        memcpy( p+wr_index, ptr, (size_t)segsize );

        /* Update the write index */
        wr_index += segsize;

        /* Null terminate the buffer */
        p[wr_index] = 0;
        /*wr_buf[wr_index] = 0;*/

	return size*nmemb ;
}


int GetHtmlByUrl( char *url, char *html )
{
	wr_index=0;
	/* CURL *pCurl = NULL; */
	char errmsg[512] ;
	char *p = NULL;
	char postdata[URL_LEN];
	char posturl[URL_LEN];
	CURLcode rc = CURLE_OK ;
	
	memset( errmsg, 0x00, sizeof( errmsg ) ) ;

	/*if( curl_flag == 0 )	
	{
		rc = curl_global_init( CURL_GLOBAL_ALL );
		if( rc != 0 )
		{
			hzb_log_error( __FILE__ , __LINE__ , "curl_global_init error" );
			return -1 ;
		}

		curl_flag = 1 ;
	}*/

	if( totalcnt%100 == 0 )
	{
		hzb_log_debug( __FILE__ , __LINE__ , "curl reconnect totalcnt=%d", totalcnt );
		curl_easy_cleanup( pCurl );
		pCurl = curl_easy_init();
		if( pCurl == NULL )
		{
			hzb_log_error( __FILE__ , __LINE__ , "curl_easy_init error" );
			return -2 ;
		}
	}

	p = strstr( url, POSTTYPE );
	if( p )
	{	
		memset( posturl, 0x00, sizeof(posturl) );
		memcpy( posturl, url, p - url );
		hzb_log_debug( __FILE__ , __LINE__ , "posturl [%s]", posturl );
		curl_easy_setopt( pCurl, CURLOPT_URL, posturl ); 

		p = p + 7;
		memset( postdata, 0x00, sizeof(postdata) );
		memcpy( postdata, p, url+strlen(url)- p );
		hzb_log_debug( __FILE__ , __LINE__ , "postdata [%s]", postdata );
    		curl_easy_setopt( pCurl, CURLOPT_POSTFIELDS, postdata ); 
    		curl_easy_setopt( pCurl, CURLOPT_POSTFIELDSIZE, strlen(postdata) ); 
	}
	else
		curl_easy_setopt( pCurl, CURLOPT_URL, url ) ;

	curl_easy_setopt( pCurl, CURLOPT_WRITEFUNCTION, CurlResponseProc ) ;
	curl_easy_setopt( pCurl, CURLOPT_WRITEDATA, html ) ;
	curl_easy_setopt( pCurl, CURLOPT_ERRORBUFFER, errmsg );
	curl_easy_setopt( pCurl, CURLOPT_TIMEOUT, 240 );
	curl_easy_setopt( pCurl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:34.0) Gecko/20100101 Firefox/34.0");
	
	hzb_log_debug( __FILE__ , __LINE__ , "curl_easy_init totalcnt=%d", totalcnt );
	rc = curl_easy_perform( pCurl ) ;
	if( rc != CURLE_OK )
	{
		hzb_log_error( __FILE__ , __LINE__ , "curl_easy_perform error [%s]" , errmsg );
		curl_easy_cleanup( pCurl );
		pCurl = curl_easy_init();
		if( pCurl == NULL )
		{
			hzb_log_error( __FILE__ , __LINE__ , "curl_easy_init error" );
			return -2 ;
		}
		return -1;
	}

	hzb_log_debug( __FILE__ , __LINE__ , "curl_easy_perform" );
	/*convchar(html);*/
	totalcnt++;

	/* curl_easy_cleanup( pCurl ); */

	return 0 ;
}


int t_GetHtmlByUrl( char *url, char *html )
{
	CURL *pCurll ;
	char errmsg[512] ;
	CURLcode rc = CURLE_OK ;
	
	memset( errmsg, 0x00, sizeof( errmsg ) ) ;

	/*if( curl_flag == 0 )	
	{
		rc = curl_global_init( CURL_GLOBAL_ALL );
		if( rc != 0 )
		{
			hzb_log_error( __FILE__ , __LINE__ , "curl_global_init error" );
			return -1 ;
		}

		curl_flag = 1 ;
	}*/

	pCurll = curl_easy_init();
	if( pCurll == NULL )
	{
		hzb_log_error( __FILE__ , __LINE__ , "curl_easy_init error" );
		return -2 ;
	}

	curl_easy_setopt( pCurll, CURLOPT_URL, url ) ;
	curl_easy_setopt( pCurll, CURLOPT_WRITEFUNCTION, CurlResponseProc ) ;
	curl_easy_setopt( pCurll, CURLOPT_WRITEDATA, html ) ;
	curl_easy_setopt( pCurll, CURLOPT_ERRORBUFFER, errmsg );
	curl_easy_setopt( pCurll, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:34.0) Gecko/20100101 Firefox/34.0");
	
	hzb_log_debug( __FILE__ , __LINE__ , "curl_easy_init" );
	rc = curl_easy_perform( pCurll ) ;
	if( rc != CURLE_OK )
	{
		hzb_log_error( __FILE__ , __LINE__ , "curl_easy_perform error [%s]" , errmsg );
	}
	hzb_log_debug( __FILE__ , __LINE__ , "curl_easy_perform" );
	totalcnt++;

	curl_easy_cleanup( pCurll );

	return 0 ;
}


int WriteHtmlToFile( char *filepath, char *html )
{
	FILE *fp = NULL ;

	if( filepath == NULL )
	{
		fprintf( stdout, "filepath is NULL\n" ) ;
		return -1 ;
	}

	fp = fopen( filepath, "w" ) ;	
	if( fp == NULL )
	{
		hzb_log_debug( __FILE__ , __LINE__ , "fopen error [%s]" , filepath );
		return -1 ;
	}
	
	fprintf( fp, "%s", html ) ;

	fclose( fp ) ;

	return 0 ;
}



/*
1. set seed html
2. get seed html
3. analyse seed html get href link, set todo list
4. get todo list analyse href link , set href link to todolist , set it to done list

question:
1. depth travel 
2. width travel 

todo:
1.multi thread
2.hash list
*/


int spider( int argc, char *argv[], int process_index )
{
	int nret = 0 ;
	int i = 0 ;
	int page = 0 ;
	char url[URL_LEN] ;
	char cururl[URL_LEN];
	char tmpurl[URL_LEN];
	char mainurl[URL_LEN] ;
	char htmldata[BUF_LEN] ;
	char filepath[512] ;
	char *ptrb = NULL ;
	char *ptre = NULL ;
	char *ptra = NULL ;
	char *ptrc = NULL ;
	FILE *fp = NULL ;
	int j = 0 ;
	int lines = 0;
	int len = 0 ;
	char buff[URL_LEN] ;
	char htmlname[URL_LEN];
	char content[URL_LEN];

	DList	todoList;
	DList	doneList;
	DList	filterList;
	DList_stepInfo  sTodoinfo;
	DList_stepInfo  sDoneinfo;
	DList_stepInfo	sFilterinfo;	
	char	urlcontent[URL_LEN];
	char	href[URL_LEN];
	char 	downfilepath[URL_LEN];
	int	ifound = 0;
	int	depth = 0;
	int	depth1 = 0;
	int	index = 0;
	int	ifilter = 0;
	char	keyword[100];
	char	*pfound = NULL;
	char	*pposttype = NULL;
	char	logfile[100];
	stDefMsgBuf	stMsgBuf;
	stDefMsgBuf	*pstMsgBuf=NULL;
	stDefurldata	*psturldata=NULL;
	int	iLen = 0;
	int	analyse_cnt = 0;

        DList_new(&todoList);
        DList_new(&doneList);
	DList_new(&filterList);

        /*nret = hzb_log_init();
        if( nret )
        {
                printf( "hzb_log_init failed[%d]\n" , nret );
                return -1; 
        }*/

	sprintf( logfile, "spider_%d", getpid() );	
        nret = hzb_log_set_category( logfile );
        if( nret )
        {
                printf( "hzb_log_set_category failed[%d]\n" , nret );
                return -1;
        }

	open_resource();

#ifdef	THREAD
	t_open_resource();
#endif

	/* get seed url */
	memset( seedurl, 0x00, sizeof(seedurl) );
	strcpy( seedurl, argv[0] );
	hzb_log_info( __FILE__ , __LINE__ , "seedurl [%s]", seedurl );


	/* get depth */
	travel_depth = atoi( argv[1] );
	hzb_log_debug( __FILE__ , __LINE__ , "travel depth [%d]" , travel_depth );
	if( travel_depth == 0)
		travel_depth = DEFAULT_DEPTH;

	if( travel_depth == -1 )
		icontrol = 0;

#ifdef	COMMON

	memset( htmldata, 0x00, sizeof( htmldata ) ) ;
	nret = GetHtmlByUrl( seedurl, htmldata ) ;		
	if( nret < 0 )
	{
		hzb_log_error( __FILE__ , __LINE__ , "GetHtmlByUrl error [%s]" , seedurl );
		return -1 ;
	}	

	/* add seed to todo list */
	depth = 1;
	if( strstr(seedurl, "shixin.court.gov.cn") )
	{
		memset( url, 0x00, sizeof(url) );
		sprintf( url, "%s/unitMore.do", seedurl );
		DList_add(&todoList, url, NULL, depth, NULL); 
	}
	else
		DList_add(&todoList, seedurl, NULL, depth, NULL); 
#endif


#ifdef THREAD
	depth++;
	while(1)
	{
		if( ptre )
			ptrb = strstr( ptre, "<a href=" ) ;
		else
			ptrb = strstr( htmldata, "<a href=" ) ;
		if( ptrb == NULL )
		{
			break;
		}

		ptre = strstr( ptrb, "</a>" ) ;
		if( ptre == NULL )
		{
			hzb_log_debug( __FILE__ , __LINE__ , "not found [ ]");
			break;
		}

		memset( tmpurl, 0x00, sizeof(tmpurl) ) ;

		if( ptre - ptrb > URL_LEN )
		{
			memcpy( tmpurl, ptrb+8, URL_LEN-1 ) ;  
			hzb_log_debug( __FILE__ , __LINE__ , "url too long [%s]", tmpurl  );
			continue;
		}
		else
			memcpy( tmpurl, ptrb+8, ptre - ptrb ) ;  

		memset( urlcontent, 0x00, sizeof(urlcontent) );
		memset( href, 0x00, sizeof(href) );
		nret = get_href( tmpurl, seedurl, href, urlcontent, url );
		
		if( strlen(href) <= 0 )	
			continue;
		hzb_log_debug( __FILE__ , __LINE__ , "href[%s] urlcotent[%s]" , href, urlcontent );

		/*if( ( (icontentfilter || iurlfilter) && checkfilter(urlcontent, href) ) ||
			( !icontentfilter && !iurlfilter )
		)*/
		{
			nret = DList_add( &todoList, href, urlcontent, depth, checkurl );
			if( nret )
				hzb_log_error( __FILE__ , __LINE__ , "add todolist duplicate [%s]" , href );
			else
				hzb_log_debug( __FILE__ , __LINE__ , "add todolist [%s]" , href );
				
		}
		ifound = 0;
	}
#endif


#ifdef	MULTI_PROC

	hzb_log_debug( __FILE__ , __LINE__ , "process_index=%d", process_index );

	get_shixin_pagecondition( seedurl );

        char ipckey[100];
        sprintf(ipckey,"%s", getenv("IPCKEY"));
	OpenUnixMsgQ(atol(ipckey));

        memset((char*)&stmem, 0, sizeof(stmem));
        nret = OpenSharedMem(&stmem, atol(ipckey), MAX_SHAREMEM_LEN);
        if (nret != SUCC)
        {
                hzb_log_error( __FILE__ , __LINE__ , " get sharemem error r=%d" , nret );
                return nret;
        }
	
	/* memset( stmem.pcData, 0x00, MAX_SHAREMEM_LEN ); */

	for(i=0;i<URL_NUM;i++)
		donelist[i] = stmem.pcData + i*BUFF_LEN;

	/* analyse seed html get href link, set todo list */
	while(1)
	{
		memset( &stMsgBuf, 0x00, sizeof(stMsgBuf) );
		nret = IntervalRecvUnixMsgQ(giQidOut, &stMsgBuf, QUEUE_DEFAULT_QUEUETYPE, 180, &iLen);
		if (nret != SUCC)
			break;

		memset( url, 0x00, sizeof(url) );
		memset( urlcontent, 0x00, sizeof(urlcontent) );
		depth = 0;
		analyse_cnt = 0;

		psturldata = (stDefurldata *)&stMsgBuf.ucaText;
		strcpy( url, psturldata->url );	
		/*strcpy( urlcontent, psturldata->urlcontent );*/
		depth = psturldata->depth;
		analyse_cnt = psturldata->analyse_cnt;

		hzb_log_debug( __FILE__ , __LINE__ , "get url=[%s]" , url );
		/*hzb_log_debug( __FILE__ , __LINE__ , "get urlcontent=[%s]" , urlcontent );*/
		hzb_log_debug( __FILE__ , __LINE__ , "get depth=[%d]" , depth );
		hzb_log_debug( __FILE__ , __LINE__ , "get analyse_cnt=[%d]" , analyse_cnt );

		/*nret = LockSharedMem(&stmem);
		if (nret != SUCC) 
		{
			hzb_log_debug( __FILE__ , __LINE__ , "semop lock error" );
			return	nret;
		}
		for(i=0;i<URL_NUM;i++)
		{
			if(strlen(donelist[i])<=0)
				continue;
					
			if(strcmp(donelist[i], url)==0)
			{
				hzb_log_debug( __FILE__ , __LINE__ , "check sharemem exsit url=[%s]" , donelist[i] );
				icontinue = 1;
				break;
			}
		}
		UnlockSharedMem(&stmem);
		if(icontinue)
		{
			icontinue = 0;
			continue;
		}*/

		if( analyse_cnt < 5 )
		{
			if( icontrol && depth > travel_depth )
			{
				hzb_log_debug( __FILE__ , __LINE__ , " travel depth=[%d] > [%d]" , depth, travel_depth );
				continue;
			}

			hzb_log_debug( __FILE__ , __LINE__ , "begin analyse todolist url=[%s], urlcontent=[%s], depth=[%d],todoindex=[%d]", url, urlcontent, depth, index );

			/*nret = DList_add( &doneList, url, urlcontent, depth, NULL );
			if( nret < 0 )
			{
				if( nret == DLIST_DUP_USERDATA )
					hzb_log_error( __FILE__ , __LINE__ , "Dlist_add donelist duplicate url=[%s]" , url );
				continue;
			}*/

			memset( htmldata, 0x00, sizeof(htmldata) );
			nret = GetHtmlByUrl( url, htmldata );
			if( nret < 0 )
			{
				hzb_log_error( __FILE__ , __LINE__ , "GetHtmlByUrl error [%s]" , url );
				psturldata->analyse_cnt++;
				stMsgBuf.stMsgHead.lQTypeSend = QUEUE_DEFAULT_QUEUETYPE;
				SendUnixMsgQ(giQidOut, &stMsgBuf, sizeof(stDefInternalMsgHead)+sizeof(stDefurldata) );
				continue;
			}
			hzb_log_debug( __FILE__ , __LINE__ , "htmldata:[%d][%s]",strlen(htmldata), htmldata );
			
			/* for shixin */
			if( strstr(seedurl, "shixin.court.gov.cn") )
			{
				if( !ifileflag )	
				{
					sprintf( filepath, "%s/log/%s_%d", getenv("HOME"), seedurl, getpid() );
					fp = fopen( filepath, "w" );
					if( !fp )
					{
						hzb_log_debug( __FILE__ , __LINE__ , "fopen error\n"  );
						return -1;
					}
					ifileflag = 1;
				}

				if( ifilecontentinvolve || ifileurlinvolve )
				{
					if( checkfilelist(urlcontent, url) )
					{
						fprintf( fp, "[%-50s] [%s]\n", urlcontent, url );
						if( idownload )
						{
							char *pp=NULL;
							pp=strstr(url,"id=");
							sprintf( downfilepath, "%s/%s.json", basepath, pp+3 );
							convchar(htmldata);
							WriteHtmlToFile( downfilepath, htmldata );
							
							DSCDBBEGINWORK();
							court_shixin_task task;	
							memset( &task, 0x00, sizeof(task) );	
							strcpy( task.task_type, "unit" );	
							task.task_level = 2;
							strcpy( task.url, url );
							task.curl_result = 200;
							strcpy(task.pathfilename, downfilepath);		
							task.parse_result = 0;
							sprintf(task.remark, "%s", pp+3);
							DSCSQLACTION_INSERT_INTO_court_shixin_task( & task );
							if( SQLCODE )
							{       
								hzb_log_error( __FILE__ , __LINE__ , "DSCSQLACTION_INSERT_INTO_court_shixin_task failed[%d][%s][%s]" , SQLCODE , SQLSTATE, sqlca.sqlerrm.sqlerrmc );
								DSCDBROLLBACK();
							}       
							else
							{
								hzb_log_debug( __FILE__ , __LINE__ , "DSCSQLACTION_INSERT_INTO_court_shixin_task ok" );
								DSCDBCOMMIT();
							}
 
						}
					}
				}
				else
				{
					fprintf( fp, "[%-50s] [%s]\n", urlcontent, url );
					if( idownload )
					{
						char *pp=NULL;
						pp=strstr(url,"id=");
						sprintf( downfilepath, "%s/%s.json", basepath, pp+3 );
						convchar(htmldata);
						WriteHtmlToFile( downfilepath, htmldata );
					}
				}
			}

			depth++;

			while(1)
			{
				if( ptre )
					ptrb = strstr( ptre, "href=" ) ;
				else
					ptrb = strstr( htmldata, "href=" ) ;
				if( ptrb == NULL )
				{
					break;
				}

				ptre = strstr( ptrb, "</a>" ) ;
				if( ptre == NULL )
				{
					hzb_log_debug( __FILE__ , __LINE__ , "not found [ ]");
					break;
				}

				memset( tmpurl, 0x00, sizeof(tmpurl) ) ;

				if( ptre - ptrb > URL_LEN )
				{
					memcpy( tmpurl, ptrb+5, URL_LEN-1 ) ;  
					hzb_log_debug( __FILE__ , __LINE__ , "url too long [%s]", tmpurl  );
					continue;
				}
				else
					memcpy( tmpurl, ptrb+5, ptre - ptrb ) ;  


				memset( urlcontent, 0x00, sizeof(urlcontent) );
				memset( href, 0x00, sizeof(href) );

				/* for shixin  set post url */
				memset( cururl, 0x00, sizeof(cururl) );
				pposttype = strstr(url, POSTTYPE); 
				if( pposttype )
					memcpy( cururl, url, pposttype-url );
				else
					strcpy( cururl, url );
				
				nret = get_href( tmpurl, seedurl, href, urlcontent, cururl );
				
				if( strlen(href) <= 0 )	
					continue;

				hzb_log_debug( __FILE__ , __LINE__ , "href[%s] urlcotent[%s]" , href, urlcontent );

				if( ((icontentinvolve || iurlinvolve) && checkinvolve(urlcontent, href)) ||
					( !icontentinvolve && !iurlinvolve )
				)
					iflag = 1;

				if( ((icontentfilter || iurlfilter) && checkfilter(urlcontent, href)) )
					iflag = 0;
				
				if( iflag )
				{
					if( !checkurl(href) )
					{
						nret = LockSharedMem(&stmem);
						if (nret != SUCC) 
						{
							hzb_log_debug( __FILE__ , __LINE__ , "semop lock error" );
							return	nret;
						}
						for(i=0;i<URL_NUM;i++)
						{
							if(strlen(donelist[i])<=0)
								continue;
									
							if(strcmp(donelist[i], href)==0)
							{
								hzb_log_debug( __FILE__ , __LINE__ , "check sharemem exsit url=[%s] Do not SendQ " , donelist[i] );
								icontinue = 1;
								break;
							}
						}
						if(icontinue)
						{
							icontinue = 0;
							iflag = 0;
							UnlockSharedMem(&stmem);
							continue;
						}
						for(i=0;i<URL_NUM;i++)
						{
							if(strlen(donelist[i])>0)
								continue;
									
							strcpy(donelist[i], href);
							hzb_log_debug( __FILE__ , __LINE__ , "write sharemem url=[%s] i=[%d]", href,i );
							break;	
						}
						UnlockSharedMem(&stmem);

						memset( &stMsgBuf.ucaText, 0x00, sizeof(stMsgBuf.ucaText) );
						stMsgBuf.stMsgHead.lQTypeSend = QUEUE_DEFAULT_QUEUETYPE;
						strcpy( psturldata->url, href );
						/*strcpy( psturldata->urlcontent, urlcontent );*/
						psturldata->depth = depth;
						psturldata->analyse_cnt = 0;
						SendUnixMsgQ(giQidOut, &stMsgBuf, sizeof(stDefInternalMsgHead)+sizeof(stDefurldata) );
					}
				}
				iflag = 0;
			}
			ptre = NULL;
			ptrb = NULL;
			hzb_log_debug( __FILE__ , __LINE__ , "end analyse todolist url=[%s], todoindex=[%d]" , url, index );
			depth = 0;
			memset( url, 0x00, sizeof(url) );
			memset( urlcontent, 0x00, sizeof(urlcontent) );
			index++;
		}
	}
	if( fp )
		fclose(fp);
	CloseSharedMem( &stmem );
#endif



#ifdef COMMON
	/*3. analyse seed html get href link, set todo list */
	while(1)
	{
		if( todoList.cnt <= 0 )
			break;
		
		hzb_log_debug( __FILE__ , __LINE__ , "todo list cnt=[%d]" , todoList.cnt );

		DList_openStep( &todoList, &sTodoinfo );
		memset( url, 0x00, sizeof(url) );
		memset( urlcontent, 0x00, sizeof(urlcontent) );
		while ( DList_fetchStep( &todoList, &sTodoinfo, url, urlcontent, &depth, &index ) == 0 )
		{

			if( icontrol && depth > travel_depth )
			{
				hzb_log_debug( __FILE__ , __LINE__ , " travel depth=[%d] > [%d]" , depth, travel_depth );
				DList_del( &todoList, url, NULL );
				break;
			}

			hzb_log_debug( __FILE__ , __LINE__ , "begin analyse todolist url=[%s], urlcontent=[%s], depth=[%d],todoindex=[%d]", url, urlcontent, depth, index );

			nret = DList_add( &doneList, url, urlcontent, depth, NULL );
			if( nret < 0 )
			{
				if( nret == DLIST_DUP_USERDATA )
				{
					hzb_log_error( __FILE__ , __LINE__ , "Dlist_add donelist duplicate url=[%s]" , url );
					DList_del( &todoList, (char *)url, NULL );
				}
				break;
			 }

			memset( htmldata, 0x00, sizeof(htmldata) );
			nret = GetHtmlByUrl( url, htmldata );
			if( nret < 0 )
			{
				hzb_log_error( __FILE__ , __LINE__ , "GetHtmlByUrl error [%s]" , url );
				continue;
			}
			hzb_log_debug( __FILE__ , __LINE__ , "htmldata:[%d][%s]",strlen(htmldata), htmldata );
			
			/* for shixin */
			if( strstr(seedurl, "shixin.court.gov.cn") )
			{
				if( !ifileflag )	
				{
					sprintf( filepath, "%s/log/%s_%d", getenv("HOME"), seedurl, getpid() );
					fp = fopen( filepath, "w" );
					if( !fp )
					{
						hzb_log_debug( __FILE__ , __LINE__ , "fopen error\n"  );
						return -1;
					}
					ifileflag = 1;
				}

				if( ifilecontentinvolve || ifileurlinvolve )
				{
					if( checkfilelist(urlcontent, url) )
					{
						fprintf( fp, "[%-50s] [%s]\n", urlcontent, url );
						if( idownload )
						{
							sprintf( downfilepath, "%s/%d.jason", basepath, totalcnt );
							WriteHtmlToFile( downfilepath, htmldata );
						}
					}
				}
				else
				{
					fprintf( fp, "[%-50s] [%s]\n", urlcontent, url );
					if( idownload )
					{
						sprintf( downfilepath, "%s/%d.jason", basepath, totalcnt );
						WriteHtmlToFile( downfilepath, htmldata );
					}
				}
			}

			nret = DList_del( &todoList, (char *)url, NULL );
			if( nret < 0 )
				hzb_log_error( __FILE__ , __LINE__ , "DList_del error url=[%s]" , url );

			depth++;

			while(1)
			{
				if( ptre )
					ptrb = strstr( ptre, "href=" ) ;
				else
					ptrb = strstr( htmldata, "href=" ) ;
				if( ptrb == NULL )
				{
					break;
				}

				ptre = strstr( ptrb, "</a>" ) ;
				if( ptre == NULL )
				{
					hzb_log_debug( __FILE__ , __LINE__ , "not found [ ]");
					break;
				}

				memset( tmpurl, 0x00, sizeof(tmpurl) ) ;

				if( ptre - ptrb > URL_LEN )
				{
					memcpy( tmpurl, ptrb+5, URL_LEN-1 ) ;  
					hzb_log_debug( __FILE__ , __LINE__ , "url too long [%s]", tmpurl  );
					continue;
				}
				else
					memcpy( tmpurl, ptrb+5, ptre - ptrb ) ;  


				memset( urlcontent, 0x00, sizeof(urlcontent) );
				memset( href, 0x00, sizeof(href) );

				/* for shixin  set post url */
				memset( cururl, 0x00, sizeof(cururl) );
				pposttype = strstr(url, POSTTYPE); 
				if( pposttype )
					memcpy( cururl, url, pposttype-url );
				else
					strcpy( cururl, url );
				
				nret = get_href( tmpurl, seedurl, href, urlcontent, cururl );
				
				if( strlen(href) <= 0 )	
					continue;
				hzb_log_debug( __FILE__ , __LINE__ , "href[%s] urlcotent[%s]" , href, urlcontent );

				if( ((icontentinvolve || iurlinvolve) && checkinvolve(urlcontent, href)) ||
					( !icontentinvolve && !iurlinvolve )
				)
					iflag = 1;

				if( ((icontentfilter || iurlfilter) && checkfilter(urlcontent, href)) )
					iflag = 0;
				
				if( iflag )
				{
					nret = DList_add( &todoList, href, urlcontent, depth, checkurl );
					if( nret )
						hzb_log_error( __FILE__ , __LINE__ , "add todolist duplicate [%s]" , href );
					else
						hzb_log_debug( __FILE__ , __LINE__ , "add todolist [%s]" , href );
				}
				iflag = 0;

			}
			ptre = NULL;
			ptrb = NULL;
			hzb_log_debug( __FILE__ , __LINE__ , "end analyse todolist url=[%s], todoindex=[%d]" , url, index );
			depth = 0;
			memset( url, 0x00, sizeof(url) );
			memset( urlcontent, 0x00, sizeof(urlcontent) );

		}
		DList_closeStep( &todoList,&sTodoinfo );

	}
#endif

#ifdef THREAD
	index = 0;
	i = 0;
	memset( url, 0x00, sizeof(url) );
	memset( urlcontent, 0x00, sizeof(urlcontent) );
	DList_openStep( &todoList, &sTodoinfo );
	while ( DList_fetchStep( &todoList, &sTodoinfo, url, urlcontent, &depth, &index ) == 0 )
	{
		hzb_log_debug( __FILE__ , __LINE__ , "TodoList [%s]" , url );
		j = i%THREAD_CNT;
		DList_add( &t_todoList[j], url, urlcontent, depth, checkurl );
		memset( url, 0x00, sizeof(url) );
		memset( urlcontent, 0x00, sizeof(urlcontent) );
		i++;
	}
	DList_closeStep( &todoList,&sTodoinfo );

	hzb_log_debug( __FILE__ , __LINE__ , "depth 1 cnt=[%d]" , todoList.cnt );

	memset(g_caDebugLogFile, 0, sizeof(g_caDebugLogFile));
	sprintf( g_caDebugLogFile, "%s/log/spider_%d", getenv("HOME"), getpid() );
	g_iLogLevel = 5;

	/* DebugLog(LOG_LEVEL_ALL, NULL, 0, "[%s.%d]start create thread ", __FILE__, __LINE__); */
	hzb_log_debug( __FILE__ , __LINE__ , "start create thread");

	i = 0;
	for( i=0; i<THREAD_CNT; i++ )
	{
		env[i].todo_list = &t_todoList[i];
		env[i].done_list = &t_doneList[i];
		env[i].info = &t_sTodoinfo[i];
		nret = pthread_create( &threadId[i], NULL, (void *(*)(void*))analysehtml, (void *) &env[i] );
		/* nret = pthread_create( &threadId[i], NULL, analysehtml, (void *)&t_todoList[i] ); */
		if( nret ) 
		{
			/* DebugLog(LOG_LEVEL_ALL, NULL, 0, "[%s.%d]pthread_create fail [%d]", __FILE__, __LINE__, nret); */
			hzb_log_debug( __FILE__ , __LINE__ , "pthread_create fail [%d]" , nret );
			return -1;
		}
		else
			/* DebugLog(LOG_LEVEL_ALL, NULL, 0, "[%s.%d]pthread_create succ tid=[%d]", __FILE__, __LINE__, threadId[i] );*/
			hzb_log_debug( __FILE__ , __LINE__ , "tid[%d] pthread_create succ", threadId[i] );
		/* memset(g_caDebugLogFile, 0, sizeof(g_caDebugLogFile));
		sprintf( g_caDebugLogFile, "%s/log/spider_%d", getenv("HOME"), threadId[i] );
		g_iLogLevel = 5;*/
	}

	for( i=0; i<THREAD_CNT; i++ )
	{
		nret = pthread_join( threadId[i], NULL );
		if( nret )
		{
			/* DebugLog(LOG_LEVEL_ALL, NULL, 0, "[%s.%d]pthread_join fail [%d]", __FILE__, __LINE__, nret); */
			hzb_log_debug( __FILE__ , __LINE__ , "pthread_join fail [%d]" , nret );
			return -1;
		}
		else
			/* DebugLog(LOG_LEVEL_ALL, NULL, 0, "[%s.%d]pthread_join succ tid=[%d]", __FILE__, __LINE__, threadId[i]); */
			hzb_log_debug( __FILE__ , __LINE__ , "tid[%d] pthread_join succ" , threadId[i] );
	}

#endif

#ifdef COMMON
	sprintf( filepath, "%s/log/%s_%d", getenv("HOME"), seedurl, getpid() );
	fp = fopen( filepath, "w" );
	if( !fp )
	{
		hzb_log_debug( __FILE__ , __LINE__ , "fopen error\n"  );
		return -1;
	}
	memset( url, 0x00, sizeof(url) );
	memset( urlcontent, 0x00, sizeof(urlcontent) );
	iflag = 0;
	DList_openStep( &doneList, &sDoneinfo );
	while ( DList_fetchStep( &doneList, &sDoneinfo, url, urlcontent, &depth, &index ) == 0 )
	{
		hzb_log_debug( __FILE__ , __LINE__ , "DoneList url[%s] content[%s] depth[%d], doneindex[%d]" , url, urlcontent, depth, index );

		if( strstr(seedurl, "shixin.court.gov.cn") )
			continue;
		
		if( ifilecontentinvolve || ifileurlinvolve )
		{
			if( checkfilelist(urlcontent, url) )
			{
				fprintf( fp, "[%-50s] [%s]\n", urlcontent, url );
				if( idownload )
					downloadhtml( url, index );
			}
		}
		else
		{
			fprintf( fp, "[%-50s] [%s]\n", urlcontent, url );
			if( idownload )
				downloadhtml( url, index );
		}

		memset( url, 0x00, sizeof(url) );
		memset( urlcontent, 0x00, sizeof(urlcontent) );
	}
	fclose(fp);
	DList_closeStep( &doneList,&sDoneinfo );
	hzb_log_debug( __FILE__ , __LINE__ , "complete [%d]\n" , doneList.cnt );
#endif

	hzb_log_debug( __FILE__ , __LINE__ , "totalcnt [%d]\n" , totalcnt );

	close_resource();

	return 0 ;
}



int get_href( char *url, char *seedurl, char *href, char *content, char *cur_url ) 
{
	char *pa = NULL;
	char *pb = NULL;
	char *pc = NULL;
	char *pd = NULL;
	char *pe = NULL;
	char *pf = NULL;
	char *pg = NULL;
	char buf[URL_LEN];
	char buf1[URL_LEN];

	pa = strchr( url, '"' );
	if( pa == NULL )
	{
		pa = strstr( url, "¡°" ); 
		if( pa == NULL )	
			return -1;
		else
		
		pa=pa+2;
		pb = strstr( pa, "¡°");
		if( pb == NULL )
		{
			hzb_log_debug( __FILE__ , __LINE__ , "pb is null" );
			return -1;	
		}
		memset( buf, 0x00, sizeof(buf) );
		memcpy( buf, pa, pb-pa );
	}
	else
	{
		pa++;
		pb = strchr( pa, '"' );
		if( pb == NULL )
		{
			hzb_log_debug( __FILE__ , __LINE__ , "pb is null" );
			return -1;	
		}
		memset( buf, 0x00, sizeof(buf) );
		memcpy( buf, pa, pb-pa );
	}

	if( strlen(buf) > 200 )
		return 0;
	
	/* local */
	if( strcmp(buf, "#") == 0 )
	{
		if( strstr(seedurl, "shixin.court.gov.cn") )
		{
			pa = NULL;
			pb = NULL;
			/* for shixin.gov.cn */
			pa = strchr( url, '\'' );
			if( pa == NULL )
			{
				hzb_log_debug( __FILE__ , __LINE__ , "pb is null" );
				return -1;	
			}
			pa++;
			pb = strchr( pa, '\'' );
			if( pb == NULL )
			{
				hzb_log_debug( __FILE__ , __LINE__ , "pb is null" );
				return -1;	
			}
			memset( buf, 0x00, sizeof(buf) );
			memcpy( buf, pa, pb-pa );
			hzb_log_debug( __FILE__ , __LINE__ , "when find # [%s] ", buf );
			sprintf( href, "%s/%s", seedurl, buf);		
			return 0;
		}
		return 0;
	}

	char *a[6] = {"rar", "pdf", "zip", "jsp", "exe", "jpg"};
	int i = 0;
	for( i=0; i<6; i++ )
	{
		if( strcmp( (char *)&buf[strlen(buf)-3], a[i] ) == 0 )
		{
			hzb_log_debug( __FILE__ , __LINE__ , "end filter url[%s]" , a[i] );
			return -1;
		}
	}
		
	if( strncmp( buf, "http", 4) != 0 )
	{
		if( (memcmp( buf, "javascript:void", strlen("javascript:void") ) != 0) &&
			( buf[0] == '/' )
		)
			sprintf( href, "%s%s", seedurl, buf );
		else
		{
			if( strstr(seedurl, "shixin.court.gov.cn") )
			{
				/* javascrpit:void*/
				hzb_log_debug( __FILE__ , __LINE__ , "javascrpit void [%s]", url );
				pe = strstr( url, "id=" );
				if( pe == NULL )
				{
					hzb_log_debug( __FILE__ , __LINE__ , "can not find id= try to find onclick" );
					pe = strstr( url, "onclick=" );
					if( pe == NULL )
					{
						hzb_log_debug( __FILE__ , __LINE__ , "can not find id= try to find onclick" );
						return -1;
					}
					pe=pe+9;
					pf = strchr( pe, '>' );
					if( pb == NULL )
					{
						hzb_log_debug( __FILE__ , __LINE__ , "pb is null" );
						return -1;	
					}
					memset( buf, 0x00, sizeof(buf) );
					memcpy( buf, pe, pf-pe-1 );
					hzb_log_debug( __FILE__ , __LINE__ , "when find onclick= [%s] ", buf );
					if( strncmp(buf, "gotoPage", 8) == 0 )	
					{
						pa = NULL;
						pb = NULL;
						pa = strchr(buf, '(');
						if( pa == NULL )
						{
							hzb_log_debug( __FILE__ , __LINE__ , "can not find gotoPage " );
							return -1;
						}
						pa++;
						pb = strchr(pa, ')');
						if( pb == NULL )
						{
							hzb_log_debug( __FILE__ , __LINE__ , "can not find gotoPage " );
							return -1;
						}
						char page[10];
						memset( page, 0x00, sizeof(page));
						memcpy( page, pa, pb-pa );

						if( llt && !lgt )
						{
							if( atol(page) > llt )
							{
								hzb_log_debug( __FILE__ , __LINE__ , "pagenum not satisfy cur[%ld] llt[%ld]", atol(page), llt );
								if(!lt_flag)
								{
									hzb_log_debug( __FILE__ , __LINE__ , "init condition url llt=[%d]" , llt );
									sprintf( href, "%s%scurrentPage=%ld", cur_url, POSTTYPE, llt);
									lt_flag=1;
								}
								return 0;	
							}
						}	

						if( lgt && !llt )
						{
							if( atol(page) < lgt )
							{
								hzb_log_debug( __FILE__ , __LINE__ , "pagenum not satisfy cur[%ld] lgt[%ld]", atol(page), lgt );
								if(!gt_flag)
								{
									hzb_log_debug( __FILE__ , __LINE__ , "init condition url lgt=[%ld]" , lgt );
									sprintf( href, "%s%scurrentPage=%ld", cur_url, POSTTYPE, lgt);
									gt_flag=1;
								}
								return 0;	
							}
						}
							
						if( llt && lgt )
						{
							if( atol(page) > llt || atol(page) < lgt )
							{
								hzb_log_debug( __FILE__ , __LINE__ , "pagenum not satisfy cur[%ld] lgt[%ld] llt[%ld]", atol(page), lgt, llt );
								if(!and_flag)
								{
									hzb_log_debug( __FILE__ , __LINE__ , "init condition url llt=[%ld], lgt=[%ld] " , llt, lgt );
									sprintf( href, "%s%scurrentPage=%ld", cur_url, POSTTYPE, lgt);
									and_flag=1;
								}
								return 0;	
							}
						}

						sprintf( href, "%s%scurrentPage=%s", cur_url, POSTTYPE, page);
						return 0;
					}

				}
				pe=pe+4;
				pf = strchr( pe, '>' );
				if( pb == NULL )
				{
					hzb_log_debug( __FILE__ , __LINE__ , "pb is null" );
					return -1;	
				}
				memset( buf, 0x00, sizeof(buf) );
				memcpy( buf, pe, pf-pe-1 );
				hzb_log_debug( __FILE__ , __LINE__ , "when find id= [%s] cur_url=[%s], seedurl=[%s] ", buf, cur_url, seedurl );
				sprintf( href, "%s/detail?id=%s", seedurl, buf);		
				return 0;
			}
			return 0;
		}
	}
	else
	{
		if( strncmp(seedurl, buf, strlen(seedurl)) == 0 )
			sprintf( href, "%s", buf );
		else
		{
			/* search if href involve part url */
			pe = strchr( seedurl, '.' );
			if( pe == NULL )
			{
				hzb_log_debug( __FILE__ , __LINE__ , "pe is null" );
				return -1;	
			}
			pe++;

			pf = strchr( pe, '.' );
			if( pf == NULL )
			{
				hzb_log_debug( __FILE__ , __LINE__ , "pf is null" );
				return -1;	
			}
			memset( buf1, 0x00, sizeof(buf1) );
			memcpy( buf1, pe, pf-pe );

			pg = strstr( url, buf1 );
			if( pg == NULL )	
				return 0;
			else
			{
				sprintf( href, "%s", buf );
			}
		}
	}

	pb++;
	pc = strchr( pb, '>' );
	if( pc == NULL )
	{
		return -1;	
	}
	pc++;

	pd = strchr( pc, '<' );
	if( pd == NULL )
	{
		return -1;	
	}
	/* memcpy( content, pc, pd-pc ); */

	/* convert characters */
	char *outBuf = NULL;
	char *inBuf = NULL;
	char cabuffer[4096];	
	memset(cabuffer, 0, sizeof(cabuffer));
	size_t rtsize=0, insize=0, outsize=0, orgoutsize = 0;
	
	/*iconv_toinchar = iconv_open("GB18030",  "UTF-8");*/

	strncpy( cabuffer, pc, pd-pc );

	char *b[8] = {"<", ">", "!", ";", "www", "User-Agent", "http", "div"};
	for( i=0; i<8; i++ )
	{
		if(strstr(cabuffer, b[i]))
		{
			hzb_log_debug( __FILE__ , __LINE__ , "check content error [%s]", b[i] );
			return -1;
		}
	}

	trimSpace(cabuffer);

	if( strlen(cabuffer) <= 0 )
		return 0;

	/*inBuf = cabuffer;
	insize = strlen(inBuf);
	outBuf = cabuffer;
	outsize = sizeof(cabuffer);
	orgoutsize = outsize;

	rtsize = iconv(iconv_toinchar,
		(char**)&inBuf, &insize,
		(char**)&outBuf, &outsize);

	cabuffer[orgoutsize - outsize] = '\0';*/
	strcpy(content, cabuffer);

	return 0;
}


int convchar( char *cabuffer )
{
	/* convert characters */
	char *outBuf = NULL;
	char *inBuf = NULL;
	size_t rtsize=0, insize=0, outsize=0, orgoutsize = 0;

	if( strlen(cabuffer) <= 0 )
		return 0;

	inBuf = cabuffer;
	insize = strlen(inBuf);
	outBuf = cabuffer;
	outsize = BUF_LEN ;
	orgoutsize = outsize;

	rtsize = iconv(iconv_toinchar,
		(char**)&inBuf, &insize,
		(char**)&outBuf, &outsize);

	cabuffer[orgoutsize - outsize] = '\0';
	return 0;
}


void open_resource()
{
	FILE *fp = NULL;
	char keyword[URL_LEN];
	char filepath[URL_LEN];
	int i = 0;

 	if( getenv("DBHOST") == NULL || getenv("DBPORT") == NULL || getenv("DBNAME") == NULL || getenv("DBUSER") == NULL || getenv("DBPASS") == NULL ) 
        {       
		hzb_log_error( __FILE__ , __LINE__ , "db env error" );
                return ; 
        }
                
        DSCDBCONN( getenv("DBHOST") , atoi(getenv("DBPORT")) , getenv("DBNAME") , getenv("DBUSER") , getenv("DBPASS") ); 
        if( SQLCODE )
        {       
		hzb_log_error( __FILE__ , __LINE__ , "DSCDBCONN failed[%d][%s]\n" , SQLCODE , SQLSTATE );
                return ;
        }

	pCurl = curl_easy_init();
	if( pCurl == NULL )
	{
		hzb_log_error( __FILE__ , __LINE__ , "curl_easy_init error" );
		return ;
	}
	iconv_toinchar = iconv_open("GB18030",  "UTF-8");

	for(i=0;i<MAX_CNT;i++)
	{
		urlfilter[i] = (char *)malloc(URL_LEN);
		if(!urlfilter[i])
		{
			hzb_log_debug( __FILE__ , __LINE__ , "malloc fail" );
			exit(-1);
		}
		memset( urlfilter[i], 0x00, URL_LEN );
	}

	for(i=0;i<MAX_CNT;i++)
	{
		contentfilter[i] = (char *)malloc(URL_LEN);
		if(!contentfilter[i])
		{
			hzb_log_debug( __FILE__ , __LINE__ , "malloc fail" );
			exit(-1);
		}
		memset( contentfilter[i], 0x00, URL_LEN );
	}

	for(i=0;i<MAX_CNT;i++)
	{
		urlinvolve[i] = (char *)malloc(URL_LEN);
		if(!urlinvolve[i])
		{
			hzb_log_debug( __FILE__ , __LINE__ , "malloc fail" );
			exit(-1);
		}
		memset( urlinvolve[i], 0x00, URL_LEN );
	}

	for(i=0;i<MAX_CNT;i++)
	{
		contentinvolve[i] = (char *)malloc(URL_LEN);
		if(!contentinvolve[i])
		{
			hzb_log_debug( __FILE__ , __LINE__ , "malloc fail" );
			exit(-1);
		}
		memset( contentinvolve[i], 0x00, URL_LEN );
	}

	for(i=0;i<MAX_CNT;i++)
	{
		fileurlinvolve[i] = (char *)malloc(URL_LEN);
		if(!fileurlinvolve[i])
		{
			hzb_log_debug( __FILE__ , __LINE__ , "malloc fail" );
			exit(-1);
		}
		memset( fileurlinvolve[i], 0x00, URL_LEN );
	}

	for(i=0;i<MAX_CNT;i++)
	{
		filecontentinvolve[i] = (char *)malloc(URL_LEN);
		if(!filecontentinvolve[i])
		{
			hzb_log_debug( __FILE__ , __LINE__ , "malloc fail" );
			exit(-1);
		}
		memset( filecontentinvolve[i], 0x00, URL_LEN );
	}


	/* get filter words */
	i = 0;
	sprintf( filepath, "%s/etc/contentfilter.conf", getenv("HOME") );
	fp = fopen( filepath, "r" );
	if( fp == NULL )
	{
		hzb_log_debug( __FILE__ , __LINE__ , "no contentfilter.conf file" );
	}
	else
	{
		hzb_log_debug( __FILE__ , __LINE__ , "get contentfilter.conf keyword" );
		memset( keyword, 0x00, sizeof(keyword) );
		while( fgets( keyword, sizeof(keyword), fp ) )
		{
			trimSpace(keyword);
			if( strlen(keyword) > 0 )
			{
				strcpy( contentfilter[i], keyword );
				i++;			
				icontentfilter = 1;
			}
			memset( keyword, 0x00, sizeof(keyword) );
		}
		fclose(fp);
	}

	i = 0;
	sprintf( filepath, "%s/etc/urlfilter.conf", getenv("HOME") );
	fp = fopen( filepath, "r" );
	if( fp == NULL )
	{
		hzb_log_debug( __FILE__ , __LINE__ , "no urlfilter.conf file" );
	}
	else
	{
		hzb_log_debug( __FILE__ , __LINE__ , "get urlfilter keyword" );
		memset( keyword, 0x00, sizeof(keyword) );
		while( fgets( keyword, sizeof(keyword), fp ) )
		{
			trimSpace(keyword);
			if( strlen(keyword) > 0 )
			{
				strcpy( urlfilter[i], keyword );
				i++;			
				iurlfilter = 1;
			}
			memset( keyword, 0x00, sizeof(keyword) );
		}
		fclose(fp);
	}

	i = 0;
	sprintf( filepath, "%s/etc/contentinvolve.conf", getenv("HOME") );
	fp = fopen( filepath, "r" );
	if( fp == NULL )
	{
		hzb_log_debug( __FILE__ , __LINE__ , "no contentinvolve.conf file" );
	}
	else
	{
		hzb_log_debug( __FILE__ , __LINE__ , "get contentinvolve keyword" );
		memset( keyword, 0x00, sizeof(keyword) );
		while( fgets( keyword, sizeof(keyword), fp ) )
		{
			trimSpace(keyword);
			if( strlen(keyword) > 0 )
			{
				strcpy( contentinvolve[i], keyword );
				i++;			
				icontentinvolve = 1;
			}
			memset( keyword, 0x00, sizeof(keyword) );
		}
		fclose(fp);
	}

	i = 0;
	sprintf( filepath, "%s/etc/urlinvolve.conf", getenv("HOME") );
	fp = fopen( filepath, "r" );
	if( fp == NULL )
	{
		hzb_log_debug( __FILE__ , __LINE__ , "no urlinvolve.conf file" );
	}
	else
	{
		hzb_log_debug( __FILE__ , __LINE__ , "get urlinvolve.conf keyword" );
		memset( keyword, 0x00, sizeof(keyword) );
		while( fgets( keyword, sizeof(keyword), fp ) )
		{
			trimSpace(keyword);
			if( strlen(keyword) > 0 )
			{
				strcpy( urlinvolve[i], keyword );
				i++;			
				iurlinvolve = 1;
			}
			memset( keyword, 0x00, sizeof(keyword) );
		}
		fclose(fp);
	}

	i = 0;
	sprintf( filepath, "%s/etc/filecontentinvolve.conf", getenv("HOME") );
	fp = fopen( filepath, "r" );
	if( fp == NULL )
	{
		hzb_log_debug( __FILE__ , __LINE__ , "no filecontentinvolve.conf file" );
	}
	else
	{
		hzb_log_debug( __FILE__ , __LINE__ , "get filecontentinvolve keyword" );
		memset( keyword, 0x00, sizeof(keyword) );
		while( fgets( keyword, sizeof(keyword), fp ) )
		{
			trimSpace(keyword);
			if( strlen(keyword) > 0 )
			{
				strcpy( filecontentinvolve[i], keyword );
				i++;			
				ifilecontentinvolve = 1;
			}
			memset( keyword, 0x00, sizeof(keyword) );
		}
		fclose(fp);
	}

	i = 0;
	sprintf( filepath, "%s/etc/fileurlinvolve.conf", getenv("HOME") );
	fp = fopen( filepath, "r" );
	if( fp == NULL )
	{
		hzb_log_debug( __FILE__ , __LINE__ , "no fileurlinvolve.conf file" );
	}
	else
	{
		hzb_log_debug( __FILE__ , __LINE__ , "get fileurlinvolve.conf keyword" );
		memset( keyword, 0x00, sizeof(keyword) );
		while( fgets( keyword, sizeof(keyword), fp ) )
		{
			trimSpace(keyword);
			if( strlen(keyword) > 0 )
			{
				strcpy( fileurlinvolve[i], keyword );
				i++;			
				ifileurlinvolve = 1;
			}
			memset( keyword, 0x00, sizeof(keyword) );
		}
		fclose(fp);
	}

	sprintf( filepath, "%s/etc/filepath.conf", getenv("HOME") );
	fp = fopen( filepath, "r" );
	if( fp == NULL )
	{
		hzb_log_debug( __FILE__ , __LINE__ , "no filepath.conf file" );
	}
	else
	{
		hzb_log_debug( __FILE__ , __LINE__ , "get filepath.conf keyword" );
		memset( keyword, 0x00, sizeof(keyword) );
		fgets( keyword, sizeof(keyword), fp ); 
		trimSpace(keyword);
		if( strlen(keyword) > 0 )
		{
			memset( basepath, 0x00, sizeof(basepath) );
			strcpy( basepath, keyword );
			idownload = 1;
		}
		fclose(fp);
	}

	return ;
}


void t_open_resource()
{
	curl_global_init( CURL_GLOBAL_ALL );
	return ;
}



void close_resource()
{
	/*curl_easy_cleanup( pCurl );*/
	iconv_close(iconv_toinchar);
	return ;
}


int checkurl( char *url )
{
	char *a = "[]<>!();\r\n ";
	int i = 0;

	for( i=0; i<strlen(a); i++ )
	{
		if(strchr(url, a[i]))
		{
			hzb_log_debug( __FILE__ , __LINE__ , "checkurl error [%c]" , a[i] );
			return -1;
		}
	}
	return 0;
}



int checkfilter( char *urlcontent, char *href )
{
	int i = 0;
	char *pfound = NULL;

	for( i = 0; i < MAX_CNT; i++ )
	{
		if( strlen(contentfilter[i]) > 0 )
		{
			pfound = strstr( urlcontent, contentfilter[i] );
			if( pfound )
			{
				hzb_log_debug( __FILE__ , __LINE__ , "found content filter [%s]", contentfilter[i] );
				return 1;
			}
		}
		if( strlen(urlfilter[i]) > 0 )
		{
			pfound = strstr( href, urlfilter[i] );
			if( pfound )
			{
				hzb_log_debug( __FILE__ , __LINE__ , "found url filter [%s]", urlfilter[i] );
				return 1;
			}
		}
	}

	return 0;
}


int checkinvolve( char *urlcontent, char *href )
{
	int i = 0;
	char *pfound = NULL;

	for( i = 0; i < MAX_CNT; i++ )
	{
		if( strlen(contentinvolve[i]) > 0 )
		{
			pfound = strstr( urlcontent, contentinvolve[i] );
			if( pfound )
			{
				hzb_log_debug( __FILE__ , __LINE__ , "found content involve [%s]", contentinvolve[i] );
				return 1;
			}
		}
		if( strlen(urlinvolve[i]) > 0 )
		{

			pfound = strstr( href, urlinvolve[i] );
			if( pfound )
			{
				hzb_log_debug( __FILE__ , __LINE__ , "found url involve [%s]", urlinvolve[i] );
				return 1;
			}
		}
	}
	return 0;
}


int checkfilelist( char *urlcontent, char *href )
{
	int i = 0;
	char *pfound = NULL;

	for( i = 0; i < MAX_CNT; i++ )
	{
		if( strlen(filecontentinvolve[i]) > 0 )
		{
			pfound = strstr( urlcontent, filecontentinvolve[i] );
			if( pfound )
			{
				hzb_log_debug( __FILE__ , __LINE__ , "found filecontent involve [%s]", filecontentinvolve[i] );
				return 1;
			}
		}
		if( strlen(fileurlinvolve[i]) > 0 )
		{
			pfound = strstr( href, fileurlinvolve[i] );
			if( pfound )
			{
				hzb_log_debug( __FILE__ , __LINE__ , "found fileurl involve [%s]", fileurlinvolve[i] );
				return 1;
			}
		}
	}
	return 0;
}



int downloadhtml( char *href, int index )
{
	int nret = 0;
	char downfilepath[URL_LEN];
	char *htmldata = NULL;

	htmldata = (char *)malloc( 4096*4096 );
	if( !htmldata )
	{
		hzb_log_error( __FILE__ , __LINE__ , "GetHtmlByUrl error [%s]" , href );
		return -1 ;
	}
	memset( htmldata, 0x00, 4096*4096 );

	nret = GetHtmlByUrl( href, htmldata ) ;	
	if( nret < 0 )
	{
		free(htmldata);
		hzb_log_error( __FILE__ , __LINE__ , "GetHtmlByUrl error [%s]" , href );
		return -1 ;
	}
	else
	{
		sprintf( downfilepath, "%s/%d", basepath, index );
		WriteHtmlToFile( downfilepath, htmldata );
	}

	free(htmldata);

	return 0;
}



void *analysehtml( void *env11 )
{
	char url[URL_LEN];
	char tmpurl[URL_LEN];
	char urlcontent[URL_LEN];
	char href[URL_LEN];
	int index = 0;
	int depth = 0;
	int ifound = 0;
	int nret = 0;
	char *ptra = NULL;
	char *ptrb = NULL;
	char *ptrc = NULL;
	char *ptre = NULL;
	char *pfound = NULL;
	char htmldata[3024*2024] ;
	char cate[20];
	int i = 0;

	t_env *env_th = NULL;
	DList *t_todoListt = NULL;
	DList *t_doneListt = NULL;
	DList_stepInfo  info;

	env_th = env11;
	t_todoListt = env_th->todo_list;
	t_doneListt = env_th->done_list;
	info = *(env_th->info);

	memset(url, 0x00, sizeof(url));

	/*memset(g_caDebugLogFile, 0, sizeof(g_caDebugLogFile));
	sprintf( g_caDebugLogFile, "%s/log/spider_%d", getenv("HOME"), pthread_self() );
	g_iLogLevel = 5;*/

	/*sprintf( cate, "thread_%d", pthread_self());
        nret = hzb_log_set_category( cate );
        if( nret )
        {
                printf( "hzb_log_set_category failed[%d]\n" , nret );
                return ;
        }*/

	/* DebugLog(LOG_LEVEL_ALL, NULL, 0, "[%s.%d] tid[%d] thread analyse html start", __FILE__, __LINE__, pthread_self());*/

	hzb_log_debug( __FILE__ , __LINE__ , "tid[%d] thread analyse html start" , pthread_self() );

	while(1)
	{
		if( t_todoListt->cnt <= 0 )
			break;
		
		hzb_log_debug( __FILE__ , __LINE__ , "tid[%d] todo list cnt = [%d]", pthread_self(), t_todoListt->cnt );
		/* DebugLog(LOG_LEVEL_ALL, NULL, 0, "[%s.%d] tid[%d] todo list cnt = [%d]", __FILE__, __LINE__, pthread_self(), t_todoListt->cnt ); */

		DList_openStep( t_todoListt, &info );
		memset( url, 0x00, sizeof(url) );
		memset( urlcontent, 0x00, sizeof(urlcontent) );
		while ( DList_fetchStep( t_todoListt, &info, url, urlcontent, &depth, &index ) == 0 )
		{
			if( icontrol && depth > travel_depth )
			{
				/* DebugLog(LOG_LEVEL_ALL, NULL, 0, "[%s.%d] tid[%d] depth = [%d] > 5", __FILE__, __LINE__, pthread_self(), depth ); */
				
				hzb_log_debug( __FILE__ , __LINE__ , "tid[%d] travel depth = [%d] > [%d] " , pthread_self(), travel_depth, depth );
				DList_del( t_todoListt, url, NULL );
				break;
			}

			hzb_log_debug( __FILE__ , __LINE__ , "tid[%d] begin analyse todolist url=[%s],depth=[%d],todoindex=[%d]", pthread_self(), url, depth,index ); 
			/* DebugLog(LOG_LEVEL_ALL, NULL, 0, "[%s.%d] tid[%d] begin analyse todolist url=[%s],depth=[%d],todoindex=[%d]", __FILE__, __LINE__, pthread_self(), url, depth, index ); */

			nret = DList_add( t_doneListt, url, urlcontent, depth, NULL );
			if( nret < 0 )
			{
				if( nret == DLIST_DUP_USERDATA )
					/* DebugLog(LOG_LEVEL_ALL, NULL, 0, "[%s.%d] tid[%d] Dlist_add donelist duplicate url=[%s]", __FILE__, __LINE__, pthread_self(), url ); */
					hzb_log_debug( __FILE__ , __LINE__ , "tid[%d] Dlist_add donelist duplicate url=[%s]" , pthread_self(), url );
				else
					/* DebugLog(LOG_LEVEL_ALL, NULL, 0, "[%s.%d] tid[%d] Dlist_add donelist error  url=[%s]", __FILE__, __LINE__, pthread_self(), url ); */
					hzb_log_debug( __FILE__ , __LINE__ , "tid[%d] Dlist_add donelist error url=[%s]" , pthread_self(), url );
				DList_del( t_todoListt, (char *)url, NULL );
				break;
			}

			memset( htmldata, 0x00, sizeof(htmldata) );
			nret = t_GetHtmlByUrl( url, htmldata );
			if( nret < 0 )
			{
				hzb_log_debug( __FILE__ , __LINE__ , "tid[%d] t_GetHtmlByUrl error url=[%s]" , pthread_self(), url );
				/* DebugLog(LOG_LEVEL_ALL, NULL, 0, "[%s.%d] tid[%d] t_GetHtmlByUrl error url=[%s]", __FILE__, __LINE__, pthread_self(), url ); */
				continue;
			}

			nret = DList_del( t_todoListt, url, NULL );
			if( nret < 0 )
			{
				/* hzb_log_error( __FILE__ , __LINE__ , "DList_del error url=[%s]" , url ); */
			}

			depth++;

			while(1)
			{
				if( ptre )
					ptrb = strstr( ptre, "href=" ) ;
				else
					ptrb = strstr( htmldata, "href=" ) ;
				if( ptrb == NULL )
				{
					break;
				}

				ptre = strstr( ptrb, "</a>" ) ;
				/*ptre = strstr( ptrb, ">" ) ;*/
				if( ptre == NULL )
				{
					/* hzb_log_debug( __FILE__ , __LINE__ , "not found [ ]"); */
					break;
				}

				memset( tmpurl, 0x00, sizeof(tmpurl) ) ;
				/* url too long */
				if( ptre - ptrb > URL_LEN )
				{
					memcpy( tmpurl, ptrb+5, URL_LEN-1 ) ;  
					/* DebugLog(LOG_LEVEL_ALL, NULL, 0, "[%s.%d] tid[%d] url too long [%s]", __FILE__, __LINE__, pthread_self(), tmpurl ); */
					continue;
				}
				else
					memcpy( tmpurl, ptrb+5, ptre - ptrb ) ;  

				memset( urlcontent, 0x00, sizeof(urlcontent) );
				memset( href, 0x00, sizeof(href) );
				nret = get_href( tmpurl, seedurl, href, urlcontent, url );
				
				if( strlen(href) <= 0 )	
					continue;


				hzb_log_debug( __FILE__ , __LINE__ , "tid[%d] href[%s] urlcontent[%s] " , pthread_self(), href, urlcontent );
				/* DebugLog(LOG_LEVEL_ALL, NULL, 0, "[%s.%d] tid[%d] href[%s] urlcontent[%s]", __FILE__, __LINE__, pthread_self(), href, urlcontent ); */

				/*if( ( (icontentfilter || iurlfilter) && checkfilter(urlcontent, href) ) ||
					( !icontentfilter && !iurlfilter )
				)*/
				{
					nret = DList_add( t_todoListt, href, urlcontent, depth, checkurl );
					if( nret )
						hzb_log_error( __FILE__ , __LINE__ , "tid[%d] add todolist duplicate [%s]" , pthread_self(), href ); 
						/* DebugLog(LOG_LEVEL_ERROR, NULL, 0, "[%s.%d] tid[%d] add todolist duplicate [%s]", __FILE__, __LINE__, pthread_self(), href ); */
					else
						hzb_log_debug( __FILE__ , __LINE__ , "tid[%d] add todolist [%s]" , pthread_self(), href );
						/* DebugLog(LOG_LEVEL_ALL, NULL, 0, "[%s.%d] tid[%d] add todolist url=[%s]", __FILE__, __LINE__, pthread_self(), href ); */
						
				}
			}
			ptre = NULL;
			ptrb = NULL;
			/* DebugLog(LOG_LEVEL_ALL, NULL, 0, "[%s.%d] tid[%d] end analyse todolist url=[%s], todoindex=[%d]", __FILE__, __LINE__, pthread_self(), url, index ); */
			hzb_log_debug( __FILE__ , __LINE__ , "tid[%d] end analyse todolist url=[%s], todoindex=[%d]" , pthread_self(), url, index );

			depth = 0;
			memset( url, 0x00, sizeof(url) );
			memset( urlcontent, 0x00, sizeof(urlcontent) );

		}
		DList_closeStep( t_todoListt,&info );
	}

	depth = 0;
	memset( url, 0x00, sizeof(url) );
	memset( urlcontent, 0x00, sizeof(urlcontent) );
	DList_openStep( t_doneListt, &info );
	while ( DList_fetchStep( t_doneListt, &info, url, urlcontent, &depth, &index ) == 0 )
	{
		/* DebugLog(LOG_LEVEL_ALL, NULL, 0, "[%s.%d] tid[%d] donelist url=[%s], depth=[%d] todoindex=[%d]", __FILE__, __LINE__, pthread_self(), url, depth, index ); */
		hzb_log_debug( __FILE__ , __LINE__ , "tid[%d] donelist url=[%s], content=[%s], depth=[%d] todoindex=[%d]", pthread_self(), url, urlcontent, depth, index );
		memset( url, 0x00, sizeof(url) );
		memset( urlcontent, 0x00, sizeof(urlcontent) );
	}
	DList_closeStep( t_doneListt, &info );
	/* DebugLog(LOG_LEVEL_ALL, NULL, 0, "[%s.%d] tid[%d]  complete [%d]", __FILE__, __LINE__, pthread_self(), t_doneListt->cnt ); */
	hzb_log_debug( __FILE__ , __LINE__ , "tid[%d]  complete [%d]", pthread_self(), t_doneListt->cnt );

	/* DebugLog(LOG_LEVEL_ALL, NULL, 0, "[%s.%d] tid[%d] thread analyse html end", __FILE__, __LINE__, pthread_self() ); */
	hzb_log_debug( __FILE__ , __LINE__ , "tid[%d] thread analyse html end ", pthread_self() );

	return ;
}




void get_shixin_pagecondition(char *seedurl)
{
	char filepath[URL_LEN];
	char etcpage[10];
	FILE *fd = NULL;
	char clt[20];
	char cgt[20];
	
	if( strstr(seedurl, "shixin.court.gov.cn") )
	{
	
		sprintf( filepath, "%s/etc/shixin.conf", getenv("HOME") );	
		if( !access( filepath, F_OK ) )
		{
			fd = fopen( filepath, "r" );
			if( fd == NULL )
			{
				hzb_log_error( __FILE__ , __LINE__ , "fd is null");
				return ;
			}
			memset( etcpage, 0x00, sizeof(etcpage) );
			while( fgets( etcpage, sizeof(etcpage), fd ) )
			{
				if( etcpage[0] == '<' )	
				{
					llt = atol(&etcpage[1]);
					hzb_log_debug( __FILE__ , __LINE__ , "condition llt=[%ld]", llt );
				}
				if( etcpage[0] == '>' )	
				{
					lgt = atol(&etcpage[1]);
					hzb_log_debug( __FILE__ , __LINE__ , "condition lgt=[%ld]", lgt );
				}
				memset( etcpage, 0x00, sizeof(etcpage) );
			}
			fclose(fd);
		}
		else
		{
			if( getenv("LT_CONDITION") )
			{
				sprintf( clt, "%s", getenv("LT_CONDITION") );	
				llt = atol(clt);
				hzb_log_debug( __FILE__ , __LINE__ , "condition llt=[%ld]", llt );
			}
			if( getenv("GT_CONDITION") )
			{
				sprintf( cgt, "%s", getenv("GT_CONDITION") );
				lgt = atol(cgt);
				hzb_log_debug( __FILE__ , __LINE__ , "condition lgt=[%ld]", lgt );
			}
		}
	}
	return ;
}


