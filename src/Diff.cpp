#include "Tree.h"
#include "Diff.h"

TreeElem_t Calc(Tree_t *Tree, TreeNode_t *Curr)
{
    switch(Curr->type)
    {
        case OP:
        {
            switch((Operator_t)Curr->data)
            {
                case ADD:
                    return Calc(Tree, Curr->left) + Calc(Tree, Curr->right);
                    
                case SUB:
                    return Calc(Tree, Curr->left) - Calc(Tree, Curr->right);

                case MUL:
                    return Calc(Tree, Curr->left) * Calc(Tree, Curr->right);

                case DIV:
                {
                    TreeElem_t op2 = Calc(Tree, Curr->right);
                    if (DblCmp(op2, 0) == 0)
                        break;

                    return Calc(Tree, Curr->left) / op2;
                }

                case SIN:
                    return sin(Calc(Tree, Curr->left));

                case COS:
                    return cos(Calc(Tree, Curr->left));

                case LOG:
                    return log(Calc(Tree, Curr->left));
                
                default:
                    ;
            }
        }
        break;

        case NUM:
            return Curr->data;
        
        case VAR:
            return Tree->VarList[(int)Curr->data];

        default:
            return ERROR_VAL;
    }
}

TreeNode_t *Diff(Tree_t *Tree, const TreeNode_t *Curr)
{
    switch(Curr->type)
    {
        case OP:
        {
            switch((Operator_t)Curr->data)
            {
                case ADD:
                    return _ADD(_DL, _DR);
                
                case SUB:
                    return _SUB(_DL, _DR);

                case MUL:
                    return _ADD(_MUL(_DL, _CR), _MUL(_CL, _DR));

                case DIV:
                    return _DIV(_SUB(_MUL(_DL, _CR), _MUL(_CL, _DR)), _MUL(_CR, _CR)); 

                default:
                    ;
            }
        }

        case NUM:
            return _NUM(0);

        case VAR:
        {
            if ((int)Curr->data == 0)
                return _NUM(1);
            else
                return _NUM(0);
        }       
    }
}

int DblCmp(TreeElem_t a, TreeElem_t b)
{
    const double EPSILON = 1e-10;

    if (fabs(a - b) < EPSILON)
        return 0;

    return a - b;
}

TreeNode_t *_NUM(TreeElem_t data)
{
    return NewNode(NUM, data, NULL, NULL);
}

TreeNode_t *_VAR(int var_num)
{
    return NewNode(VAR, (TreeElem_t)var_num, NULL, NULL);
}

TreeNode_t *_ADD(TreeNode_t *left, TreeNode_t *right)
{
    return NewNode(OP, ADD, left, right); 
}

TreeNode_t *_SUB(TreeNode_t *left, TreeNode_t *right)
{
    return NewNode(OP, SUB, left, right);
}

TreeNode_t *_MUL(TreeNode_t *left, TreeNode_t *right) 
{ 
    return NewNode(OP, MUL, left, right); 
}

TreeNode_t *_DIV(TreeNode_t *left, TreeNode_t *right)
{
    return NewNode(OP, DIV, left, right);
}

TreeNode_t *_SIN(TreeNode_t *left)
{
    return NewNode(OP, SIN, left, NULL);
}

TreeNode_t *_COS(TreeNode_t *left)
{
    return NewNode(OP, COS, left, NULL);
}

TreeNode_t *_LOG(TreeNode_t *left)
{
    return NewNode(OP, LOG, left, NULL);
}

TreeNode_t *Copy(TreeNode_t *Curr)
{
    if (!Curr->left && !Curr->right)
        return NewNode(Curr->type, Curr->data, NULL, NULL);

    TreeNode_t *Copy_left  = NULL;
    TreeNode_t *Copy_right = NULL;

    if (Curr->left)
        Copy_left  = Copy(Curr->left);
    if (Curr->right)
        Copy_right = Copy(Curr->right);

    return NewNode(Curr->type, Curr->data, Copy_left, Copy_right);
}
