#ifndef DSL_H
#define DSL_H

#include "Names.h"
#include "Tree.h"
#include "Diff.h"

#define _NUM(data)          NewNode(NUM, data, NULL, NULL)
#define _VAR(var_num)       NewNode(VAR, (TreeElem_t)var_num, NULL, NULL)
#define _ADD(left, right)   NewNode(OP, ADD, left, right)
#define _SUB(left, right)   NewNode(OP, SUB, left, right)
#define _MUL(left, right)   NewNode(OP, MUL, left, right)
#define _DIV(left, right)   NewNode(OP, DIV, left, right)
#define _POW(left, right)   NewNode(OP, POW, left, right)
#define _SIN(left)          NewNode(OP, SIN, left, NULL) 
#define _COS(left)          NewNode(OP, COS, left, NULL)
#define _LOG(left)          NewNode(OP, LOG, left, NULL)

#define _DL Diff(Tree, Curr->left)
#define _DR Diff(Tree, Curr->right)
#define _CL Copy(Curr->left)
#define _CR Copy(Curr->right)

TreeElem_t calc_ADD(Tree_t *Tree, TreeNode_t *Curr);
TreeElem_t calc_SUB(Tree_t *Tree, TreeNode_t *Curr);
TreeElem_t calc_MUL(Tree_t *Tree, TreeNode_t *Curr);
TreeElem_t calc_DIV(Tree_t *Tree, TreeNode_t *Curr);
TreeElem_t calc_POW(Tree_t *Tree, TreeNode_t *Curr);
TreeElem_t calc_SIN(Tree_t *Tree, TreeNode_t *Curr);
TreeElem_t calc_COS(Tree_t *Tree, TreeNode_t *Curr);
TreeElem_t calc_LOG(Tree_t *Tree, TreeNode_t *Curr);

TreeNode_t *diff_ADD(Tree_t *Tree, const TreeNode_t *Curr);
TreeNode_t *diff_SUB(Tree_t *Tree, const TreeNode_t *Curr);
TreeNode_t *diff_MUL(Tree_t *Tree, const TreeNode_t *Curr);
TreeNode_t *diff_DIV(Tree_t *Tree, const TreeNode_t *Curr);
TreeNode_t *diff_POW(Tree_t *Tree, const TreeNode_t *Curr);
TreeNode_t *diff_SIN(Tree_t *Tree, const TreeNode_t *Curr);
TreeNode_t *diff_COS(Tree_t *Tree, const TreeNode_t *Curr);
TreeNode_t *diff_LOG(Tree_t *Tree, const TreeNode_t *Curr);

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