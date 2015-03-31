/* Processed by ecpg (4.1.1) */
/* These include files are added by the preprocessor */
#include <ecpgtype.h>
#include <ecpglib.h>
#include <ecpgerrno.h>
#include <sqlca.h>
/* End of automatic include section */
#line 1 "db.ec"
#include "common.h"

/* exec sql begin declare section */
                       
                     
                    
                    
                   
                   
                          

#line 4 "db.ec"
 extern char  court_shixin_task_task_type [ 16 + 1 ]   ;
 
#line 4 "db.ec"
 extern short  court_shixin_task_task_type_id   ;
 
#line 5 "db.ec"
 extern int  court_shixin_task_task_level   ;
 
#line 5 "db.ec"
 extern short  court_shixin_task_task_level_id   ;
 
#line 6 "db.ec"
 extern char  court_shixin_task_url [ 1024 + 1 ]   ;
 
#line 6 "db.ec"
 extern short  court_shixin_task_url_id   ;
 
#line 7 "db.ec"
 extern int  court_shixin_task_curl_result   ;
 
#line 7 "db.ec"
 extern short  court_shixin_task_curl_result_id   ;
 
#line 8 "db.ec"
 extern char  court_shixin_task_pathfilename [ 256 + 1 ]   ;
 
#line 8 "db.ec"
 extern short  court_shixin_task_pathfilename_id   ;
 
#line 9 "db.ec"
 extern int  court_shixin_task_parse_result   ;
 
#line 9 "db.ec"
 extern short  court_shixin_task_parse_result_id   ;
 
#line 10 "db.ec"
 extern char  court_shixin_task_remark [ 16 + 1 ]   ;
 
#line 10 "db.ec"
 extern short  court_shixin_task_remark_id   ;
/* exec sql end declare section */
#line 11 "db.ec"



/* exec sql begin declare section */
                 
                 
                 

#line 15 "db.ec"
 char  DBNAME [ 1024 + 1 ]   ;
 
#line 16 "db.ec"
 char  DBUSER [ 128 + 1 ]   ;
 
#line 17 "db.ec"
 char  DBPASS [ 128 + 1 ]   ;
/* exec sql end declare section */
#line 18 "db.ec"



void DSCDBCONN( char *host , int port , char *dbname , char *user , char *pass )
{
        strcpy( DBNAME , dbname );
        if( host )
        {
                strcat( DBNAME , "@" );
                strcat( DBNAME , host );
        }
        if( port )
        {
                strcat( DBNAME , ":" );
                sprintf( DBNAME + strlen(DBNAME) , "%d" , port );
        }
        strcpy( DBUSER , user );
        strcpy( DBPASS , pass );

        { ECPGconnect(__LINE__, 0, DBNAME , DBUSER , DBPASS , NULL, 0); }
#line 40 "db.ec"


        return;
}

void DSCDBDISCONN()
{
        { ECPGdisconnect(__LINE__, "CURRENT");}
#line 48 "db.ec"


        return;
}


void DSCDBBEGINWORK()
{
        { ECPGtrans(__LINE__, NULL, "begin transaction ");}
#line 57 "db.ec"


        return;
}

void DSCDBCOMMIT()
{
        { ECPGtrans(__LINE__, NULL, "commit");}
#line 65 "db.ec"


        return;
}

void DSCDBROLLBACK()
{
        { ECPGtrans(__LINE__, NULL, "rollback");}
#line 73 "db.ec"


        return;
}


void DSCSTOV_court_shixin_task( court_shixin_task *pst )
{
	memset(court_shixin_task_task_type, 0x00, sizeof(court_shixin_task_task_type));
	court_shixin_task_task_level = 0;
	memset(court_shixin_task_url, 0x00, sizeof(court_shixin_task_url));
	court_shixin_task_curl_result = 0;
	memset(court_shixin_task_pathfilename, 0x00, sizeof(court_shixin_task_pathfilename));
	court_shixin_task_parse_result = 0;
	memset(court_shixin_task_remark, 0x00, sizeof(court_shixin_task_remark));

        strcpy( court_shixin_task_task_type , pst->task_type );
        court_shixin_task_task_level = pst->task_level ;
        strcpy( court_shixin_task_url , pst->url );
        court_shixin_task_curl_result = pst->curl_result ;
        strcpy( court_shixin_task_pathfilename , pst->pathfilename );
        court_shixin_task_parse_result = pst->parse_result ;
        strcpy( court_shixin_task_remark , pst->remark );

	hzb_log_debug( __FILE__ , __LINE__ , "court_shixin_task_task_type=[%s]" , court_shixin_task_task_type );
	hzb_log_debug( __FILE__ , __LINE__ , "court_shixin_task_task_level=[%d]" , court_shixin_task_task_level );
	hzb_log_debug( __FILE__ , __LINE__ , "court_shixin_task_url=[%s]" , court_shixin_task_url );
	hzb_log_debug( __FILE__ , __LINE__ , "court_shixin_task_curl_result=[%d]" , court_shixin_task_curl_result );
	hzb_log_debug( __FILE__ , __LINE__ , "court_shixin_task_pathfilename=[%s]" , court_shixin_task_pathfilename );
	hzb_log_debug( __FILE__ , __LINE__ , "court_shixin_task_parse_result=[%d]" , court_shixin_task_parse_result );
	hzb_log_debug( __FILE__ , __LINE__ , "court_shixin_task_remark=[%s]" , court_shixin_task_remark );
        return;
}


void DSCSQLACTION_INSERT_INTO_court_shixin_task( court_shixin_task *pst )
{
        DSCSTOV_court_shixin_task( pst );

        { ECPGdo(__LINE__, 0, 1, NULL, "insert into court_shixin_task values(  ? ,  ? ,  ? ,  ? ,  ? ,  ? ,  ? )", 
	ECPGt_char,(court_shixin_task_task_type),(long)16 + 1,(long)1,(16 + 1)*sizeof(char), 
	ECPGt_short,&(court_shixin_task_task_type_id),(long)1,(long)1,sizeof(short), 
	ECPGt_int,&(court_shixin_task_task_level),(long)1,(long)1,sizeof(int), 
	ECPGt_short,&(court_shixin_task_task_level_id),(long)1,(long)1,sizeof(short), 
	ECPGt_char,(court_shixin_task_url),(long)1024 + 1,(long)1,(1024 + 1)*sizeof(char), 
	ECPGt_short,&(court_shixin_task_url_id),(long)1,(long)1,sizeof(short), 
	ECPGt_int,&(court_shixin_task_curl_result),(long)1,(long)1,sizeof(int), 
	ECPGt_short,&(court_shixin_task_curl_result_id),(long)1,(long)1,sizeof(short), 
	ECPGt_char,(court_shixin_task_pathfilename),(long)256 + 1,(long)1,(256 + 1)*sizeof(char), 
	ECPGt_short,&(court_shixin_task_pathfilename_id),(long)1,(long)1,sizeof(short), 
	ECPGt_int,&(court_shixin_task_parse_result),(long)1,(long)1,sizeof(int), 
	ECPGt_short,&(court_shixin_task_parse_result_id),(long)1,(long)1,sizeof(short), 
	ECPGt_char,(court_shixin_task_remark),(long)16 + 1,(long)1,(16 + 1)*sizeof(char), 
	ECPGt_short,&(court_shixin_task_remark_id),(long)1,(long)1,sizeof(short), ECPGt_EOIT, ECPGt_EORT);}
#line 120 "db.ec"

        return;
}











/*
EXEC SQL BEGIN DECLARE SECTION ;
        extern char court_shixin_schedule_schedule_type[ 16 + 1 ] ;     extern short court_shixin_schedule_schedule_type_id ;
        extern int court_shixin_schedule_order_index ;  extern short court_shixin_schedule_order_index_id ;
        extern char court_shixin_schedule_schedule_name[ 64 + 1 ] ;     extern short court_shixin_schedule_schedule_name_id ;
        extern char court_shixin_schedule_schedule_desc[ 128 + 1 ] ;    extern short court_shixin_schedule_schedule_desc_id ;
        extern int court_shixin_schedule_pos_x ;        extern short court_shixin_schedule_pos_x_id ;
        extern int court_shixin_schedule_pos_y ;        extern short court_shixin_schedule_pos_y_id ;
        extern int court_shixin_schedule_progress ;     extern short court_shixin_schedule_progress_id ;
        extern int court_shixin_schedule_pid ;  extern short court_shixin_schedule_pid_id ;
EXEC SQL END DECLARE SECTION ;



void DSCINITV_court_shixin_schedule()
{       
        memset( court_shixin_schedule_schedule_type , 0x00 , sizeof(court_shixin_schedule_schedule_type) );
        court_shixin_schedule_schedule_type_id = 0 ;
        court_shixin_schedule_order_index = 0 ;
        court_shixin_schedule_order_index_id = 0 ;
        memset( court_shixin_schedule_schedule_name , 0x00 , sizeof(court_shixin_schedule_schedule_name) );
        court_shixin_schedule_schedule_name_id = 0 ;
        memset( court_shixin_schedule_schedule_desc , 0x00 , sizeof(court_shixin_schedule_schedule_desc) );
        court_shixin_schedule_schedule_desc_id = 0 ;
        court_shixin_schedule_pos_x = 0 ;
        court_shixin_schedule_pos_x_id = 0 ;
        court_shixin_schedule_pos_y = 0 ;
        court_shixin_schedule_pos_y_id = 0 ;
        court_shixin_schedule_progress = 0 ;
        court_shixin_schedule_progress_id = 0 ;
        court_shixin_schedule_pid = 0 ;
        court_shixin_schedule_pid_id = 0 ;
        return;
}


void DSCSTOV_court_shixin_schedule( court_shixin_schedule *pst )
{
        strcpy( court_shixin_schedule_schedule_type , pst->schedule_type );
        court_shixin_schedule_order_index = pst->order_index ;
        strcpy( court_shixin_schedule_schedule_name , pst->schedule_name );
        strcpy( court_shixin_schedule_schedule_desc , pst->schedule_desc );
        court_shixin_schedule_pos_x = pst->pos_x ;
        court_shixin_schedule_pos_y = pst->pos_y ;
        court_shixin_schedule_progress = pst->progress ;
        court_shixin_schedule_pid = pst->pid ;
        return;
}


void DSCVTOS_court_shixin_schedule( court_shixin_schedule *pst )
{
        strcpy( pst->schedule_type , court_shixin_schedule_schedule_type );
        pst->order_index = court_shixin_schedule_order_index ;
        strcpy( pst->schedule_name , court_shixin_schedule_schedule_name );
        strcpy( pst->schedule_desc , court_shixin_schedule_schedule_desc );
        pst->pos_x = court_shixin_schedule_pos_x ;
        pst->pos_y = court_shixin_schedule_pos_y ;
        pst->progress = court_shixin_schedule_progress ;
        pst->pid = court_shixin_schedule_pid ;
        return;
}


void DSCSQLACTION_SELECT_A_FROM_court_shixin_schedule_WHERE_schedule_type_E_AND_schedule_name_E( court_shixin_schedule *pst )
{
        DSCSTOV_court_shixin_schedule( pst );

        EXEC SQL
                SELECT  *
		INTO :court_shixin_schedule_schedule_type :court_shixin_schedule_schedule_type_id , 
        :court_shixin_schedule_order_index :court_shixin_schedule_order_index_id , 
        :court_shixin_schedule_schedule_name :court_shixin_schedule_schedule_name_id , 
        :court_shixin_schedule_schedule_desc :court_shixin_schedule_schedule_desc_id , 
        :court_shixin_schedule_pos_x :court_shixin_schedule_pos_x_id , 
        :court_shixin_schedule_pos_y :court_shixin_schedule_pos_y_id , 
        :court_shixin_schedule_progress :court_shixin_schedule_progress_id , 
        :court_shixin_schedule_pid :court_shixin_schedule_pid_id
                FROM    court_shixin_schedule
                WHERE   schedule_type = :court_shixin_schedule_schedule_type AND schedule_name = :court_shixin_schedule_schedule_name
                ;
        if( SQLCODE )
                return;

        DSCVTOS_court_shixin_schedule( pst );

        return;
}


void DSCSQLACTION_UPDATE_court_shixin_schedule_SET_A_WHERE_schedule_type_E_AND_order_index_E( court_shixin_schedule *pst )
{
        DSCSTOV_court_shixin_schedule( pst );

        EXEC SQL
                UPDATE  court_shixin_schedule
                SET     schedule_type = :court_shixin_schedule_schedule_type , order_index = :court_shixin_schedule_order_index , schedule_name = :court_shixin_schedule_schedule_name , schedule_desc = :court_shixin_schedule_schedule_desc , pos_x = :court_shixin_schedule_pos_x , pos_y = :court_shixin_schedule_pos_y , progress = :court_shixin_schedule_progress , pid = :court_shixin_schedule_pid
                WHERE   schedule_type = :court_shixin_schedule_schedule_type AND order_index = :court_shixin_schedule_order_index
                ;
        return;
}


static int RefreshProgress( int progress )
{
	court_shixin_schedule	schedule;
	
	DSCDBBEGINWORK();
	
	memset( & schedule , 0x00 , sizeof(court_shixin_schedule) );
	strcpy( schedule.schedule_type , "collect" );
	strcpy( schedule.schedule_name , "shixin_unit_downloadlist" );
	DSCSQLACTION_SELECT_A_FROM_court_shixin_schedule_WHERE_schedule_type_E_AND_schedule_name_E( & schedule );
	if( SQLCODE )
	{
		hzb_log_debug( __FILE__ , __LINE__ , "DSCSQLACTION_SELECT_A_FROM_court_shixin_schedule_WHERE_schedule_type_E_AND_schedule_name_E failed[%d][%s]", SQLCODE , SQLSTATE );
		DSCDBROLLBACK();
		exit(1);
	}
	else
	{
		hzb_log_debug( __FILE__ , __LINE__ , "DSCSQLACTION_SELECT_A_FROM_court_shixin_schedule_WHERE_schedule_type_E_AND_schedule_name_E ok");
	}
	
	schedule.progress = progress ;
	schedule.pid = getpid() ;
	DSCSQLACTION_UPDATE_court_shixin_schedule_SET_A_WHERE_schedule_type_E_AND_order_index_E( & schedule );
	if( SQLCODE )
	{
		hzb_log_debug( __FILE__ , __LINE__ , "DSCSQLACTION_UPDATE_court_shixin_schedule_SET_A_WHERE_schedule_type_E_AND_order_index_E failed[%d][%s]" , SQLCODE , SQLSTATE );
		DSCDBROLLBACK();
		exit(1);
	}
	else
	{
		hzb_log_debug( __FILE__ , __LINE__ , "DSCSQLACTION_UPDATE_court_shixin_schedule_SET_A_WHERE_schedule_type_E_AND_order_index_E ok");
	}
	
	DSCDBCOMMIT();
	
	return 0;
}


*/


