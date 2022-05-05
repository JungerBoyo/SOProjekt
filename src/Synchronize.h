#ifndef SYNCHRONIZE_H
#define SYNCHRONIZE_H

#include "Command.h"

/**
 * @brief performs dirs synchronization based on cmdData
 * @param cmdData 
 */
void SyncDirs(const struct CpSyncCmdData* cmdData);

#endif