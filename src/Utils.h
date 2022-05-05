#ifndef UTILS_H
#define UTILS_H

/**
 * @brief merge two passed strings with delim inbetween, ALLOCATES MEMORY
 * 
 * @param lhsStr left hand side string 
 * @param rhsStr right hand side string
 * @param delim
 * @return char* (ptr to merged string) 
 */
char* MergeStrings(const char* lhsStr, const char* rhsStr, const char delim);

#endif