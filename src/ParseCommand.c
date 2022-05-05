#include "ParseCommand.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define KB 1000
#define MB 1000000
#define GB 1000000000

static uint64_t ParseThreshold(const char* str)
{
  size_t len = strlen(str);
  if(str[len-1] != 'B')
  {
    return 0;
  }
  else
  {
    switch(str[len-2])
    {
      case 'G': return strtoull(str, NULL, 10) * GB;  
      case 'M': return strtoull(str, NULL, 10) * MB;
      case 'K': return strtoull(str, NULL, 10) * KB;    
      default:  return strtoull(str, NULL, 10);     
    }  
  }
}


static const char* HELP = "\
  Usage:\n\n\
    CpSync -s <sleep_time> -t <threshold{B, MB, GB}> -r -w <working_dir> -S (srcDir) -D (dstDir)\n\n\
    ////////////////////////////////////////////////////////////////////////////////\n\
    ///> sleep time         daemon sleep time in seconds                        ////\n\
    ///> threshold          file size < threshold ? write : mmap                ////\n\
    ///> -R                 allow recursive sync                                ////\n\
    ///> working dir        directory path from which program will operate      ////\n\
    ///> (srcDir)/(dstDir)  REQUIRED arguments, tells which dirs to synchronize ////\n\
    ////////////////////////////////////////////////////////////////////////////////\n\n\n\
  Options:\n\n\
    -h    show this message\n\n";

struct CpSyncCmdData ParseCommand(int32_t argc, const char* const* argv)
{
  struct CpSyncCmdData data = {
    .sleepTime      = 300,
    .recursiveSync  = false,
    .threshold      = UINT64_MAX,
    .workingDirPath = NULL,
    .srcDirPath     = NULL,
    .dstDirPath     = NULL
  };

  char i;
  while((i = getopt(argc, argv, "hs:t:rw:S:D:")) != -1)
  {
    if(i == '?')
    { 
      printf("ParseCommand error\n\n %s", HELP);
      exit(EXIT_FAILURE);
    } 
    else
    {
      switch(i)
      {
        case 's': 
            data.sleepTime = atoi(optarg);
          break;

        case 'r': 
            data.recursiveSync = true;
          break;

        case 't': 
            data.threshold = ParseThreshold(optarg);
          break;

        case 'w': 
            data.workingDirPath = strdup(optarg);
          break;

        case 'S':
            data.srcDirPath = strdup(optarg);
          break;

        case 'D':
            data.dstDirPath = strdup(optarg);
          break;  

        case 'h': 
            printf("%s", HELP); exit(EXIT_SUCCESS); 
          break;
      }
    }
  }

  if(data.srcDirPath == NULL || data.dstDirPath == NULL)
  {
    printf("ParseCommand error\n\n %s", HELP);
    exit(EXIT_FAILURE);
  }

  return data;
}

