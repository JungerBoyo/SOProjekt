#include "Synchronize.h"
#include "FileSystem.h"
#include "Utils.h"
#include "Log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILL(ptr, size, value) 

/**
 * @brief recursivly removes files starting from dirPath
 * 
 * @param dirPath 
 */
static void RecursiveRemove(const char* dirPath);

static void NativeSync(const char* srcPath, const char* dstPath, const uint64_t threshold, const bool recursive);

void SyncDirs(const struct CpSyncCmdData* cmdData) 
{
  NativeSync(cmdData->srcDirPath, cmdData->dstDirPath, cmdData->threshold, cmdData->recursiveSync);
} 

void RecursiveRemove(const char* dirPath)
{
  struct FileInfo* fInfo;
  uint64_t fInfoCount; 

  if(GetFilesInfo(dirPath, &fInfo, &fInfoCount) != true)
  {
    SystemLog(LOGTYPE_ERROR, "GetFileInfo() error, dir = ", dirPath);
    exit(EXIT_FAILURE);
  }

  for(uint64_t i=0; i < fInfoCount; ++i)
  {
    if(fInfo[i].isDir)
    {
      RecursiveRemove(fInfo[i].path);
    }
      
    remove(fInfo[i].path);
  }
}

void NativeSync(  
  const char* srcPath, 
  const char* dstPath, 
  const uint64_t threshold, 
  const bool recursive)
{
  struct FileInfo* fInfoDst;
  struct FileInfo* fInfoSrc;
  uint64_t fInfoDstCount; 
  uint64_t fInfoSrcCount;

  if(GetFilesInfo(dstPath, &fInfoDst, &fInfoDstCount) != true ||
     GetFilesInfo(srcPath, &fInfoSrc, &fInfoSrcCount) != true)
  {
    SystemLog(LOGTYPE_ERROR, "GetFileInfo() error, dst = %s  src = %s", dstPath, srcPath);
    exit(EXIT_FAILURE);
  }

  bool* existenceArray = malloc(fInfoDstCount);
  for(uint64_t i=0; i<fInfoDstCount; ++i)
  { 
    existenceArray[i] = false; 
  }

  for(uint64_t s_i=0; s_i < fInfoSrcCount; ++s_i)
  {
    bool copy = true;
    uint64_t d_i;
    for(d_i=0; d_i < fInfoDstCount; ++d_i)
    {
      if(strcmp(fInfoSrc[s_i].name, fInfoDst[d_i].name) == 0)
      {
        existenceArray[d_i] = true;
        if(fInfoSrc[s_i].mTime_nsec/1000 == fInfoDst[d_i].mTime_nsec/1000)
        {
          copy = false;
          break;
        }
      }
    }
    
    if(copy)
    {
      if(!fInfoSrc[s_i].isDir)
      {
        MirrorFileTo(&fInfoSrc[s_i], dstPath, threshold);
        SystemLog(LOGTYPE_INFO, "copied file %s from %s to %s", fInfoSrc[s_i].name, srcPath, dstPath);
      }
      else if(recursive)
      {
        if(!existenceArray[d_i])
        {
          char* newDstDirPath = MergeStrings(dstPath, fInfoSrc[s_i].name, '/');
          mkdir(newDstDirPath);
          chmod(newDstDirPath, fInfoSrc[s_i].permissions);

          NativeSync(fInfoSrc[s_i].path, newDstDirPath, threshold, recursive);
          free(newDstDirPath);
        }
        else
        {
          NativeSync(fInfoSrc[s_i].path, fInfoDst[d_i].path, threshold, recursive);
        }
      }
    }
  }

  for(uint64_t i=0; i < fInfoDstCount; ++i)
  {
    if(existenceArray[i] == false)
    {
      if(fInfoDst[i].isDir)
      {
        RecursiveRemove(fInfoDst[i].path);
      }

      remove(fInfoDst[i].path);
      SystemLog(LOGTYPE_INFO, "removed %s %s from %s", fInfoDst[i].isDir ? "dir" : "file", fInfoDst[i].name, dstPath);
    }
  }

  free(existenceArray);
  FreeFilesInfo(fInfoSrc, fInfoSrcCount);
  FreeFilesInfo(fInfoDst, fInfoDstCount);
}
