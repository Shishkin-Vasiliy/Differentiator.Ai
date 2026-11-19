#include <math.h>
#include "Tree.h"

const double ERROR_VAL = 0xFFFFFFFF;

TreeElem_t Calc(Tree_t *Tree, TreeNode_t *Curr);
TreeNode_t *Diff(Tree_t *Tree, const TreeNode_t *Curr);

TreeNode_t *Copy(TreeNode_t *Curr);

TreeNode_t *_NUM(TreeElem_t data);
TreeNode_t *_VAR(int var_num);
TreeNode_t *_ADD(TreeNode_t *left, TreeNode_t *right);
TreeNode_t *_SUB(TreeNode_t *left, TreeNode_t *right);
TreeNode_t *_MUL(TreeNode_t *left, TreeNode_t *right);
TreeNode_t *_DIV(TreeNode_t *left, TreeNode_t *right);
TreeNode_t *_SIN(TreeNode_t *left);
TreeNode_t *_COS(TreeNode_t *left);
TreeNode_t *_LOG(TreeNode_t *left);

#define _DL Diff(Tree, Curr->left)
#define _DR Diff(Tree, Curr->right)
#define _CL Copy(Curr->left)
#define _CR Copy(Curr->right)

int DblCmp(TreeElem_t a, TreeElem_t b);
