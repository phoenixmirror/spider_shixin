/*
 * debuglog.h
 *
 *  Created on: 2011-9-17
 *      Author: wenchenyu
 */

#ifndef DEBUGLOG_H_
#define DEBUGLOG_H_

#include <stdarg.h>

#define	LOG_LEVEL_NONE		0	/* for DebugLog, LogLevel: none */
#define	LOG_LEVEL_ERROR		1	/* for DebugLog, LogLevel: error*/
#define	LOG_LEVEL_WARNING	2	/* for DebugLog, LogLevel: warning */
#define	LOG_LEVEL_AP_EVENT	3	/* for DebugLog, LogLevel: event1 */
#define	LOG_LEVEL_SYS_EVENT	4	/* for DebugLog, LogLevel: event2 */
#define	LOG_LEVEL_ALL		5	/* for DebugLog, LogLevel: all  */

char g_caDebugLogFile[256];
int g_iLogLevel;


int DebugLog( int iLogLevel, char *pucMsg, int iLen, ...);


#endif /* DEBUGLOG_H_ */


