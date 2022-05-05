#include "ParseCommand.h"
#include "Daemon.h"
#include "Synchronize.h"
#include "FileSystem.h"
#include "Log.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>

static pthread_mutex_t condMutex;
static pthread_cond_t condVar;
static bool quit = false;

void WakeUpCallback(int signalID)
{
  if(signalID == SIGUSR1)
  {
    pthread_cond_signal(&condVar);
  }
  else if(signalID == SIGKILL)
  {
    pthread_cond_signal(&condVar);
    quit = true;
  }
}

int main(int32_t argc, const char* const* argv)
{
  struct CpSyncCmdData cmdData = ParseCommand(argc, argv); 

  if(!DirExists(cmdData.dstDirPath)  || !DirExists(cmdData.srcDirPath) ||
    (cmdData.workingDirPath != NULL) && !DirExists(cmdData.workingDirPath))
  {
    perror("passed src/dst/working dir paths are not valid");    
    exit(EXIT_FAILURE);
  }

  pthread_mutex_init(&condMutex, NULL);
  pthread_cond_init(&condVar, NULL);
 
  CreateDaemon(&cmdData, WakeUpCallback);
  while(!quit)
  {
    SyncDirs(&cmdData);
    
    SystemLog(LOGTYPE_INFO, "going to sleep");

    sleep(cmdData.sleepTime);

    time_t currTime;
    time(&currTime);

    struct timespec sleepTime = {
      .tv_sec  = currTime + cmdData.sleepTime,
      .tv_nsec = 0
    };

    pthread_mutex_lock(&condMutex);
    pthread_cond_timedwait(&condVar, &condMutex, &sleepTime);
    pthread_mutex_unlock(&condMutex);

    SystemLog(LOGTYPE_INFO, "woke up");
  } 

  pthread_mutex_destroy(&condMutex);
  pthread_cond_destroy(&condVar);
  
  FreeCommandData(&cmdData);

  SystemLog(LOGTYPE_INFO, "daemon quit");

  UninitDaemonLog();  
  exit(EXIT_SUCCESS);
}