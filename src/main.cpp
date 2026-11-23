#include "Names.h"
#include "Tree.h"
#include "Diff.h"
#include "DSL.h"

int main(void)
{
    TreeLogCtx_t TreeLogCtx = {};
    LatexLogCtx_t LatexLogCtx = {};
    DiffCtx_t DiffCtx = {};

    //   add DiffCtx ctor & dtor to calloc and free ProblemTable[]

    LogInit(&TreeLogCtx, &LatexLogCtx);

    Differentiator(&DiffCtx, &TreeLogCtx, &LatexLogCtx);

    LatexDump(&DiffCtx, &LatexLogCtx);

    DifferentiatorDtor(&DiffCtx);

    LogDeinit(&TreeLogCtx, &LatexLogCtx);

    return 0;
}