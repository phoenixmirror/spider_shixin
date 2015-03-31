#ifndef _H_COMMON_ 
#define _H_COMMON_ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/socket.h>                         
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <libxml/tree.h>
#include <libxml/parser.h>                      
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <pthread.h>
#include <curl/curl.h>
#include <iconv.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "hzb_log.h"
#include "debuglog.h"

#include <ecpglib.h>
#include <ecpgerrno.h>
#include <sqlca.h>

#undef SQLCODE
#define SQLCODE         (int)(sqlca.sqlcode)

#undef SQLSTATE
#define SQLSTATE        sqlca.sqlstate

#define SQLNOTFOUND     100

#define DLIST_NOMEMORY          -1100
#define DLIST_NOTFOUND          -1101
#define DLIST_DUP_USERDATA      -1102
#define DLIST_STEPERROR         -1103

#define DLIST_MAX_STEPD         8

#define URL_LEN			1024	
#define MAX_CNT			2000
#define THREAD_CNT		1
#define DEFAULT_DEPTH		10 
#define GET			"get"
#define	POST			"post"
#define POSTTYPE		"phoenix"
#define BUF_LEN			2048*4096
#define QUEUE_DEFAULT_QUEUETYPE	1L
#define KEYID_QUEUE_OUT		1
#define	QUEFN_RECV_TIMEOUT_RC	-2
#define	SUCC			0
#define	MAX_SHAREMEM_LEN	1024*1024*100
#define	BUFF_LEN		300
#define	URL_NUM			300000
#define BASE_DIR		"/home/idd"

struct dListNode
{
        struct  dListNode       *f,*n;
        char    userData[300];
	char	userContent[300];
	int	depth;
	int	index;
};
typedef struct dListNode        DListNode;

struct  dList
{
        DListNode       *head;
        int             cnt;
        int             useF[DLIST_MAX_STEPD];
        DListNode       *use[DLIST_MAX_STEPD];
};
typedef struct  dList           DList;
typedef int     DList_stepInfo;

typedef struct 
{
	DList *todo_list;
	DList *done_list;
	DList_stepInfo *info;
	CURL *pcurl;
} t_env;

typedef struct 
{
	long lQTypeSend;
	long lQTypeReturn;
	long lRc;
} stDefInternalMsgHead;

typedef struct 
{
	stDefInternalMsgHead stMsgHead;	
	unsigned char ucaText[URL_LEN];
} stDefMsgBuf;

typedef struct
{
        char    url[300];
/*        char    urlcontent[300];*/
        int     depth;
	int	analyse_cnt;		
} stDefurldata;

#define IPC_SEM_INIT    (short)0
#define IPC_SEM_LOCK    (short)1
#define IPC_SEM_UNLOCK  (short)-1

typedef struct {
	int iShmId;
	int iSemId;
	size_t size;
	char *pcData;
} stDefSharedMemory;

DList *DList_new(DList *list);
int DList_add(DList *list,char *userData, char *userContent, int depth, int (*addFunc)(char *userData));
int DList_clear(DList *list,int (*clearFunc)(char *userData));
int DList_del(DList *list,char *userData,int (*delFunc)(void *userData));
int DList_check(DList *list,char *userData,int (*checkFunc)(void *userData));
int DList_step(DList *list,int (*stepFunc)(void *userData));
int DList_step2(DList *list,int (*step2Func)(void *userData,void *stepData),void *stepData);
int DList_openStep(DList *list,DList_stepInfo *sinfo);
int DList_fetchStep(DList *list,DList_stepInfo *sinfo,char *userDataP, char *userContentP, int *depth, int *index);
int DList_closeStep(DList *list,DList_stepInfo *sinfo);

void open_resource();
void t_open_resource();
void close_resource();
int checkurl( char *url );
int checkfilter( char *content, char *url );
int checkinvlove( char *content, char *url);
void *analysehtml( void *arg );
int WriteHtmlToFile( char *filepath, char *html );
void get_shixin_pagecondition(char *seedurl );
int OpenUnixMsgQ(long ipckey);


typedef struct
{
        char    task_type[ 16 + 1 ] ;
        int     task_level ;
        char    url[ 1024 + 1 ] ;
        int     curl_result ; 
        char    pathfilename[ 256 + 1 ] ;
        int     parse_result ;
        char    remark[ 16 + 1 ] ;
} court_shixin_task ;


char court_shixin_task_task_type[16+1];
int court_shixin_task_task_level;
char court_shixin_task_url[1024+1];
int court_shixin_task_curl_result;
char court_shixin_task_pathfilename[256+1];
int court_shixin_task_parse_result;
char court_shixin_task_remark[16+1];
short court_shixin_task_task_type_id ;
short court_shixin_task_task_level_id ;
short court_shixin_task_url_id ;
short court_shixin_task_curl_result_id ;
short court_shixin_task_pathfilename_id ;
short court_shixin_task_parse_result_id ;
short court_shixin_task_remark_id ;


typedef struct
{
        char    schedule_type[ 16 + 1 ] ;
        int     order_index ;
        char    schedule_name[ 64 + 1 ] ;
        char    schedule_desc[ 128 + 1 ] ;
        int     pos_x ;
        int     pos_y ;
        int     progress ;
        int     pid ;
} court_shixin_schedule ;

char court_shixin_schedule_schedule_type[16+1];
int court_shixin_schedule_order_index;
char court_shixin_schedule_schedule_name[64+1];
char court_shixin_schedule_schedule_desc[128+1];
int court_shixin_schedule_pos_x;
int court_shixin_schedule_pos_y;
int court_shixin_schedule_progress;
int court_shixin_schedule_pid;
short court_shixin_schedule_schedule_type_id;
short court_shixin_schedule_order_index_id;
short court_shixin_schedule_schedule_name_id;
short court_shixin_schedule_schedule_desc_id;
short court_shixin_schedule_pos_x_id;
short court_shixin_schedule_pos_y_id;
short court_shixin_schedule_progress_id;
short court_shixin_schedule_pid_id;


#define DBVLLIST_court_shixin_task \
        :court_shixin_task_task_type :court_shixin_task_task_type_id , \
        :court_shixin_task_task_level :court_shixin_task_task_level_id , \
        :court_shixin_task_url :court_shixin_task_url_id , \
        :court_shixin_task_curl_result :court_shixin_task_curl_result_id , \
        :court_shixin_task_pathfilename :court_shixin_task_pathfilename_id , \
        :court_shixin_task_parse_result :court_shixin_task_parse_result_id , \
        :court_shixin_task_remark :court_shixin_task_remark_id
        	


#endif


