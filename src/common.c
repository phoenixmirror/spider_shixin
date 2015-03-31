#include "common.h"

xmlChar *findDefaultNamespaceHref( const xmlDocPtr doc )
{       
        xmlNodePtr pXmlNode = NULL;
        if( doc == NULL )
                return(NULL);
        pXmlNode = xmlDocGetRootElement(doc);
        
        return((xmlChar *)pXmlNode->ns->href);
}

void getDataByXpath( xmlXPathContextPtr xpathCtx, xmlChar * xmlPath, char* pcDataBuf, int iBufSize )
{       
        xmlXPathObjectPtr xpathObj;
        int size = 0;
        
        xpathObj = xmlXPathEvalExpression(xmlPath, xpathCtx);
        if( xmlXPathNodeSetIsEmpty(xpathObj->nodesetval) )
        {       
                hzb_log_info( __FILE__ , __LINE__ , "xpath=[%s],获取节点信息为空", xmlPath );
                xmlXPathFreeObject(xpathObj);
                return;
        }
        size = (xpathObj != NULL && xpathObj->nodesetval) ? xpathObj->nodesetval->nodeNr : 0;
        if (size > 0)
        {       
                if( xpathObj->nodesetval->nodeTab[0]->children != NULL)
                        strcpy( pcDataBuf, (char*)xpathObj->nodesetval->nodeTab[0]->children->content );
        }
        xmlXPathFreeObject(xpathObj);
        return;
}


int check_num( char *number )
{       
        int     i = 0; 
        int     inum = 0;
        int     irc = 0;
        if( strlen(number) <= 0 )
        {       
                hzb_log_error( __FILE__ , __LINE__ , "字符串长度小于0" );
                return  -1;
        }
        
        for( i = 0; i < strlen(number); i++ )
        {
                irc = isdigit(number[i]);
                if( irc == 0 )
                {
                        hzb_log_error( __FILE__ , __LINE__ , "[%s],字符串中含有非数字", number ); 
                        return  -1;
                }
        }
        inum = atoi(number);
        return inum;
}


/*int get_service_cfg( char *cfg, stDefsercfg *pstsercfg )
{
        int     irc = 0;
        xmlChar *ns_DefNamespce = NULL;
        xmlDocPtr       doc;
        xmlXPathContextPtr      xpathCtx;
        struct stat filestat;
        char    filename[256];
        char    buffer[1024];

        hzb_log_info( __FILE__ , __LINE__ , "parse cfg xml [%s]", cfg );
        memset( filename, 0x00, sizeof(filename) );
        memset( buffer, 0x00, sizeof(buffer) );
        memset( (char*)&filestat, 0x00, sizeof(struct stat) );

        sprintf( filename, "%s/etc/%s", (char*)(getenv("HOME")), cfg );
        irc = stat( filename, &filestat );
        if( irc != 0 )
        {
                hzb_log_error( __FILE__ , __LINE__ , "[%s] file is not exsit", filename );
                return  -1;
        }

        doc = xmlParseFile( filename );
        if( doc == NULL )
        {
                hzb_log_error( __FILE__ , __LINE__ , "unable to parse [%s]", filename );
                return  -1;
        }

        xpathCtx = xmlXPathNewContext( doc );
        if( xpathCtx == NULL )
        {
                hzb_log_error( __FILE__ , __LINE__ , "unable to create new XPath context" );
                return  -1;
        }

        ns_DefNamespce = findDefaultNamespaceHref( doc );
        if (ns_DefNamespce != NULL)
        {
                if( xmlXPathRegisterNs(xpathCtx, BAD_CAST "c", ns_DefNamespce) != 0 )
                {
                        hzb_log_error( __FILE__ , __LINE__ , "unable to create new XPath context" );
                        return  -1;
                }
        }

        memset( buffer, 0x00, sizeof(buffer) );
        getDataByXpath(xpathCtx, BAD_CAST "/c:hzbank/c:ttm/c:service_name", buffer, sizeof(buffer) - 1);
        if( strlen(buffer) > 0 )
        {
                strcpy( pstsercfg->service_name, buffer );
                hzb_log_info( __FILE__ , __LINE__ , "service_name=[%s]", pstsercfg->service_name );
        }
        else
        {
                printf("service_name不能为空\n");
                return -1;
        }

        memset( buffer, 0x00, sizeof(buffer) );
        getDataByXpath(xpathCtx, BAD_CAST "/c:hzbank/c:ttm/c:so_name", buffer, sizeof(buffer) - 1);
        if( strlen(buffer) > 0 )
        {
                strcpy( pstsercfg->so_name, buffer );
                hzb_log_info( __FILE__ , __LINE__ , "so_name=[%s]", pstsercfg->so_name );
        }
        else
        {
                printf("so_name不能为空\n");
                return -1;
	}

        memset( buffer, 0x00, sizeof(buffer) );
        getDataByXpath(xpathCtx, BAD_CAST "/c:hzbank/c:ttm/c:service_func", buffer, sizeof(buffer) - 1);
        if( strlen(buffer) > 0 )
        {
                strcpy( pstsercfg->service_func, buffer );
                hzb_log_info( __FILE__ , __LINE__ , "service_func=[%s]", pstsercfg->service_func );
        }
        else
        {
                printf("service_func不能为空\n");
                return -1;
        }

        memset( buffer, 0x00, sizeof(buffer) );
        getDataByXpath(xpathCtx, BAD_CAST "/c:hzbank/c:ttm/c:arguments", buffer, sizeof(buffer) - 1);
        if( strlen(buffer) > 0 )
        {
                strcpy( pstsercfg->arguments, buffer );
                hzb_log_info( __FILE__ , __LINE__ , "arguments=[%s]", pstsercfg->arguments );
        }

        memset( buffer, 0x00, sizeof(buffer) );
        getDataByXpath(xpathCtx, BAD_CAST "/c:hzbank/c:ttm/c:trigger_interval", buffer, sizeof(buffer) - 1);
        if( strlen(buffer) > 0 )
        {
                if( strcmp( buffer, TRIGGER_INTERVAL_DAY ) != 0 &&
                        strcmp( buffer, TRIGGER_INTERVAL_WEEK ) != 0 )
                {
                        hzb_log_error( __FILE__ , __LINE__ , "trigger_interval值不符合规定[%s]", buffer );
                        printf("trigger_interval值不符合规定[%s]", buffer);
                        return  -1;
                }
                strcpy( pstsercfg->trigger_interval, buffer );
                hzb_log_info( __FILE__ , __LINE__ , "trigger_interval=[%s]", pstsercfg->trigger_interval );
        }

        memset( buffer, 0x00, sizeof(buffer) );
        getDataByXpath(xpathCtx, BAD_CAST "/c:hzbank/c:ttm/c:trigger_interval_value", buffer, sizeof(buffer) - 1);
        if( strlen(buffer) > 0 )
        {
                strcpy( pstsercfg->trigger_interval_value, buffer );
                hzb_log_info( __FILE__ , __LINE__ , "trigger_interval_value=[%s]", pstsercfg->trigger_interval_value );
        }
        else
        {
                if( strlen( pstsercfg->trigger_interval ) > 0 )
                {
                        hzb_log_error( __FILE__ , __LINE__ , "trigger_interval有值 trigger_interval_value不能为空" );
                        printf("trigger_interval有值 trigger_interval_value不能为空");
                        return  -1;
                }
        }

        memset( buffer, 0x00, sizeof(buffer) );
        getDataByXpath(xpathCtx, BAD_CAST "/c:hzbank/c:ttm/c:trigger_type", buffer, sizeof(buffer) - 1);
        if( strlen(buffer) > 0 )
        {
                if( strcmp( buffer, TRIGGER_TYPE_TIME ) != 0  &&
                        strcmp( buffer, TRIGGER_TYPE_CLOCK ) != 0 )
                {
                        hzb_log_error( __FILE__ , __LINE__ , "trigger_type值不符合规定[%s]", buffer );
                        printf("trigger_type值不符合规定[%s]", buffer);
                        return  -1;
                }
                strcpy( pstsercfg->trigger_type, buffer );
                hzb_log_info( __FILE__ , __LINE__ , "trigger_type=[%s]", pstsercfg->trigger_type );
        }

        memset( buffer, 0x00, sizeof(buffer) );
        getDataByXpath(xpathCtx, BAD_CAST "/c:hzbank/c:ttm/c:trigger_start_time", buffer, sizeof(buffer) - 1);
        if( strlen(buffer) > 0 )
        {
                strcpy( pstsercfg->trigger_start_time, buffer );
                hzb_log_info( __FILE__ , __LINE__ , "trigger_start_time=[%s]", pstsercfg->trigger_start_time );
        }
        else
        {
                if( strlen( pstsercfg->trigger_type ) > 0 )
                {
                        hzb_log_error( __FILE__ , __LINE__ , "trigger_type有值 trigger_start_time不能为空" );
		                       printf("trigger_type有值, trigger_start_time不能为空");
                        return  -1;
                }
        }

        memset( buffer, 0x00, sizeof(buffer) );
        getDataByXpath(xpathCtx, BAD_CAST "/c:hzbank/c:ttm/c:trigger_end_time", buffer, sizeof(buffer) - 1);
        if( strlen(buffer) > 0 )
        {
                strcpy( pstsercfg->trigger_end_time, buffer );
                hzb_log_info( __FILE__ , __LINE__ , "trigger_end_time=[%s]", pstsercfg->trigger_end_time );
        }
        else
        {
                if( strcmp( pstsercfg->trigger_type, TRIGGER_TYPE_TIME ) == 0 )
                {
                        hzb_log_error( __FILE__ , __LINE__ , "trigger_type值是时间区间 trigger_end_time不能为空");
                        printf("trigger_type值是时间区间 trigger_end_time不能为空");
                        return  -1;
                }
        }

        memset( buffer, 0x00, sizeof(buffer) );
        getDataByXpath(xpathCtx, BAD_CAST "/c:hzbank/c:ttm/c:hostname", buffer, sizeof(buffer) - 1);
        if( strlen(buffer) > 0 )
        {
                strcpy( pstsercfg->hostname, buffer );
                hzb_log_info( __FILE__ , __LINE__ , "hostname=[%s]", pstsercfg->hostname );
        }
        else
        {
                printf("主机名不能为空\n");
                return  -1;
        }

        memset( buffer, 0x00, sizeof(buffer) );
        getDataByXpath(xpathCtx, BAD_CAST "/c:hzbank/c:ttm/c:service_number", buffer, sizeof(buffer) - 1);
        irc = check_num(buffer);
        if( irc <= 0 )
        {

                printf("服务个数不能小于0\n");
                return  -1;
        }
        else
        {
                pstsercfg->service_number = irc;
                hzb_log_info( __FILE__ , __LINE__ , "service_number=[%d]", pstsercfg->service_number );
        }

        memset( buffer, 0x00, sizeof(buffer) );
        getDataByXpath(xpathCtx, BAD_CAST "/c:hzbank/c:ttm/c:interval", buffer, sizeof(buffer) - 1);
        irc = check_num(buffer);
        if( irc <= 0 )
        {
                printf("服务运行间隔时间不能小于0\n");
                return  -1;
        }
        else
        {
                pstsercfg->interval = irc;
                hzb_log_info( __FILE__ , __LINE__ , "interval=[%d]", pstsercfg->interval );
        }

        memset( buffer, 0x00, sizeof(buffer) );
        getDataByXpath(xpathCtx, BAD_CAST "/c:hzbank/c:ttm/c:timeout", buffer, sizeof(buffer) - 1);
        irc = check_num(buffer);
        if( irc <= 0 )
        {
                printf("服务运行超时时间不能小于0\n");
                return  -1;
        }
        else
        {
                pstsercfg->timeout = irc;
                hzb_log_info( __FILE__ , __LINE__ , "timeout=[%d]", pstsercfg->timeout );
        }

        memset( buffer, 0x00, sizeof(buffer) );
        getDataByXpath(xpathCtx, BAD_CAST "/c:hzbank/c:ttm/c:max_cnt", buffer, sizeof(buffer) - 1);
        irc = check_num(buffer);
	if( irc <= 0 )
        {
                printf("服务运行超时时间不能小于0\n");
                return  -1;
        }
        else
        {
                pstsercfg->timeout = irc;
                hzb_log_info( __FILE__ , __LINE__ , "timeout=[%d]", pstsercfg->timeout );
        }

        memset( buffer, 0x00, sizeof(buffer) );
        getDataByXpath(xpathCtx, BAD_CAST "/c:hzbank/c:ttm/c:max_cnt", buffer, sizeof(buffer) - 1);
        irc = check_num(buffer);
        if( irc <= 0 )
        {
                printf("服务运行最大次数不能小于0\n");
                return  -1;
        }
        else
        {
                pstsercfg->max_cnt = irc;
                hzb_log_info( __FILE__ , __LINE__ , "max_cnt=[%d]", pstsercfg->max_cnt );
        }

        xmlXPathFreeContext( xpathCtx );
        xmlFreeDoc( doc );
        xmlCleanupParser();
        return  0;
}
*/




void trimTailSpace(char *pcData)
{
        int iLen;

        for(iLen=strlen(pcData)-1;iLen>=0;iLen--)
        {
                if(pcData[iLen]!=' ' && pcData[iLen]!='\0' && pcData[iLen]!='\n' && pcData[iLen]!='\r' && pcData[iLen]!='\t')
                {
                        break;
                }
                else
                {
                        pcData[iLen] = '\0';
                }
        }
}


void trimNewLine(char *pcData)
{
        int iLen;

        for(iLen=strlen(pcData)-1;iLen>=0;iLen--)
        {
                if(pcData[iLen]!='\0' && pcData[iLen]!='\n' && pcData[iLen]!='\r' && pcData[iLen]!='\t')
                {
                        break;
                }
                else
                {
                        pcData[iLen] = '\0';
                }
        }
}


void trimHeaderSpace(char *pcData)
{
        int iLen,i=0,iBufLen=0;
        char *pcBuf = NULL;

        iLen=strlen(pcData);

        if(iLen > 0)
        {
                pcBuf = (char *)malloc(iLen);
                memset(pcBuf, 0x00, iLen);

                for(i=0;i < iLen ;i++)
                {
                        if(pcData[i] !=' ' && pcData[i]!='\0' && pcData[i]!='\n' && pcData[i]!='\r' && pcData[i]!='\t')
                        {
                                iBufLen=i;
                                break;
                        }
                }

                if (iBufLen > 0)
                {
                        memcpy(pcBuf,&pcData[iBufLen],iLen-iBufLen);
                        memset(pcData,0x00,iLen);
                        memcpy(pcData,pcBuf,iLen-iBufLen);
                }

                free(pcBuf);
        }
}


void trimSpace( char *pcData )
{
        trimHeaderSpace(pcData);
        trimTailSpace(pcData);
}


static int sg_iTimeoutFlag = 0;
extern int giQidOut;

static void TimeOutHandle(int signo)
{
  sg_iTimeoutFlag = 1;
}

int OpenUnixMsgQ(long ipckey)
{
	key_t QUEUE_KEY_OUT;
	if (giQidOut != 0 )
		return(SUCC);

	/* 往帐队列 */
	QUEUE_KEY_OUT = ftok(BASE_DIR, ipckey);
	if ( ( giQidOut = msgget( QUEUE_KEY_OUT, 0666|IPC_CREAT ) ) < 0 ) 
	{
		hzb_log_debug( __FILE__ , __LINE__ , "msgget() error %d(%s)" , errno, strerror(errno));
		return -1;
	}
	hzb_log_debug( __FILE__ , __LINE__ , "SYSTEM QUEUE OPENED lQKey=%ld iQCtlIdx=%d BASE_DIR=%s", QUEUE_KEY_OUT, giQidOut, BASE_DIR);
	return (SUCC);
}

void QueueUnixRmId(void)
{
	struct msqid_ds stMsgCtl;

	msgctl( giQidOut, IPC_RMID, &stMsgCtl );
	giQidOut = 0;
	return;
}

int SendUnixMsgQ(int iQidOut, stDefMsgBuf *pstMsgBuf,int iLen)
{
	int nret=0;
	hzb_log_debug( __FILE__ , __LINE__ , "SendUnixMsgQ start" );

	if ( ( nret = msgsnd(iQidOut, (char*)pstMsgBuf, iLen, 0) ) < 0 ) 
	{	
		hzb_log_error( __FILE__ , __LINE__ , " SendUnixMsgQ() error QIdx=%d, QType=%lu Len=%d Rc=%d " , 
		iQidOut, pstMsgBuf->stMsgHead.lQTypeSend, iLen, nret );
		hzb_log_debug( __FILE__ , __LINE__ , "msgsnd error %d (%s)", errno, strerror(errno) );
		switch(errno) {
			case ENOMEM:
			case EAGAIN:
			return -1;
		}
		return -1;
	}

	hzb_log_debug( __FILE__ , __LINE__ , "msgsnd Len=[%.6d] Type=[%lu] QId=[%d]",iLen, pstMsgBuf->stMsgHead.lQTypeSend, iQidOut );

	return(SUCC);
}


int IntervalRecvUnixMsgQ(int iQIdIn, stDefMsgBuf *pstMsgBuf,long lQType,long lWaitInterval, int *piLen)
{

	hzb_log_debug( __FILE__ , __LINE__ , "IntervalRecvUnixMsgQ start" );

	memset((char*)pstMsgBuf, 0, sizeof(stDefMsgBuf));
	signal(SIGALRM,SIG_IGN);
	signal(SIGALRM,TimeOutHandle);
	alarm(lWaitInterval);
	if ( ( *piLen = msgrcv(iQIdIn, (char*)pstMsgBuf, sizeof(stDefMsgBuf), lQType, 0) ) < 0 ) 
	{
		signal(SIGALRM,SIG_IGN);
		alarm(0);
		if ( ! sg_iTimeoutFlag ) 
		{
			hzb_log_debug( __FILE__ , __LINE__ , "msgrcv(interval) error %d (%s)", errno, strerror(errno));
			sg_iTimeoutFlag = 0;
			if (errno == 4)
				return(QUEFN_RECV_TIMEOUT_RC);
			else
				return -1;
		}
		else 
		{
			sg_iTimeoutFlag = 0;
			return (QUEFN_RECV_TIMEOUT_RC);
		}
	}
	signal(SIGALRM,SIG_IGN);
	alarm(0);

	hzb_log_debug( __FILE__ , __LINE__ , "IntervalRecvUnixMsgQ() Len=[%.6d] Type=[%lu] QId=[%d]", *piLen, lQType, iQIdIn );
	return(SUCC);
}

void ClearQueue(int iQId, long lQueueType)
{
	stDefMsgBuf stMsgBuf;
	int nret = 0, iLen;

	memset((char*)&stMsgBuf, 0, sizeof(stDefMsgBuf));
	while (nret >= 0) 
	{
		nret = msgrcv(iQId, (char*)&stMsgBuf, sizeof(stDefMsgBuf), lQueueType, IPC_NOWAIT | MSG_NOERROR) ;
	}
	return;
}


int LockSharedMem(stDefSharedMemory *pstShm)
{
	struct sembuf sops[2];

	sops[0].sem_num = 0;                   /* Operate on semaphore 0 */
	sops[0].sem_op = IPC_SEM_INIT;         /* Wait for value to equal 0 */
	sops[0].sem_flg = 0;

	sops[1].sem_num = 0;                   /* Operate on semaphore 0 */
	sops[1].sem_op = IPC_SEM_LOCK;         /* Increment value by one */
	sops[1].sem_flg = SEM_UNDO;

	if (semop(pstShm->iSemId, sops, 2) == -1) 
	{
		hzb_log_error( __FILE__ , __LINE__ , "semop() error, maybe interrupted! shm id=%d, sem id=%d, size=%d, errno: %d(%s)" , pstShm->iShmId, pstShm->iSemId, pstShm->size, errno, strerror(errno) );
		return(-1);
	}
	return(SUCC);
}


int UnlockSharedMem(stDefSharedMemory *pstShm)
{
	struct sembuf sops[1];

	sops[0].sem_num = 0;                     /* Operate on semaphore 0 */
	sops[0].sem_op = IPC_SEM_UNLOCK;         /* Wait for value to equal 0 */
	sops[0].sem_flg = IPC_NOWAIT|SEM_UNDO;

	if (semop(pstShm->iSemId, sops, 1) == -1) 
	{
		hzb_log_error( __FILE__ , __LINE__ , "semop() error, maybe interrupted! shm id=%d, sem id=%d, size=%d, errno: %d(%s)", pstShm->iShmId, pstShm->iSemId, pstShm->size, errno, strerror(errno) );
		return(SUCC);
	}
	return(SUCC);
}


int OpenSharedMem(stDefSharedMemory *pstShm, int iKeyId, int iSize)
{
	key_t k_Shm;
	key_t k_Sem;

	k_Shm = ftok(BASE_DIR, iKeyId);
	pstShm->iShmId = shmget(k_Shm, (size_t)iSize, 0666|IPC_CREAT);
	if (pstShm->iShmId == -1) 
	{
		hzb_log_error( __FILE__ , __LINE__ , "shmget() error ! key id=%d, size=%d, errno: %d(%s)", iKeyId, iSize, errno, strerror(errno) );
		return -1;
	}

	pstShm->iSemId = semget(k_Shm, 1, 0666|IPC_CREAT);
	if (pstShm->iSemId == -1) 
	{
		hzb_log_error( __FILE__ , __LINE__ , "semget() error ! key id=%d, size=%d, errno: %d(%s)", iKeyId, iSize, errno, strerror(errno) );
		return -1;
	}
	pstShm->pcData = (char*)shmat(pstShm->iShmId, NULL, 0);
	pstShm->size = iSize;

	hzb_log_debug( __FILE__ , __LINE__ , "OpenSharedMem ShmId[%u] Key_t[%u] size[%u] keyid[%u], SemId[%u], BASE_DIR=[%s]", pstShm->iShmId, k_Shm, pstShm->size, iKeyId, pstShm->iSemId, BASE_DIR );
	return(SUCC);
}

void CloseSharedMem(stDefSharedMemory *pstShm)
{
	if (pstShm->pcData != NULL) 
	{
		shmdt(pstShm->pcData);
		pstShm->pcData = NULL;
	}
	return;
}

void DestorySharedMem(stDefSharedMemory *pstShm)
{
	int nret;
	int iShmId;
	struct shmid_ds stShmCtl;


	nret = shmctl( pstShm->iShmId, IPC_RMID, &stShmCtl );
	if ( nret == -1 )
	{
		hzb_log_debug( __FILE__ , __LINE__ , "Failure to remove shared memory with id [%d]! (errno:%d==>%s)", pstShm->iShmId, errno, strerror(errno) );
		return;
	}
	nret = semctl( pstShm->iSemId, 0, IPC_RMID );
	if ( nret == -1 )
	{
		hzb_log_debug( __FILE__ , __LINE__ , "Failure to remove semaphore with id [%d]! (errno:%d==>%s)", pstShm->iSemId, errno, strerror(errno) );
		return;
	}
	return;
}


