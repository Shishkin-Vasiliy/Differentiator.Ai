#include "Names.h"
#include "Tree.h"
#include "Diff.h"
#include "DSL.h"

TreeElem_t calc_ADD(Tree_t *Tree, TreeNode_t *Curr)
{
    return Calc(Tree, Curr->left) + Calc(Tree, Curr->right);
}

TreeElem_t calc_SUB(Tree_t *Tree, TreeNode_t *Curr)
{
    return Calc(Tree, Curr->left) - Calc(Tree, Curr->right);
}

TreeElem_t calc_MUL(Tree_t *Tree, TreeNode_t *Curr)
{
    return Calc(Tree, Curr->left) * Calc(Tree, Curr->right);
}

TreeElem_t calc_DIV(Tree_t *Tree, TreeNode_t *Curr)
{
    TreeElem_t op2 = Calc(Tree, Curr->right);
    if (DblCmp(op2, 0) == 0)
        return ERROR_VAL;

return Calc(Tree, Curr->left) / op2;
}

TreeElem_t calc_POW(Tree_t *Tree, TreeNode_t *Curr)
{
    TreeElem_t op1 = Calc(Tree, Curr->left);
    TreeElem_t power = Calc(Tree, Curr->right);
    
    return pow(op1, power);
}

TreeElem_t calc_SIN(Tree_t *Tree, TreeNode_t *Curr)
{
    return sin(Calc(Tree, Curr->left));
}

TreeElem_t calc_COS(Tree_t *Tree, TreeNode_t *Curr)
{
    return cos(Calc(Tree, Curr->left));
}

TreeElem_t calc_LOG(Tree_t *Tree, TreeNode_t *Curr)
{
    return log(Calc(Tree, Curr->left));
}

TreeNode_t *diff_ADD(Tree_t *Tree, const TreeNode_t *Curr)
{
    return _ADD(_DL, _DR);
}

TreeNode_t *diff_SUB(Tree_t *Tree, const TreeNode_t *Curr)
{
    return _SUB(_DL, _DR);
}

TreeNode_t *diff_MUL(Tree_t *Tree, const TreeNode_t *Curr)
{
    return _ADD(_MUL(_DL, _CR), _MUL(_CL, _DR));
}

TreeNode_t *diff_DIV(Tree_t *Tree, const TreeNode_t *Curr)
{
    return _DIV(_SUB(_MUL(_DL, _CR), _MUL(_CL, _DR)), _MUL(_CR, _CR));
}

TreeNode_t *diff_POW(Tree_t *Tree, const TreeNode_t *Curr)
{
    int is_x_left  = FindX(Curr->left);
    int is_x_right = FindX(Curr->right);
    
    if (!is_x_left && !is_x_right)
        return _NUM(0);

    if (is_x_left && !is_x_right)
        return _MUL(_MUL(_CR, _POW(_CL, _SUB(_CR, _NUM(1)))), _DL);

    if (!is_x_left && is_x_right)
        return _MUL(_DR, _MUL(_POW(_CL, _CR), _LOG(_CL)));

    if (is_x_left && is_x_right)
        return _MUL(_ADD(_MUL(_DL, _LOG(_CL)), _DIV(_MUL(_CR, _DL), _CL)), _POW(_CL, _CR));

    return NULL;
}

TreeNode_t *diff_SIN(Tree_t *Tree, const TreeNode_t *Curr)
{
    return _MUL(_COS(_CL), _DL);
}

TreeNode_t *diff_COS(Tree_t *Tree, const TreeNode_t *Curr)
{
    return _MUL(_NUM(-1), _MUL(_SIN(_CL), _DL));
}

TreeNode_t *diff_LOG(Tree_t *Tree, const TreeNode_t *Curr)
{
    return _MUL(_DIV(_NUM(1), _CL), _DR);
}

void print_ADD(void) { printf("+"); }
void print_SUB(void) { printf("-"); }
void print_MUL(void) { printf("*"); }
void print_DIV(void) { printf("/"); }
void print_POW(void) { printf("^"); }
void print_SIN(void) { printf("sin"); }
void print_COS(void) { printf("cos"); }
void print_LOG(void) { printf("log"); }

void dot_ADD(TreeNode_t *Curr, FILE *fp) 
{
    fprintf(fp, "node_%p [shape=Mrecord; style=filled; fillcolor=\"#DB7093\"; fontcolor=\"#FFFFFF\";\n", Curr);
    fprintf(fp, "       label=\"  { OP | ADD }\"];\n");
}

void dot_SUB(TreeNode_t *Curr, FILE *fp) 
{
    fprintf(fp, "node_%p [shape=Mrecord; style=filled; fillcolor=\"#DB7093\"; fontcolor=\"#FFFFFF\";\n", Curr);
    fprintf(fp, "       label=\"  { OP | SUB }\"];\n");
}

void dot_MUL(TreeNode_t *Curr, FILE *fp) 
{
    fprintf(fp, "node_%p [shape=Mrecord; style=filled; fillcolor=\"#DB7093\"; fontcolor=\"#FFFFFF\";\n", Curr);
    fprintf(fp, "       label=\"  { OP | MUL }\"];\n");
}

void dot_DIV(TreeNode_t *Curr, FILE *fp) 
{
    fprintf(fp, "node_%p [shape=Mrecord; style=filled; fillcolor=\"#DB7093\"; fontcolor=\"#FFFFFF\";\n", Curr);
    fprintf(fp, "       label=\"  { OP | DIV }\"];\n");
}

void dot_POW(TreeNode_t *Curr, FILE *fp) 
{
    fprintf(fp, "node_%p [shape=Mrecord; style=filled; fillcolor=\"#DB7093\"; fontcolor=\"#FFFFFF\";\n", Curr);
    fprintf(fp, "       label=\"  { OP | POW }\"];\n");
}

void dot_SIN(TreeNode_t *Curr, FILE *fp) 
{
    fprintf(fp, "node_%p [shape=Mrecord; style=filled; fillcolor=\"#DB7093\"; fontcolor=\"#FFFFFF\";\n", Curr);
    fprintf(fp, "       label=\"  { OP | SIN }\"];\n");
}

void dot_COS(TreeNode_t *Curr, FILE *fp) 
{
    fprintf(fp, "node_%p [shape=Mrecord; style=filled; fillcolor=\"#DB7093\"; fontcolor=\"#FFFFFF\";\n", Curr);
    fprintf(fp, "       label=\"  { OP | COS }\"];\n");
}

void dot_LOG(TreeNode_t *Curr, FILE *fp) 
{
    fprintf(fp, "node_%p [shape=Mrecord; style=filled; fillcolor=\"#DB7093\"; fontcolor=\"#FFFFFF\";\n", Curr);
    fprintf(fp, "       label=\"  { OP | LOG }\"];\n");
}