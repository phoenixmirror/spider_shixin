#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdarg.h>
#include <time.h>
#include <sys/times.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>

#include "debuglog.h"

int DebugLog( int iLogLevel, char *pucMsg, int iLen, ...);
void xmlRedirectLog(void *ctx, const char *msg, ...);


#define FILENAME_SIZE 256
#define SUCC 0
#define MAX_MSG_SIZE 1024



/* --------------------       CONSTANT DEFINITION        -------------------- */


/* --------------------        GLOBAL DECLARATION        -------------------- */
struct tm    *g_pstTmPtr;

/* --------   CALLED FUNCTION & SUBROUTINE PROTOTYPE DECLARATIONS   --------- */
struct tm *TmGetDate(void);
int Printable( char cChar );
ssize_t WriteLog(int iLogLevel, int iFD, const void *cpv_Buf, size_t s_Count);
void DebugDump(char *pucMsg, int iLen , int iLogFd1);
int DebugLogWithName(char *pcDebugLogFile, char *pcOLogFile1, int *piLogFd1,  int iLogLevel, char *pucMsg, int iLen, va_list vaArgs);
/* move to cmhdebug.h */

struct tm *TmGetDate()
{
  time_t     timeBuf;
  struct tm  *pstTmPtr;

  timeBuf  = time((long *) 0);
  pstTmPtr = localtime(&timeBuf);
  return(pstTmPtr);
}

/******************************************************************************
 *ROUTINE NAME: Printable
 *ARGUMENTS:
 *      char cChar : the original char.
 *RETURN VALUE(S):
 *      printable char
 *DESCRIPTION:
 *      pass a char to this function and then return its printable char
 ******************************************************************************/
int Printable( char cChar )
{
  return ( cChar >= 32 && cChar <= 126 );
}


/******************************************************************************
 *ROUTINE NAME: Write Log to file
 *ARGUMENTS:
 *      iLogLevel : Tracing log level, 0 ~ 5(with debug dump)
 *      iFD : file descriptor(same as write, stdio.h)
 *      cpv_Buf : void pointer to buffer written
 *                (same as write, stdio.h)
 *      size_t s_Count : size of buffer to be written.(same as write,
 *                       stdio.h)
 *RETURN VALUE(S):
 *      ssize_t(same as write, stdio.h)
 *DESCRIPTION:
 *      redefine of  write, stdio.h
 ******************************************************************************/
ssize_t WriteLog(int iLogLevel, int iFD, const void *cpv_Buf, size_t s_Count)
{
  return write(iFD, cpv_Buf, s_Count);
}

static char s_caBigChar[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F', ' '};

/******************************************************************************
 *ROUTINE NAME: DebugDump
 *ARGUMENTS:
 *      pucMsg : the starting address that the function will dump
 *      iLen : the number of bytes the function will dump.
 *RETURN VALUE(S):
 *      N/A
 *DESCRIPTION:
 *      dumping the memory block by specifying the starting address and
 *      length.
 *      whenever caBuf is filled with 60 lines,WriteLog to file.
 ******************************************************************************/
void DebugDump(char *pucMsg, int iLen , int iLogFd1)
{
  int   i,j, iIdx, iHi, iLow, iShift;
  int   iLine;
  char  caBuf[60*80];
  char  caTmp[11];

  g_pstTmPtr = TmGetDate();
  memset( caBuf,'\0', sizeof(caBuf) );
  sprintf( caBuf, "%.2d:%.2d:%.2d.(%ld): dumping --> (%d) \n",
           g_pstTmPtr->tm_hour, g_pstTmPtr->tm_min,
           g_pstTmPtr->tm_sec, (long int)getpid(), iLen );
  iLine = iLen%16 ? iLen/16 + 1 : iLen/16;
  /* modified by cywen 2004/02/03, improve DebugDump() performance to 30 times. */
  for ( i = 0; i < iLine; i++ ) {
    sprintf(caTmp, " %04d:", i+1);
    iShift = strlen(caTmp);
    iIdx = strlen(caBuf);
    memset(caBuf + iIdx, ' ', 70 + iShift);
    caBuf[iIdx + 70 + iShift] = '\n';
    memcpy(caBuf + iIdx, caTmp, iShift);

    for ( j = 0; j < 16; j++ ) {
      if ( i*16+j <  iLen ) {
        iHi = (pucMsg[i*16+j] & 0xF0) >> 4;
        iLow = pucMsg[i*16+j] & 0x0F;
        if ( Printable( pucMsg[i*16+j] ) ) {
          caBuf[iIdx + 54 + j + iShift] = pucMsg[i*16+j];
        } else {
          caBuf[iIdx + 54 + j + iShift] = '.';
        }
      }
      else {
        iHi = 16;
        iLow = 16;
      }
      if ( j <= 7 ) {
        caBuf[iIdx + (2*(j+1)) + j + iShift - 1] = s_caBigChar[iHi];
        caBuf[iIdx + (2*(j+1)) + j + 1 + iShift - 1] = s_caBigChar[iLow];
      }
      else {
        caBuf[iIdx + (2*(j+1)) + j + 1 + iShift - 1] = s_caBigChar[iHi];
        caBuf[iIdx + (2*(j+1)) + j + 2 + iShift - 1] = s_caBigChar[iLow];
      }
    }
    if ( (i+1)%60 == 0 ) {
      WriteLog( LOG_LEVEL_ALL, iLogFd1, caBuf, strlen(caBuf) );
      memset( caBuf,'\0',sizeof(caBuf) );
    }
  }

  WriteLog( LOG_LEVEL_ALL, iLogFd1, caBuf, strlen(caBuf) );
}
/*把libxml2的错误信息转向debuglog*/
static char   s_caOLogFile1[FILENAME_SIZE];
static int   s_iDebugLogFd1 = 1;

/******************************************************************************
 *ROUTINE NAME: DebugLog
 *ARGUMENTS:
 *      iLogLevel : Tracing log level, 0 ~ 5(with debug dump)
 *                  unsigned char *pucMsg : the starting address that
 *                  the function will dump
 *      int iLen : the number of bytes the function will dump.
 *RETURN VALUE(S):
 *      SUCC :g_iLogLevel < iLogLevel or Successful WriteLog
 *      -1   :open file error
 *DESCRIPTION:
 *      output the trace log to file g_caDebugLogFile(
 *      define by caller) + YYYMMDD, customized log Msg
 ******************************************************************************/
int DebugLog( int iLogLevel, char *pucMsg, int iLen, ...)
{
  va_list vaArgs;

  if ( (g_iLogLevel & 0x0F) < iLogLevel ) {
    return( SUCC );
  }

  va_start(vaArgs, iLen);
        /* Specify iLen is the last parameter of fix type argument.
           All of the parameter after iLen are arguments of varying types. */
  DebugLogWithName(g_caDebugLogFile, s_caOLogFile1, &s_iDebugLogFd1, iLogLevel, pucMsg, iLen, vaArgs);
  va_end(vaArgs);
  return(SUCC);
}

/******************************************************************************
 *ROUTINE NAME: DebugLog
 *ARGUMENTS:
 *      iLogLevel : Tracing log level, 0 ~ 5(with debug dump)
 *                  unsigned char *pucMsg : the starting address that
 *                  the function will dump
 *      int iLen : the number of bytes the function will dump.
 *RETURN VALUE(S):
 *      SUCC :g_iLogLevel < iLogLevel or Successful WriteLog
 *      -1   :open file error
 *DESCRIPTION:
 *      output the trace log to file g_caDebugLogFile(
 *      define by caller) + YYYYMMDD, customized log Msg
 ******************************************************************************/
int DebugLogWithName(char *pcDebugLogFile, char *pcOLogFile1, int *piLogFd1,  int iLogLevel, char *pucMsg, int iLen, va_list vaArgs)
{
  int   iChiYear;
  char  *pcFmt;
  char  caTmpBuf[100];
  char  caLogMsg[MAX_MSG_SIZE];
  char  caLogBuf[MAX_MSG_SIZE*2];
  char  caCurTime[11];
  char  caLevel[16];
  static pid_t  s_HerePid;
  struct timeval tv;
  struct timezone tz;
  char   caLogFile1[FILENAME_SIZE];
  long	lpcFmt = 0;

  if ( (g_iLogLevel & 0x0F) < iLogLevel ) {
    return( SUCC );
  }

  gettimeofday(&tv, &tz);
  g_pstTmPtr = localtime(&tv.tv_sec);
  iChiYear   = g_pstTmPtr->tm_year + 1900;
  s_HerePid = getpid();

  pcFmt = va_arg(vaArgs, char *);
  /* 如果va_arg失败，pcFmt地址为负，vsnprintf会coredump*/
  lpcFmt = (long)pcFmt;
  if (lpcFmt < 0) {
  	lpcFmt = 0;
  	return;
  }
  vsnprintf( caLogMsg, sizeof(caLogMsg)-3, pcFmt, vaArgs);

  if (caLogMsg[strlen(caLogMsg)-1] != '\n') {
    (void) strcat(caLogMsg,"\n");
  }
  memset(caCurTime,0,sizeof(caCurTime));
  sprintf(caCurTime,"%06u",tv.tv_usec);

  if (strlen(pcDebugLogFile) != 0) {


	  (void) sprintf(caLogFile1,"%s.%.04d%.2d%.2d%.2d", pcDebugLogFile,
	                 iChiYear, g_pstTmPtr->tm_mon+1, g_pstTmPtr->tm_mday, g_pstTmPtr->tm_hour);

	  if ( strcmp(caLogFile1, pcOLogFile1) ) {
	    /* the date changed, or its the first time */
	    if (*piLogFd1 != 1) {
	      sprintf(caTmpBuf,"%.2d%.2d%.2d!%ld: switch to new log file %s\n",
	              g_pstTmPtr->tm_hour, g_pstTmPtr->tm_min, g_pstTmPtr->tm_sec,
	              (long int)s_HerePid, caLogFile1);
	      WriteLog(iLogLevel, *piLogFd1, caTmpBuf, strlen(caTmpBuf));
	      /* close previous version of file */
	      (void) close(*piLogFd1);
	    }

	    umask(0);
	    if ( ( *piLogFd1 =
	           open(caLogFile1, O_APPEND|O_CREAT|O_RDWR, 0666) ) == -1 ) {
	      fprintf(stderr, "%s open error %d(%s)\n",
	              caLogFile1, errno, strerror(errno));
	      return(-1);
	    }
	    (void) strcpy(pcOLogFile1, caLogFile1);
	  }
  }


  memset(caLevel, 0, sizeof(caLevel));
  switch(iLogLevel & 0x0F)
  {
    case LOG_LEVEL_NONE:
      strcpy(caLevel, "[ERROR]");
      break;
    case LOG_LEVEL_ERROR:
      strcpy(caLevel, "[ERROR]");
      break;
    case LOG_LEVEL_WARNING:
      strcpy(caLevel, "[WARNING]");
      break;
    case LOG_LEVEL_AP_EVENT:
      strcpy(caLevel, "[INFO]");
      break;
    case LOG_LEVEL_SYS_EVENT:
      strcpy(caLevel, "[DEBUG]");
      break;
    default:
      strcpy(caLevel, "[ALL]");
      break;
  }


  /* form of msg line: hhmmss!(pid): usrmsg */
  (void) sprintf(caLogBuf, "%.2d:%.2d:%.2d.(%.10s)(%ld): %s%s",
                 g_pstTmPtr->tm_hour, g_pstTmPtr->tm_min,
                 g_pstTmPtr->tm_sec,caCurTime, (long int)s_HerePid, caLevel, caLogMsg);
  WriteLog(iLogLevel, *piLogFd1, caLogBuf, strlen(caLogBuf));

  /* For debugdump */
  if ( ( (g_iLogLevel & 0x0F) >= iLogLevel ) &&
       ( pucMsg != (char *)NULL ) &&
       ( iLen > 0 ) ) {
    DebugDump(pucMsg, iLen, *piLogFd1);
  }
  return( SUCC );
}


