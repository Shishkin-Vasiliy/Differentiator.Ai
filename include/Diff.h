#ifndef DIFF_H
#define DIFF_H

#include "Names.h"
#include "Tree.h"
#include "DSL.h"

void Greet(void);
int SelectOption(int *expr_calc_flag, int *expr_diff_flag, int *derive_calc_flag);
int GetVars(Tree_t *ExprTree);

void Differentiator(DiffCtx_t *DiffCtx, TreeLogCtx_t *TreeLogCtx, LatexLogCtx_t *LatexLogCtx);
void DifferentiatorDtor(DiffCtx_t *DiffCtx);

TreeElem_t ConstantFolding(Tree_t *Tree, TreeNode_t *Curr);
TreeElem_t Calc(Tree_t *Tree, TreeNode_t *Curr);
int DblCmp(TreeElem_t a, TreeElem_t b);

TreeNode_t *Diff(Tree_t *Tree, const TreeNode_t *Curr);

TreeNode_t *Copy(TreeNode_t *Curr);
int FindVariables(Tree_t *Tree, TreeNode_t *Curr);
int FindX(TreeNode_t *Curr);

int ClearBuf(void);
void SlowPrintf(const char *fmt, ...);


#endif