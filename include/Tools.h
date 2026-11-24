#ifndef TOOLS_H
#define TOOLS_H

#include "Common.h"

ssize_t GetFileSize(const char *file_name);
size_t ReadFromFile(char **buf, const char *file_name);
int ClearBuf(void);
void SlowPrintf(const char *fmt, ...);
int DblCmp(TreeElem_t a, TreeElem_t b);

#endif