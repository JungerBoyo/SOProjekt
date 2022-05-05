#ifndef LOG_H
#define LOG_H

#include <inttypes.h>

/// LOG_NOTICE = 5, LOG_ERR = 3 (from syslog)
enum 
{ 
  LOGTYPE_INFO = 5,
  LOGTYPE_ERROR = 3
};

void SystemLog(int32_t logType, const char* mess, ...);

#endif