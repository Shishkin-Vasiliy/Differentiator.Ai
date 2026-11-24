#ifndef DIFF_H
#define DIFF_H

#include "Common.h"

void Greet(void);
int SelectOption(Flags_t *Flags);
int GetVars(Tree_t *ExprTree);

void Differentiator(DiffCtx_t *DiffCtx, TreeLogCtx_t *TreeLogCtx, LatexLogCtx_t *LatexLogCtx);
void DifferentiatorDtor(DiffCtx_t *DiffCtx);

int GetExpression(DiffCtx_t *DiffCtx, Problem_t **Problem, char *buf);
int ExprCalc(DiffCtx_t *DiffCtx, Problem_t *Problem);
int ExprDiff(Problem_t *Problem);
int DeriveCalc(DiffCtx_t *DiffCtx, Problem_t *Problem);

TreeElem_t Calc(Tree_t *Tree, TreeNode_t *Curr);

TreeNode_t *Diff(Tree_t *Tree, const TreeNode_t *Curr);

void Optimize(Tree_t *Tree, TreeNode_t *Curr);
TreeElem_t FoldConstants(Tree_t *Tree, TreeNode_t *Curr);
IsOptimized_t ElimIdentities(Tree_t *Tree, TreeNode_t *Curr);

int FindVariables(Tree_t *Tree, TreeNode_t *Curr);
int FindX(TreeNode_t *Curr);

#endif