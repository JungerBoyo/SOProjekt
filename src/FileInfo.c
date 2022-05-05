#include "FileInfo.h"
#include <stdlib.h>

void FreeFilesInfo(struct FileInfo* pFilesInfo, uint64_t count) 
{
  for(uint64_t i=0; i<count; ++i)
  {
    free((char*)pFilesInfo[i].path);
    free((char*)pFilesInfo[i].name);
  }

  free(pFilesInfo);
}
