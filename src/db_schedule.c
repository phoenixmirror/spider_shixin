/* Processed by ecpg (4.1.1) */
/* These include files are added by the preprocessor */
#include <ecpgtype.h>
#include <ecpglib.h>
#include <ecpgerrno.h>
#include <sqlca.h>
/* End of automatic include section */
#line 1 "db_schedule.ec"
#include "common.h"

/* exec sql begin declare section */
                       
                
                       
                      
                      
                      
                   
                

#line 4 "db_schedule.ec"
 extern char  court_shixin_schedule_schedule_type [ 16 + 1 ]   ;
 
#line 4 "db_schedule.ec"
 extern short  court_shixin_schedule_schedule_type_id   ;
 
#line 5 "db_schedule.ec"
 extern int  court_shixin_schedule_order_index   ;
 
#line 5 "db_schedule.ec"
 extern short  court_shixin_schedule_order_index_id   ;
 
#line 6 "db_schedule.ec"
 extern char  court_shixin_schedule_schedule_name [ 64 + 1 ]   ;
 
#line 6 "db_schedule.ec"
 extern short  court_shixin_schedule_schedule_name_id   ;
 
#line 7 "db_schedule.ec"
 extern char  court_shixin_schedule_schedule_desc [ 128 + 1 ]   ;
 
#line 7 "db_schedule.ec"
 extern short  court_shixin_schedule_schedule_desc_id   ;
 
#line 8 "db_schedule.ec"
 extern int  court_shixin_schedule_pos_x   ;
 
#line 8 "db_schedule.ec"
 extern short  court_shixin_schedule_pos_x_id   ;
 
#line 9 "db_schedule.ec"
 extern int  court_shixin_schedule_pos_y   ;
 
#line 9 "db_schedule.ec"
 extern short  court_shixin_schedule_pos_y_id   ;
 
#line 10 "db_schedule.ec"
 extern int  court_shixin_schedule_progress   ;
 
#line 10 "db_schedule.ec"
 extern short  court_shixin_schedule_progress_id   ;
 
#line 11 "db_schedule.ec"
 extern int  court_shixin_schedule_pid   ;
 
#line 11 "db_schedule.ec"
 extern short  court_shixin_schedule_pid_id   ;
/* exec sql end declare section */
#line 12 "db_schedule.ec"



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

        { ECPGdo(__LINE__, 0, 1, NULL, "select  *  from court_shixin_schedule where schedule_type =  ? and schedule_name =  ?  ", 
	ECPGt_char,(court_shixin_schedule_schedule_type),(long)16 + 1,(long)1,(16 + 1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,(court_shixin_schedule_schedule_name),(long)64 + 1,(long)1,(64 + 1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, 
	ECPGt_char,(court_shixin_schedule_schedule_type),(long)16 + 1,(long)1,(16 + 1)*sizeof(char), 
	ECPGt_short,&(court_shixin_schedule_schedule_type_id),(long)1,(long)1,sizeof(short), 
	ECPGt_int,&(court_shixin_schedule_order_index),(long)1,(long)1,sizeof(int), 
	ECPGt_short,&(court_shixin_schedule_order_index_id),(long)1,(long)1,sizeof(short), 
	ECPGt_char,(court_shixin_schedule_schedule_name),(long)64 + 1,(long)1,(64 + 1)*sizeof(char), 
	ECPGt_short,&(court_shixin_schedule_schedule_name_id),(long)1,(long)1,sizeof(short), 
	ECPGt_char,(court_shixin_schedule_schedule_desc),(long)128 + 1,(long)1,(128 + 1)*sizeof(char), 
	ECPGt_short,&(court_shixin_schedule_schedule_desc_id),(long)1,(long)1,sizeof(short), 
	ECPGt_int,&(court_shixin_schedule_pos_x),(long)1,(long)1,sizeof(int), 
	ECPGt_short,&(court_shixin_schedule_pos_x_id),(long)1,(long)1,sizeof(short), 
	ECPGt_int,&(court_shixin_schedule_pos_y),(long)1,(long)1,sizeof(int), 
	ECPGt_short,&(court_shixin_schedule_pos_y_id),(long)1,(long)1,sizeof(short), 
	ECPGt_int,&(court_shixin_schedule_progress),(long)1,(long)1,sizeof(int), 
	ECPGt_short,&(court_shixin_schedule_progress_id),(long)1,(long)1,sizeof(short), 
	ECPGt_int,&(court_shixin_schedule_pid),(long)1,(long)1,sizeof(int), 
	ECPGt_short,&(court_shixin_schedule_pid_id),(long)1,(long)1,sizeof(short), ECPGt_EORT);}
#line 81 "db_schedule.ec"

        if( SQLCODE )
                return;

        DSCVTOS_court_shixin_schedule( pst );

        return;
}


void DSCSQLACTION_UPDATE_court_shixin_schedule_SET_A_WHERE_schedule_type_E_AND_order_index_E( court_shixin_schedule *pst )
{
        DSCSTOV_court_shixin_schedule( pst );

        { ECPGdo(__LINE__, 0, 1, NULL, "update court_shixin_schedule set schedule_type  =  ? , order_index  =  ? , schedule_name  =  ? , schedule_desc  =  ? , pos_x  =  ? , pos_y  =  ? , progress  =  ? , pid  =  ?  where schedule_type =  ? and order_index =  ?", 
	ECPGt_char,(court_shixin_schedule_schedule_type),(long)16 + 1,(long)1,(16 + 1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(court_shixin_schedule_order_index),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,(court_shixin_schedule_schedule_name),(long)64 + 1,(long)1,(64 + 1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,(court_shixin_schedule_schedule_desc),(long)128 + 1,(long)1,(128 + 1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(court_shixin_schedule_pos_x),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(court_shixin_schedule_pos_y),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(court_shixin_schedule_progress),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(court_shixin_schedule_pid),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,(court_shixin_schedule_schedule_type),(long)16 + 1,(long)1,(16 + 1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(court_shixin_schedule_order_index),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);}
#line 99 "db_schedule.ec"

        return;
}


int RefreshProgress( int progress )
{
	court_shixin_schedule	schedule;
	
	DSCDBBEGINWORK();
	
	memset( & schedule , 0x00 , sizeof(court_shixin_schedule) );
	strcpy( schedule.schedule_type , "collect" );
	strcpy( schedule.schedule_name , "shixin_unit_downloadlist_and_detail" );
	DSCSQLACTION_SELECT_A_FROM_court_shixin_schedule_WHERE_schedule_type_E_AND_schedule_name_E( & schedule );
	if( SQLCODE )
	{
		hzb_log_debug( __FILE__ , __LINE__ , "DSCSQLACTION_SELECT_A_FROM_court_shixin_schedule_WHERE_schedule_type_E_AND_schedule_name_E failed[%d][%s][%s]", SQLCODE , SQLSTATE, sqlca.sqlerrm.sqlerrmc );
		DSCDBROLLBACK();
		return -1;
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
		hzb_log_debug( __FILE__ , __LINE__ , "DSCSQLACTION_UPDATE_court_shixin_schedule_SET_A_WHERE_schedule_type_E_AND_order_index_E failed[%d][%s][%s]" , SQLCODE , SQLSTATE, sqlca.sqlerrm.sqlerrmc );
		DSCDBROLLBACK();
		return -1;
	}
	else
	{
		hzb_log_debug( __FILE__ , __LINE__ , "DSCSQLACTION_UPDATE_court_shixin_schedule_SET_A_WHERE_schedule_type_E_AND_order_index_E ok");
	}
	
	DSCDBCOMMIT();
	
	return 0;
}



