#include "Log.h"
#include "Utils.h"

#include <stdarg.h>
#include <time.h>
#include <syslog.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void SystemLog(int32_t logType, const char* mess, ...)
{
  va_list args;  
  va_start(args, mess);    

  time_t t;     
  time(&t);     

  char* date = ctime(&t);   
  date[strlen(date) - 1] = '\0'; 

  char prefixBuffer[128];
  snprintf(prefixBuffer, 128, "%s [%s] ", date, logType == LOGTYPE_INFO ? "INFO" : "ERROR");
  
  char messBuffer[1024];
  vsnprintf(messBuffer, 1024, mess, args);

  char* fullMess = MergeStrings(prefixBuffer, messBuffer, ' ');

  syslog(logType, fullMess);

  free(fullMess);

  va_end(args);

  /// ?? trzeba zamknac syslog jesli error
}