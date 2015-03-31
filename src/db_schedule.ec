#include "common.h"

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



