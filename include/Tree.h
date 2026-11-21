#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdarg.h>
#include <unistd.h>
#include <ctype.h>

typedef double TreeElem_t;

const TreeElem_t POIZON_VAL = 0x0D01B0EB;

const int DATA_SIZE = 64;
const int MAX_VARS = 3;

enum Type_t {
    OP,
    NUM,
    VAR,
    UNKNOWN_TYPE
};

enum Operator_t {
    ADD,
    SUB,
    MUL,
    DIV,
    POW,
    SIN,
    COS,
    LOG
};

struct TreeNode_t {
    Type_t type;
    TreeElem_t data;
    TreeNode_t *left;
    TreeNode_t *right;
};

struct Tree_t {
    TreeNode_t *root;
    TreeElem_t VarList[MAX_VARS];
};

enum TreeErr_t {
    TREE_OK,
    CALLOC_FAIL,
    BAD_PTR,
    FILE_OPEN_FAIL,
    TREE_EMPTY,
    TREE_CORRUPTED
};

struct TreeLogCtx_t {
    char **pict_index;
    size_t num_picts;
};

struct LatexLogCtx_t {
    char **pdf_index;
    size_t num_pdfs;
};

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
int LatexDump(Tree_t *Tree, LatexLogCtx_t *LatexLogCtx);

void ConstructDot(Tree_t *Tree, FILE *fp);
void ConstructTex(Tree_t *Tree, FILE *fp);

void DescribeTree(Tree_t *Tree, TreeNode_t *Curr, FILE *fp);
void DescribeExpression(Tree_t *Tree, FILE *fp);
void DescribeNodes(Tree_t *Tree, TreeNode_t *Curr, FILE *fp);
void DescribeEdges(Tree_t *Tree, TreeNode_t *Curr, FILE *fp);

void TexBeginning(FILE *fp);
void TexEnding(FILE *fp);

void PreOrderExpression(Tree_t *Tree, TreeNode_t *Curr, FILE *fp);
void InOrderExpression(Tree_t *Tree, TreeNode_t *Curr, FILE *fp);

ssize_t GetFileSize(const char *file_name);
size_t ReadFromFile(char **buf, const char *file_name);

TreeNode_t *BuildFromPreOrder(Tree_t *Tree, TreeNode_t *Curr, char **buf);
TreeNode_t *BuildFromPostOrder(Tree_t *Tree, TreeNode_t *Curr, char **buf);
TreeNode_t *BuildFromInOrder(Tree_t *Tree, TreeNode_t *Curr, char **buf);

void SlowPrintf(const char *fmt, ...);

void print_ADD(void);
void print_SUB(void);
void print_MUL(void);
void print_DIV(void);
void print_POW(void);
void print_SIN(void);
void print_COS(void);
void print_LOG(void);

void dot_ADD(TreeNode_t *Curr, FILE *fp);
void dot_SUB(TreeNode_t *Curr, FILE *fp);
void dot_MUL(TreeNode_t *Curr, FILE *fp);
void dot_DIV(TreeNode_t *Curr, FILE *fp);
void dot_POW(TreeNode_t *Curr, FILE *fp);
void dot_SIN(TreeNode_t *Curr, FILE *fp);
void dot_COS(TreeNode_t *Curr, FILE *fp);
void dot_LOG(TreeNode_t *Curr, FILE *fp);

#endif