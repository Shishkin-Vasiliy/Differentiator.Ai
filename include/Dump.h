#ifndef DUMP_H
#define DUMP_H

#include "Common.h"

void PrintPreOrder(TreeNode_t *Node);
int LogInit(TreeLogCtx_t *TreeLogCtx, LatexLogCtx_t *LatexLogCtx);
int LogDeinit(TreeLogCtx_t *TreeLogCtx, LatexLogCtx_t *LatexLogCtx);
int TreeDump(Tree_t *Tree, TreeLogCtx_t *TreeLogCtx);
void ConstructDot(Tree_t *Tree, FILE *fp);
void DescribeTree(Tree_t *Tree, TreeNode_t *Curr, FILE *fp);
void DescribeNodes(Tree_t *Tree, TreeNode_t *Curr, FILE *fp);
void DescribeEdges(Tree_t *Tree, TreeNode_t *Curr, FILE *fp);
int LatexDump(DiffCtx_t *DiffCtx, LatexLogCtx_t *LatexLogCtx);
void ConstructTex(DiffCtx_t *DiffCtx, FILE *fp);
void TexBeginning(FILE *fp);
void TexEnding(FILE *fp);
void DescribeExpression(Tree_t *Tree, FILE *fp);
void PreOrderExpression(Tree_t *Tree, TreeNode_t *Curr, FILE *fp);
void InOrderExpression(Tree_t *Tree, TreeNode_t *Curr, FILE *fp);

#endif