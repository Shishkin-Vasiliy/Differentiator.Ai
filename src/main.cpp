#include "Tree.h"
#include "Diff.h"

int main(void)
{
    TreeLogCtx_t TreeLogCtx = {};
    LatexLogCtx_t LatexLogCtx = {};
    DiffCtx_t DiffCtx = {};

    LogInit(&TreeLogCtx, &LatexLogCtx);

    Differentiator(&DiffCtx, &TreeLogCtx, &LatexLogCtx);

    LogDeinit(&TreeLogCtx, &LatexLogCtx);

    return 0;
}