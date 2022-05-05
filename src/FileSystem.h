#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdbool.h>
#include <inttypes.h>

#include "FileInfo.h"

/**
 * @brief Checks whether passed path is valid.
 * @param path 
 * @return true = exists 
 * @return false = not exists or is not a dir
 */
bool DirExists(const char* path);

/**
 * @brief Get the Files Info from dirPath, ALLOCATES MEMORY
 * 
 * @param dirPath 
 * @param pFilesInfo ptr to array of files info
 * @param count count of elements in array to which pFilesInfo points to
 * @return true if succeded
 * @return false if not succeded
 */
bool GetFilesInfo(const char* dirPath, struct FileInfo** pFilesInfo, uint64_t* count);

/**
 * @brief copy file to dir and sets its last mod time
 * 
 * @param file info about copied file
 * @param dir 
 * @param threshold if file size < thresold => read + write, else => mmap + write
 */
void MirrorFileTo(const struct FileInfo* file, const char* dirPath, uint64_t threshold);

#endif