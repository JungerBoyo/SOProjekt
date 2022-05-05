#ifndef FILEINFO_H
#define FILEINFO_H

#include <stdbool.h>
#include <inttypes.h>

struct FileInfo
{
  int64_t mTime_nsec;
  int64_t mTime_sec;
  int64_t size;
  int64_t preferredBlockSize;

  uint32_t permissions;

  bool isDir;

  const char* path;
  const char* name;
};

void FreeFilesInfo(struct FileInfo* pFilesInfo, uint64_t count);

#endif