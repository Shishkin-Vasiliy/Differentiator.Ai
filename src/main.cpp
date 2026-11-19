#include "Tree.h"
#include "Diff.h"

int main(void)
{
    Tree_t Tree = {};
    TreeLogCtx_t TreeLogCtx = {};
    LatexLogCtx_t LatexLogCtx = {};

    
    LogInit(&TreeLogCtx, &LatexLogCtx);
    
    PreOrderCtor(&Tree, "preorder.txt");
    
    Tree.VarList[0] = 1;
    Tree.VarList[1] = 6;
    Tree.VarList[2] = -5;

    Tree_t Tree_1 = {};
    Tree_1.root = Diff(&Tree, Tree.root);

    PrintPreOrder(Tree_1.root);
    printf("\n");
    PrintPreOrder(Tree.root);
    printf("\n");
    
    LatexDump(&Tree, &LatexLogCtx);
    TreeDump(&Tree, &TreeLogCtx);

    LogDeinit(&TreeLogCtx, &LatexLogCtx);

    TreeDtor(&Tree);
    TreeDtor(&Tree_1);

    return 0;
}