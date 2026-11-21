#include <math.h>
#include "Tree.h"

const int MAX_PROBLEMS = 16;
const TreeElem_t ERROR_VAL = 0xFFFFFFFF;
const TreeElem_t TEMP_VAL = 0;

struct Problem_t {
    Tree_t *Expression;
    Tree_t *Derivative;
    TreeElem_t ExpressionVal;
    TreeElem_t DerivativeVal;
};

struct DiffCtx_t {
    Problem_t *ProblemTable[MAX_PROBLEMS];
    size_t num_problems;
};

void Greet(void);
int SelectOption(int *calc_flag, int *diff_flag);
void GetVars(Tree_t *ExprTree);

void Differentiator(DiffCtx_t *DiffCtx, TreeLogCtx_t *TreeLogCtx, LatexLogCtx_t *LatexLogCtx);
TreeElem_t Calc(Tree_t *Tree, TreeNode_t *Curr);
TreeNode_t *Diff(Tree_t *Tree, const TreeNode_t *Curr);
TreeNode_t *Copy(TreeNode_t *Curr);
int FindVariables(Tree_t *Tree, TreeNode_t *Curr);
int FindX(TreeNode_t *Curr);

int ClearBuf(void);

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

int DblCmp(TreeElem_t a, TreeElem_t b);
