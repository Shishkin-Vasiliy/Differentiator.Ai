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

int DblCmp(TreeElem_t a, TreeElem_t b)
{
    const double EPSILON = 1e-10;

    if (fabs(a - b) < EPSILON)
        return 0;

    return a - b;
}