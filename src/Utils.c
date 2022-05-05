#include "Utils.h"
#include <stdlib.h>
#include <string.h>

char* MergeStrings(const char* lhsStr, const char* rhsStr, const char delim)
{
  size_t lhsStrLen = strlen(lhsStr);
  size_t rhsStrLen = strlen(rhsStr);

  char* result = malloc(lhsStrLen + rhsStrLen + 1 + 1);

  strncpy(result, lhsStr, lhsStrLen);
  result[lhsStrLen] = delim;
  strncpy(result + lhsStrLen + 1, rhsStr, rhsStrLen);
  result[lhsStrLen + rhsStrLen + 1] = '\0';

  return result;
}