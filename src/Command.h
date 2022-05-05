#ifndef COMMAND_H
#define COMMAND_H

#include <inttypes.h>
#include <stdbool.h>

struct CpSyncCmdData
{
  bool recursiveSync;
  uint32_t sleepTime;
  uint64_t threshold;

  const char* workingDirPath;

  const char* srcDirPath;
  const char* dstDirPath;
};

void FreeCommandData(struct CpSyncCmdData* data);

#endif