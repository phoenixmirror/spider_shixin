#define _MAIN_PROGRAM
#include "common.h"
int giQidOut=0;

void usage() 
{       

#ifdef	COMMON
	printf( "USAGE:  spider seedurl traveldepth \n" );
#endif

#ifdef	MULTI_PROC	
	printf( "USAGE:  spider seedurl traveldepth proc_num \n" );
#endif

	return ;
}       
        
        
int main( int argc , char *argv[] )
{
	int     pid=0;
	int	num=0;	
	int	i=0;
	int	j=0;
	int	status=0;
	int	nret=0;
	int	iflag=0;
	int	iLen=0;
	int	icnt=0;
	char	seedurl[URL_LEN];
	char	url[URL_LEN];
	char	logfile[URL_LEN];
	stDefMsgBuf	stMsgBuf;
	stDefurldata	*psturldata=NULL;
	char	*donelist[URL_NUM]={0};
	stDefSharedMemory	stmem;
	stDefSharedMemory	stmemlist[10];
	int	ipclist[10]={111,222,333,444,555,666,777,888,999,1999};
	int	progress = 0;
	int	totalcnt = 0;

#ifdef	COMMON
        if( argc < 3 )
        {
                usage();
                exit(0);
        }
	argv++; argc--;
	spider(argc, argv);
#endif

#ifdef MULTI_PROC

	char ipckey[100];
	if(getenv("IPCKEY") != NULL)
		sprintf(ipckey,"%s", getenv("IPCKEY"));
	else
		sprintf(ipckey, "%s", "111");

	char catotalcnt[100];
	if(getenv("TOTALCNT") != NULL)
		sprintf(catotalcnt,"%s", getenv("TOTALCNT"));
	else
		sprintf(catotalcnt, "%s", "150000");


	if( argc == 2 )
	{
		if( strcmp(argv[1], "view") == 0 )
		{
			sprintf( logfile, "spider_view" );
			nret = hzb_log_set_category( logfile );
			if( nret )
			{
				printf( "hzb_log_set_category failed[%d]\n" , nret );
				return -1;
			}

			memset((char*)&stmem, 0, sizeof(stmem));
			nret = OpenSharedMem(&stmem, atol(ipckey), MAX_SHAREMEM_LEN);
			if (nret != SUCC) 
			{
				hzb_log_error( __FILE__ , __LINE__ , " get sharemem error r=%d" , nret );
				return nret;
			}

			for(i=0;i<URL_NUM;i++)
				donelist[i] = stmem.pcData + i*BUFF_LEN;
			
			for(i=0;i<URL_NUM;i++)
			{
				if(strlen(donelist[i])>0)
					hzb_log_debug( __FILE__ , __LINE__ , "sharemem [%d] url=[%s]", i, donelist[i]);
			}

			CloseSharedMem(&stmem);
			exit(0);
		}
		if( strcmp(argv[1], "clear") == 0 )
		{
			memset((char*)&stmem, 0, sizeof(stmem));
			nret = OpenSharedMem(&stmem, atol(ipckey), MAX_SHAREMEM_LEN);
			if (nret != SUCC) 
			{
				hzb_log_error( __FILE__ , __LINE__ , " get sharemem error r=%d" , nret );
				return nret;
			}

			memset( stmem.pcData, 0x00, MAX_SHAREMEM_LEN );

			CloseSharedMem(&stmem);
			exit(0);
		}
	}

        if( argc < 4 )
        {
                usage();
                exit(0);
        }

	num=atoi(argv[3]);
	if( num <= 0 )	
	{
		printf( " proc num need > 0 \n" );
		exit(0);
	}

        pid=fork();
        switch( pid )
        {
                case -1:
                        return -1;
                case 0:
                        break;
                default :
                        exit( 0 );
        }

        signal(SIGHUP,  SIG_IGN);
        signal(SIGINT,  SIG_IGN);
        signal(SIGIO,   SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
        signal(SIGTERM, SIG_IGN);
        signal(SIGPIPE, SIG_IGN);

        setsid();

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

	OpenUnixMsgQ(atol(ipckey));
	ClearQueue(giQidOut, QUEUE_DEFAULT_QUEUETYPE);
	argv++; argc--;
	memset( seedurl, 0x00, sizeof(seedurl) );
        strcpy( seedurl, argv[0] );

	memset((char*)&stmem, 0, sizeof(stmem));
	nret = OpenSharedMem(&stmem, atol(ipckey), MAX_SHAREMEM_LEN);
	if (nret != SUCC) 
	{
		hzb_log_error( __FILE__ , __LINE__ , " get sharemem error r=%d" , nret );
		return nret;
	}
	for(i=0;i<URL_NUM;i++)
		donelist[i] = stmem.pcData + i*BUFF_LEN;
	for(i=0;i<URL_NUM;i++)
	{
		if(strlen(donelist[i])<=0)
			continue;
		hzb_log_debug( __FILE__ , __LINE__ , "last complete [%d] url=[%s]", i, donelist[i]);
		icnt++;
	}

	if(icnt > 0)
	{
		if(icnt > 100)
		{
			hzb_log_debug( __FILE__ , __LINE__ , "last complete [%d] donelist[0]=[%s]",icnt,donelist[0]);
			if( strstr(donelist[0], "shixin.court.gov.cn"))
			{
				hzb_log_debug( __FILE__ , __LINE__ , "last complete involve seed, continue" );
				iflag = 0;
				for(i=1;i<=100;i++)
				{
					if(strlen(donelist[icnt-i]) <= 0)
						continue;
					memset( &stMsgBuf, 0x00, sizeof(stMsgBuf) );
					stMsgBuf.stMsgHead.lQTypeSend = QUEUE_DEFAULT_QUEUETYPE;
					psturldata = (stDefurldata *)&stMsgBuf.ucaText;
					strcpy( psturldata->url, donelist[icnt-i] );
					hzb_log_debug( __FILE__ , __LINE__ , "send last spide url [%d] [%s] ",icnt-i,donelist[icnt-i] );
					psturldata->depth = 1;
					psturldata->analyse_cnt = 0;
					SendUnixMsgQ(giQidOut, &stMsgBuf, sizeof(stDefInternalMsgHead)+sizeof(stDefurldata));
					memset( donelist[icnt-i], 0x00, BUFF_LEN );
				}
			}
			else
			{
				hzb_log_debug( __FILE__ , __LINE__ , "last complete not involve seed, restart" );
				iflag = 1;
				memset( stmem.pcData, 0x00, MAX_SHAREMEM_LEN );
			}
		}
		else
		{
			hzb_log_debug( __FILE__ , __LINE__ , "last spide complete not over 100, restart" );
			iflag = 1;
			memset( stmem.pcData, 0x00, MAX_SHAREMEM_LEN );
		}
	}
	else
	{
		hzb_log_debug( __FILE__ , __LINE__ , "no last record, start" );
		iflag = 1;
		memset( stmem.pcData, 0x00, MAX_SHAREMEM_LEN );
	}

	/* check if not complete */
	/*memset( &stMsgBuf, 0x00, sizeof(stMsgBuf) );
	nret = IntervalRecvUnixMsgQ(giQidOut, &stMsgBuf, QUEUE_DEFAULT_QUEUETYPE, 1, &iLen);
	if (nret == SUCC)
	{
		psturldata = (stDefurldata *)&stMsgBuf.ucaText;
        	if( strstr(psturldata->url, seedurl) )
			iflag = 0;
		else
		{
			iflag = 1;
			ClearQueue(giQidOut, QUEUE_DEFAULT_QUEUETYPE);
		}
	}
	else
		iflag = 1;*/

	if( iflag )
	{
		if( strstr(seedurl, "shixin.court.gov.cn") )
		{
			hzb_log_debug( __FILE__ , __LINE__ , "argv[3]=%s" , argv[3] );
			if( !argv[3] )
				sprintf( url, "%s/unitMore.do", seedurl );
			else
			{
				hzb_log_debug( __FILE__ , __LINE__ , "222argv[3]=%s" , argv[3] );
				if(strcmp(argv[3],"unit") == 0)
					sprintf( url, "%s/unitMore.do", seedurl );
				else
					sprintf( url, "%s/personMore.do", seedurl );
			}
			/* sprintf( url, "%s/personMore.do", seedurl );*/
			memset( &stMsgBuf, 0x00, sizeof(stMsgBuf) );
			stMsgBuf.stMsgHead.lQTypeSend = QUEUE_DEFAULT_QUEUETYPE;
			psturldata = (stDefurldata *)&stMsgBuf.ucaText;
			strcpy( psturldata->url, url );
			psturldata->depth = 1;
			psturldata->analyse_cnt = 0;
			SendUnixMsgQ(giQidOut, &stMsgBuf, sizeof(stDefInternalMsgHead)+sizeof(stDefurldata));
			hzb_log_debug( __FILE__ , __LINE__ , "send msgq" );
		}
		else	
		{
			memset( &stMsgBuf, 0x00, sizeof(stMsgBuf) );
			psturldata = (stDefurldata *)&stMsgBuf.ucaText;
			stMsgBuf.stMsgHead.lQTypeSend = QUEUE_DEFAULT_QUEUETYPE;
			strcpy( psturldata->url, seedurl );
			psturldata->depth = 1;
			psturldata->analyse_cnt = 0;
			SendUnixMsgQ(giQidOut, &stMsgBuf, sizeof(stDefInternalMsgHead)+sizeof(stDefurldata));
			hzb_log_debug( __FILE__ , __LINE__ , "send msgq" );
		}
	}

	for(i=0;i<num;i++)
	{
		pid=fork();
		if( pid < 0 )
			hzb_log_error( __FILE__ , __LINE__ , "fork error [%d]", pid );
		else if( pid == 0 )
		{
			spider(argc, argv,i);
			exit(0);
		}
	}

	pid=fork();
	if( pid == 0 )
	{
		
		memset((char*)&stmem, 0, sizeof(stmem));
		nret = OpenSharedMem(&stmem, atol(ipckey), MAX_SHAREMEM_LEN);
		if (nret != SUCC) 
		{
			hzb_log_error( __FILE__ , __LINE__ , " get sharemem error r=%d" , nret );
			return nret;
		}

		for(i=0;i<URL_NUM;i++)
			donelist[i] = stmem.pcData + i*BUFF_LEN;
		
		for(i=0;i<URL_NUM;i++)
		{
			if(strlen(donelist[i])>0)
				hzb_log_debug( __FILE__ , __LINE__ , "sharemem [%d] url=[%s]", i, donelist[i]);
		}

		CloseSharedMem(&stmem);
		exit(0);
	}

	hzb_log_debug( __FILE__ , __LINE__ , "waiting spider end" );
	icnt=0;


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


	for(j=0;j<10;j++)
	{
		memset((char*)&stmemlist[j], 0, sizeof(stmemlist[j]));
		nret = OpenSharedMem(&stmemlist[j], ipclist[j], MAX_SHAREMEM_LEN);
		if (nret != SUCC) 
		{
			hzb_log_error( __FILE__ , __LINE__ , " get sharemem error r=%d" , nret );
			continue;
		}
	}


	while( num >= 0 )	
	{
		while( ( pid = waitpid( -1, &status, WNOHANG ) ) > 0 )
		{
			num--;
		}

		hzb_log_debug( __FILE__ , __LINE__ , "num=%d" , num );
								
		for(j=0;j<10;j++)
		{
			for(i=0;i<URL_NUM;i++)
				donelist[i] = stmemlist[j].pcData + i*BUFF_LEN;
			
			for(i=0;i<URL_NUM;i++)
			{
				if(strlen(donelist[i])>0)
					icnt++;
			}
			
			hzb_log_debug( __FILE__ , __LINE__ , "total complete=[%d]", icnt );
		}
		totalcnt = atoi(catotalcnt);
		
		hzb_log_debug( __FILE__ , __LINE__ , "r=%lf" , (double)icnt/(double)totalcnt);
		progress=((double)icnt/(double)totalcnt)*100;
		hzb_log_debug( __FILE__ , __LINE__ , "complete percent=[%d]", progress );
		if(progress != 0 && progress <= 100)
			RefreshProgress(progress);
		if(progress>100)
			RefreshProgress(100);
		icnt=0;
		sleep (30);
	}

	DSCDBDISCONN();

	memset((char*)&stmem, 0, sizeof(stmem));
	nret = OpenSharedMem(&stmem, atol(ipckey), MAX_SHAREMEM_LEN);
	if (nret != SUCC) 
	{
		hzb_log_error( __FILE__ , __LINE__ , " get sharemem error r=%d" , nret );
		return nret;
	}
	
	for(i=0;i<URL_NUM;i++)
		donelist[i] = stmem.pcData + i*BUFF_LEN;
	for(i=0;i<URL_NUM;i++)
	{
		if(strlen(donelist[i])>0)
			hzb_log_debug( __FILE__ , __LINE__ , "sharemem [%d] url=[%s]", i, donelist[i]);
	}
	
	/*memset( stmem.pcData, 0x00, MAX_SHAREMEM_LEN );*/
	CloseSharedMem(&stmem);

	hzb_log_debug( __FILE__ , __LINE__ , "spider end" );
#endif

        return 0;
}



