#include "Common.h"
#include "Differentiator.h"
#include "DSL.h"
#include "Dump.h"
#include "Tools.h"
#include "Tree.h"

int main(void)
{
    TreeLogCtx_t TreeLogCtx = {};
    LatexLogCtx_t LatexLogCtx = {};
    DiffCtx_t DiffCtx = {};

    LogInit(&TreeLogCtx, &LatexLogCtx);

    Differentiator(&DiffCtx, &TreeLogCtx, &LatexLogCtx);

    LatexDump(&DiffCtx, &LatexLogCtx);

    DifferentiatorDtor(&DiffCtx);

    LogDeinit(&TreeLogCtx, &LatexLogCtx);

    return 0;
}