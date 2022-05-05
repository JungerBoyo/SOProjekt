#ifndef DAEMON_H
#define DAEMON_H

#include "ParseCommand.h"

/**
 * @brief Creates daemon and opens system log
 * 
 * @param cmdData command arguments data from ParseCommand(...)
 */
void CreateDaemon(const struct CpSyncCmdData* cmdData, void(*SigUsrCallback)(int signalID));

/**
 * @brief closes system log
 */
void UninitDaemonLog();

#endif