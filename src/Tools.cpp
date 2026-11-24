#include "Common.h"
#include "Differentiator.h"
#include "DSL.h"
#include "Dump.h"
#include "Tools.h"
#include "Tree.h"


ssize_t GetFileSize(const char *file_name)
{
    struct stat stat_buf = {};
    stat(file_name, &stat_buf);

    return stat_buf.st_size;
}

size_t ReadFromFile(char **buf, const char *file_name)
{   
    FILE *fp = fopen(file_name, "r");
    if (!fp)
        return 0;

    ssize_t file_size = GetFileSize(file_name);

    *buf = (char *)calloc((size_t)file_size, sizeof(char));
    if (!buf)
        return 0;

    size_t count = fread(*buf, sizeof(char), (size_t)file_size, fp);

    fclose(fp);

    return count;
}

int ClearBuf(void)
{
	int garbage = 0;
	int ch = 0;
	while ((ch = getchar()) != '\n')
		if (!isspace(ch))
			garbage = 1;
	return garbage;	
}

void SlowPrintf(const char *fmt, ...)
{
    const int MAX_STR = 256;
    const int SLEEP_TIME = 10000;

    char buf[MAX_STR] = {};
    va_list args = {};

    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
    
    char *ptr = buf;

    for (; *ptr; ptr++)
    {
        printf("%c", *ptr);
        fflush(stdout);

        usleep(SLEEP_TIME);
    }
}

int DblCmp(TreeElem_t a, TreeElem_t b)
{
    const double EPSILON = 1e-10;
    
    if (fabs(a - b) < EPSILON)
        return 0;

    else if (a - b > EPSILON)
        return 1;

    else 
        return -1;
}