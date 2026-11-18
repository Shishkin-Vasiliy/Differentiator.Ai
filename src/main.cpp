#include "Tree.h"
#include "Diff.h"

int main(void)
{
    Tree_t Tree = {};
    LogCtx_t LogCtx = {};

    Tree.VarList[0] = 1;
    Tree.VarList[1] = 6;

    TreeLogInit(&LogCtx);

    PreOrderCtor(&Tree, "preorder.txt");

    PrintPreOrder(Tree.root);

    TreeDump(&Tree, &LogCtx);

    TreeLogDeinit(&LogCtx);

    TreeDtor(&Tree);

    return 0;
}