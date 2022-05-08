#include "Daemon.h"

#include "Log.h"

#include <project_configs/config.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

static char* name = NULL;

void CreateDaemon(const struct CpSyncCmdData* cmdData, void(*SigUsrCallback)(int signalID)) 
{
  /* najpierw fork w procesie macierzystym */
  pid_t pid = fork();
    
    /* jeśli pid < 0 znaczy że nie udalo się wykonać fork, zakończ program */
  if (pid < 0)
  {
    exit(EXIT_FAILURE);
  }
    
   /* jeśli pid > 0 znaczy że jest to proces macierzysty więc kończy sie go */
  if (pid > 0)
  {
    exit(EXIT_SUCCESS);
  }
      
  /* następnie proces potomny zostaje liderem sesji co odłącza go od cttl(kontrolujacego terminala), jesli wynik ujemny zakończ */
  if (setsid() < 0)
  { 
    exit(EXIT_FAILURE);
  }
      

  signal(SIGCHLD, SIG_IGN); /// sygnał o zakończeniu się procesu potomnego ignoruje się
  signal(SIGHUP, SIG_IGN); /// sygnał o zamknięciu kontrolującego go terminala ignoruje się
  signal(SIGUSR1, SigUsrCallback); /// syngał usr1 (kill -10) ustaw custom callback
  signal(SIGUSR2, SigUsrCallback);
    
  /* druga operacja fork */
  pid = fork();
    
  /* --||-- */
  if (pid < 0)
  {
    exit(EXIT_FAILURE);
  }
         
  /* kolejny raz kończy się proces macierzysty dzięki temu nie zostanie przydzielony nowy ttl */
  if (pid > 0)
  {
    exit(EXIT_SUCCESS);
  }
      
  /* ustawienie praw dostępu do plików, 755 = najmniej restrykcyjne */
  umask(0755);
    
  /* folder w który traktowany jest przez proces jako ten z którego działa ustaw*/
  if(cmdData->workingDirPath == NULL)
  {
    chdir(PROJECT_DIR);
  }
  else
  {
    chdir(cmdData->workingDirPath);
  }
  
    
  /* otworzenie logu systemowgo */
  name = malloc(512);
  snprintf(name, 512, "| CopySync(recursive sync = %s, threshold = %lu, sleep time = %i s, srcDirPath = %s, dstDirPath = %s) |", 
           cmdData->recursiveSync ? "ON" : "OFF", cmdData->threshold, cmdData->sleepTime, cmdData->srcDirPath, cmdData->dstDirPath);

  openlog(name, LOG_PID, LOG_DAEMON);
  syslog(LOG_NOTICE, "daemon up");
}

void UninitDaemonLog() 
{
  syslog(LOG_NOTICE, "Copy Sync log closed");
  closelog();

  free(name);
  name = NULL;
}
