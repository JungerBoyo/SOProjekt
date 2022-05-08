#include "FileSystem.h"

#include "Utils.h"
#include "Log.h"

#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/mman.h>


bool DirExists(const char* path) 
{
  DIR* dir;
  if((dir = opendir(path)) != NULL)
  {
    closedir(dir);
    return true;
  }
  else
  {
    return false;
  }
}

bool GetFilesInfo(const char* dirPath, struct FileInfo** pFilesInfo, uint64_t* count)
{
  DIR* dir = NULL;
  if((dir = opendir(dirPath)) == NULL)
  {
    pFilesInfo = NULL;
    *count = 0;

    return false;
  }

  uint64_t i = 0;
  for(struct dirent* dirInfo = readdir(dir); dirInfo != NULL; dirInfo = readdir(dir))
  {
    if(strcmp(dirInfo->d_name, ".") != 0 && strcmp(dirInfo->d_name, "..") != 0)
    {
      ++i;
    }
  }

  *count = i;
  rewinddir(dir);

  struct FileInfo* resultInfo = malloc(i * sizeof(struct FileInfo));
  
  i = 0;
  for(struct dirent* dirInfo = readdir(dir); dirInfo != NULL; dirInfo = readdir(dir))
  {
    if(strcmp(dirInfo->d_name, ".") != 0 && strcmp(dirInfo->d_name, "..") != 0)
    {
      struct FileInfo info;

      info.isDir = (dirInfo->d_type == DT_DIR) ? true : false;
      info.path = MergeStrings(dirPath, dirInfo->d_name, '/');
      info.name = strdup(dirInfo->d_name);

      struct stat fileStat;
      stat(info.path, &fileStat);

      info.size = fileStat.st_size;
      info.mTime_nsec = fileStat.st_mtim.tv_nsec;
      info.mTime_sec = fileStat.st_mtim.tv_sec;
      info.preferredBlockSize = fileStat.st_blksize;
      info.permissions = fileStat.st_mode;

      resultInfo[i++] = info;
    }  
  }

  *pFilesInfo = resultInfo;

  closedir(dir);

  return true;
}

void MirrorFileTo(const struct FileInfo* file, const char* dirPath, uint64_t threshold) 
{
  const char* srcFilePath = file->path;
  char* dstFilePath = MergeStrings(dirPath, file->name, '/');
  
  int32_t srcFile = open(srcFilePath, O_RDONLY);
  if(srcFile == -1)
  {
    SystemLog(LOGTYPE_ERROR, "open() error, srcFilePath = %s", srcFilePath);
    exit(EXIT_FAILURE);
  }

  int32_t dstFile = open(dstFilePath, O_WRONLY|O_CREAT|O_TRUNC, 0777);
  if(dstFile == -1)
  {
    SystemLog(LOGTYPE_ERROR, "open() error, dstFilePath = %s", dstFilePath);
    exit(EXIT_FAILURE);
  }
  
  if(file->size < threshold)
  {
    uint64_t buffSize = file->preferredBlockSize;
    uint8_t* buffer = malloc(buffSize);

    while(true)
    {
      int64_t charsRead = read(srcFile, buffer, buffSize);
      write(dstFile, buffer, charsRead);

      if(charsRead < buffSize)
      {
        break;
      }
    }
    
    free(buffer);
  }
  else
  {
    uint64_t buffSize = file->preferredBlockSize;
    char* mappedSrcFile = mmap(NULL, file->size, PROT_READ, MAP_FILE | MAP_PRIVATE, srcFile, 0); 

    for(uint64_t i=0; true; i += buffSize)
    {
      if(i + buffSize < file->size)
      {
        write(dstFile, mappedSrcFile + i, buffSize);
      }
      else
      {
        write(dstFile, mappedSrcFile + i, file->size);
        break;
      }
    }

    munmap(mappedSrcFile, file->size);
  }

  /// set metadata
  struct stat dstFileStat;
  fstat(dstFile, &dstFileStat);

  enum {access, modification};
  struct timeval dstFileTimeValues[2];
  dstFileTimeValues[access].tv_sec = dstFileStat.st_atim.tv_sec;
  dstFileTimeValues[access].tv_usec = dstFileStat.st_atim.tv_nsec / 1000;
  dstFileTimeValues[modification].tv_sec = file->mTime_sec;
  dstFileTimeValues[modification].tv_usec = file->mTime_nsec / 1000;

  futimes(dstFile, dstFileTimeValues);
  fchmod(dstFile, file->permissions);
    
  free(dstFilePath);
  close(dstFile);
  close(srcFile);
}
