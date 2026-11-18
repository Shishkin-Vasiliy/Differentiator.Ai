#include "Tree.h"

int main(void)
{
    Tree_t Tree = {};
    LogCtx_t LogCtx = {};

    TreeLogInit(&LogCtx);

    PreOrderCtor(&Tree, "preorder.txt");

    PrintPreOrder(Tree.root);

    TreeDump(&Tree, &LogCtx);

    TreeLogDeinit(&LogCtx);

    TreeDtor(&Tree);

    return 0;
}