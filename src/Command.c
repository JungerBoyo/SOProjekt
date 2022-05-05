#include "Command.h"
#include <stdlib.h>

void FreeCommandData(struct CpSyncCmdData* data)
{
  free((char*)data->srcDirPath);
  free((char*)data->dstDirPath);
  free((char*)data->workingDirPath);
}