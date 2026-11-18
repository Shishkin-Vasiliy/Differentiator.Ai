#include "Tree.h"
#include "Diff.h"

int main(void)
{
    Tree_t Tree = {};
    TreeLogCtx_t TreeLogCtx = {};
    LatexLogCtx_t LatexLogCtx = {};

    Tree.VarList[0] = 1;
    Tree.VarList[1] = 6;

    LogInit(&TreeLogCtx, &LatexLogCtx);

    PreOrderCtor(&Tree, "preorder.txt");

    PrintPreOrder(Tree.root);

    LatexDump(&Tree, &LatexLogCtx);
    TreeDump(&Tree, &TreeLogCtx);

    LogDeinit(&TreeLogCtx, &LatexLogCtx);

    TreeDtor(&Tree);

    return 0;
}