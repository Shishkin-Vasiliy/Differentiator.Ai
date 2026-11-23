#ifndef TREE_H
#define TREE_H

#include "Names.h"
#include "Tree.h"
#include "Diff.h"
#include "DSL.h"

TreeErr_t PreOrderCtor(Tree_t *Tree, const char *file_name);
TreeErr_t PostOrderCtor(Tree_t *Tree, const char *file_name);
TreeErr_t InOrderCtor(Tree_t *Tree, const char *file_name);

TreeErr_t TreeDtor(Tree_t *Tree);
TreeErr_t TreeFree(TreeNode_t *Curr);

TreeNode_t *NewNode(Type_t type, TreeElem_t data, TreeNode_t *left, TreeNode_t *right);

void PrintPreOrder(TreeNode_t *Node);
void PrintPostOrder(TreeNode_t *Node);
void PrintInOrder(TreeNode_t *Node);

void WritePreOrder(TreeNode_t *Node, FILE *fp);
void WritePostOrder(TreeNode_t *Node, FILE *fp);
void WriteInOrder(TreeNode_t *Node, FILE *fp);

int LogInit(TreeLogCtx_t *TreeLogCtx, LatexLogCtx_t *LatexLogCtx);
int LogDeinit(TreeLogCtx_t *TreeLogCtx, LatexLogCtx_t *LatexLogCtx);

int TreeDump(Tree_t *Tree, TreeLogCtx_t *TreeLogCtx);
int LatexDump(DiffCtx_t *DiffCtx, LatexLogCtx_t *LatexLogCtx);

void ConstructDot(Tree_t *Tree, FILE *fp);
void ConstructTex(DiffCtx_t *DiffCtx, FILE *fp);

void DescribeTree(Tree_t *Tree, TreeNode_t *Curr, FILE *fp);
void DescribeNodes(Tree_t *Tree, TreeNode_t *Curr, FILE *fp);
void DescribeEdges(Tree_t *Tree, TreeNode_t *Curr, FILE *fp);

void DescribeExpression(Tree_t *Tree, FILE *fp);
void PreOrderExpression(Tree_t *Tree, TreeNode_t *Curr, FILE *fp);
void InOrderExpression(Tree_t *Tree, TreeNode_t *Curr, FILE *fp);

void TexBeginning(FILE *fp);
void TexEnding(FILE *fp);


ssize_t GetFileSize(const char *file_name);
size_t ReadFromFile(char **buf, const char *file_name);

TreeNode_t *BuildFromPreOrder(Tree_t *Tree, TreeNode_t *Curr, char **buf);
TreeNode_t *BuildFromPostOrder(Tree_t *Tree, TreeNode_t *Curr, char **buf);
TreeNode_t *BuildFromInOrder(Tree_t *Tree, TreeNode_t *Curr, char **buf);

#endif