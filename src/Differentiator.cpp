#include "Common.h"
#include "Differentiator.h"
#include "DSL.h"
#include "Dump.h"
#include "Tools.h"
#include "Tree.h"

void Differentiator(DiffCtx_t *DiffCtx, TreeLogCtx_t *TreeLogCtx, LatexLogCtx_t *LatexLogCtx)
{
    const int MAX_LENGTH = 1000;
    char buf[MAX_LENGTH] = {};
    int quit_flag = 0;

    Flags_t Flags = {};

    Greet();
  
    while(1)
    {
        Flags.expr_calc_flag   = 0;
        Flags.expr_diff_flag   = 0;
        Flags.derive_calc_flag = 0;

        SlowPrintf("Enter an expression: ");

        Problem_t *Problem = NULL;
        quit_flag = GetExpression(DiffCtx, &Problem, buf);
        if (quit_flag)
            return;

        int num_flags = SelectOption(&Flags);
        if (num_flags == 0)
        {
            DiffCtx->num_problems++;
            return;
        }

        if (Flags.expr_calc_flag)
            ExprCalc(DiffCtx, Problem); 

        if (Flags.expr_diff_flag)
            ExprDiff(Problem);

        if (Flags.derive_calc_flag)
            DeriveCalc(DiffCtx, Problem);

        if (Problem->Expression)
            TreeDump(Problem->Expression, TreeLogCtx);
        if (Problem->Derivative)    
            TreeDump(Problem->Derivative, TreeLogCtx);

        DiffCtx->num_problems++;
    }
}

int GetExpression(DiffCtx_t *DiffCtx, Problem_t **Problem, char *buf)
{
    scanf("%s", buf);              
    if (buf[0] == 'q')
        return 1;

    ClearBuf();
    printf("\n");

    *Problem = (Problem_t *)calloc(1, sizeof(Problem_t));
    (*Problem)->Expression = (Tree_t *)calloc(1, sizeof(Tree_t));

    for (size_t i = 0; i < MAX_VARS; i++)
        (*Problem)->Expression->VarList[i] = POIZON_VAL;
    (*Problem)->ExpressionVal = POIZON_VAL;

    char *temp = buf;
    BuildFromPreOrder((*Problem)->Expression, (*Problem)->Expression->root, &temp);
    Optimize((*Problem)->Expression, (*Problem)->Expression->root);

    DiffCtx->ProblemTable[DiffCtx->num_problems] = (*Problem);

    return 0;
}

int ExprCalc(DiffCtx_t *DiffCtx, Problem_t *Problem)
{
    int count_vars = FindVariables(Problem->Expression, Problem->Expression->root);

    if (count_vars != 0)
    {
        int got_vars = GetVars(Problem->Expression);
        if (got_vars == 0)
        {
            DiffCtx->num_problems++;
            return 1;
        }
    }
    Problem->ExpressionVal = Calc(Problem->Expression, Problem->Expression->root);

    return 0;
}

int ExprDiff(Problem_t *Problem)
{
    Problem->Derivative = (Tree_t *)calloc(1, sizeof(Tree_t));
    Problem->Derivative->root = Diff(Problem->Expression, Problem->Expression->root);

    Optimize(Problem->Derivative, Problem->Derivative->root);

    for (size_t i = 0; i < MAX_VARS; i++)
        Problem->Derivative->VarList[i] = POIZON_VAL;
    Problem->DerivativeVal = POIZON_VAL;

    return 0;
}

int DeriveCalc(DiffCtx_t *DiffCtx, Problem_t *Problem)
{
    if (Problem->Derivative == NULL)
    {
        Problem->Derivative = (Tree_t *)calloc(1, sizeof(Tree_t));
        Problem->Derivative->root = Diff(Problem->Expression, Problem->Expression->root);

        Optimize(Problem->Derivative, Problem->Derivative->root);

        for (size_t i = 0; i < MAX_VARS; i++)
            Problem->Derivative->VarList[i] = POIZON_VAL;
        Problem->DerivativeVal = POIZON_VAL;
    }

    int count_vars = FindVariables(Problem->Derivative, Problem->Derivative->root);

    if (count_vars != 0)
    {
        int got_vars = GetVars(Problem->Derivative);
        if (got_vars == 0)
        {
            DiffCtx->num_problems++;
            return 1;
        }
    }

    Problem->DerivativeVal = Calc(Problem->Derivative, Problem->Derivative->root);

    return 0;
}

void DifferentiatorDtor(DiffCtx_t *DiffCtx)
{
    Problem_t *CurrProblem = NULL;

    for (size_t i = 0; i < DiffCtx->num_problems; i++)
    {
        CurrProblem = DiffCtx->ProblemTable[i];
        if (CurrProblem == NULL)
            continue;
        
        if (CurrProblem->Expression != NULL)
        {
            TreeFree(CurrProblem->Expression->root);
            free(CurrProblem->Expression);
        }

        if (CurrProblem->Derivative != NULL)
        {
            TreeFree(CurrProblem->Derivative->root);
            free(CurrProblem->Derivative);
        }

        CurrProblem->ExpressionVal = POIZON_VAL;
        CurrProblem->DerivativeVal = POIZON_VAL;

        free(CurrProblem);
        DiffCtx->ProblemTable[i] = NULL;
    }
}

typedef TreeElem_t (*CalcCase_t)(Tree_t *Tree, TreeNode_t *Curr);

static CalcCase_t CalcCaseTable[] = {
    calc_ADD,
    calc_SUB,
    calc_MUL,
    calc_DIV,
    calc_POW,
    calc_SIN,
    calc_COS,
    calc_LOG
};

TreeElem_t Calc(Tree_t *Tree, TreeNode_t *Curr)
{
    switch((Type_t)Curr->type)
    {
        case OP:
            return CalcCaseTable[(Operator_t)Curr->data](Tree, Curr);
        
        case NUM:
            return Curr->data;
        
        case VAR:
            return Tree->VarList[(int)Curr->data];
        
        case UNKNOWN_TYPE:
            return ERROR_VAL;
        
        default:
            return ERROR_VAL;
    }
}

typedef TreeNode_t* (*ComplexDiffCase_t)(Tree_t *Tree, const TreeNode_t *Curr);

static const ComplexDiffCase_t ComplexDiffTable[] = {
    diff_ADD,
    diff_SUB,
    diff_MUL,
    diff_DIV,
    diff_POW,
    diff_SIN,
    diff_COS,
    diff_LOG
};

TreeNode_t *Diff(Tree_t *Tree, const TreeNode_t *Curr)
{
    switch((Type_t)Curr->type)
    {
        case OP:
        return ComplexDiffTable[(Operator_t)Curr->data](Tree, Curr);
        
        case NUM:
        return _NUM(0);
        
        case VAR:
        {
        if ((int)Curr->data == 0)
            return _NUM(1);
        else
            return _NUM(0);
        }     

        case UNKNOWN_TYPE:
            return NULL;

        default:
            return NULL;
    }
}

void Optimize(Tree_t *Tree, TreeNode_t *Curr)
{
    IsOptimized_t is_tree_changing = TREE_IS_CHANGING;

    while (is_tree_changing)
    {
        FoldConstants(Tree, Curr);
        is_tree_changing = ElimIdentities(Tree, Curr);
    }
}

TreeElem_t FoldConstants(Tree_t *Tree, TreeNode_t *Curr)
{
    TreeElem_t LeftVal   = POIZON_VAL;
    TreeElem_t RightVal  = POIZON_VAL; 
    TreeElem_t Result    = POIZON_VAL;

    switch((Type_t)Curr->type)
    {
        case OP:
        {
            if (Curr->left)
                LeftVal  = FoldConstants(Tree, Curr->left);
            if (Curr->right)
                RightVal = FoldConstants(Tree, Curr->right);

            if ((Curr->left &&  Curr->right && DblCmp(LeftVal, ERROR_VAL) != 0 && DblCmp(RightVal, ERROR_VAL) != 0)
            ||  (Curr->left && !Curr->right && DblCmp(LeftVal, ERROR_VAL) != 0))
            {
                Result      = CalcCaseTable[(Operator_t)Curr->data](Tree, Curr);
                Curr->type  = NUM;
                Curr->data  = Result;
                
                if (Curr->left)
                {
                    TreeFree(Curr->left);
                    Curr->left  = NULL;
                }
                if (Curr->right)
                {
                    TreeFree(Curr->right);
                    Curr->right = NULL;
                }

                return Result;
            }
            
            return ERROR_VAL;
        }

        case NUM:
            return Curr->data;

        case VAR:
            return ERROR_VAL;

        case UNKNOWN_TYPE:
            return ERROR_VAL;

        default:
            return ERROR_VAL;
    }

    return Result;
}

IsOptimized_t ElimIdentities(Tree_t *Tree, TreeNode_t *Curr)
{
    IsOptimized_t is_tree_changing = TREE_IS_OPTIMIZED;

    if ((Type_t)Curr->type != OP)
        return TREE_IS_OPTIMIZED;

    TreeNode_t *TempLeft    = NULL;
    TreeNode_t *TempRight   = NULL;

    switch((Operator_t)Curr->data)
    {
        case ADD: 
        {
            _IFISLEFT(0)
                _ELIM_LEFT_IDENTITY;
        
            _IFISRIGHT(0)
                _ELIM_RIGHT_IDENTITY;

            return is_tree_changing;
        }
        case SUB:
        {
            _IFISRIGHT(0)
                _ELIM_LEFT_IDENTITY;

            return is_tree_changing;
        }
        case MUL:
        {
            _IFISLEFT(0)
                _ELIM_KILLER_NODE;

            _IFISRIGHT(0)
                _ELIM_KILLER_NODE;

            _IFISLEFT(1)
                _ELIM_LEFT_IDENTITY;

            _IFISRIGHT(1)
                _ELIM_RIGHT_IDENTITY;

            return is_tree_changing;
        }

        case DIV:
        {
            _IFISRIGHT(1)
                _ELIM_RIGHT_IDENTITY;

            _IFISLEFT(0)
                _ELIM_KILLER_NODE;

            return is_tree_changing;
        }

        case POW: case SIN: case COS: case LOG:
        default:
            return is_tree_changing;
    }

    return is_tree_changing;
}

int FindX(TreeNode_t *Curr)
{
    int is_x_left  = 0;
    int is_x_right = 0;
    
    if (Curr->type == NUM)
        return 0;

    if (Curr->type == VAR && (int)Curr->data == 0)
        return 1;

    is_x_left =  FindX(Curr->left);
    if (is_x_left)
        return is_x_left;

    is_x_right = FindX(Curr->right);
    if (is_x_right)
        return is_x_right;

    return 0;
}

int FindVariables(Tree_t *Tree, TreeNode_t *Curr)
{
    static int res = 0;

    if (Curr->type == VAR)
    {
        Tree->VarList[(int)Curr->data] = TEMP_VAL;
        res = 1;
    }

    if (Curr->left)
        FindVariables(Tree, Curr->left);
    if (Curr->right)
        FindVariables(Tree, Curr->right);

    return res;
}

int GetVars(Tree_t *ExprTree)
{
    int ok_flag = 0;
    int ch = 0;
    int garbage = 0;
    int state = 0;
    int got_vars = 0;

    SlowPrintf("Enter variables:\n");

    for (size_t i = 0; i < MAX_VARS; i++)
    {
        ok_flag = 0;
        garbage = 0;
        state = 0;

        if (DblCmp(ExprTree->VarList[i], TEMP_VAL) == 0)
        {
            while (!ok_flag)
            {
                SlowPrintf("%c = ", 'x' + i);
                state = scanf("%lg", &(ExprTree->VarList[i]));
                
                if (state == 0)
                {
                    if ((ch = getchar()) == 'q')
                        return 0;
                }

                garbage = ClearBuf();

                ok_flag = (state != 0 && garbage == 0 && isfinite(ExprTree->VarList[i]));
            }
            got_vars++;
        }
    }
    return got_vars;
}

void Greet(void)
{
    SlowPrintf("###########################\n");
    SlowPrintf("-----DIFFERENTIATOR.AI-----\n");
    SlowPrintf("###########################\n\n");   
}

int SelectOption(Flags_t *Flags)
{
    SlowPrintf("Select an option:\n");
    SlowPrintf("You can choose several options, enter them without a space\n");
    SlowPrintf("[1] Calculate the expression\n");
    SlowPrintf("[2] Differentiate the expression\n");
    SlowPrintf("[3] Calculate the derivative in a point\n");
    SlowPrintf("[q] Quit\n");

    const int BUF_SIZE = 10;
    char buf[BUF_SIZE] = {};

    scanf("%s", buf);

    if (buf[0] == 'q')
        return 0;

    if (strchr(buf, '1') != NULL)
        Flags->expr_calc_flag = 1;
    if (strchr(buf, '2') != NULL)
        Flags->expr_diff_flag = 1;
    if (strchr(buf, '3') != NULL)
        Flags->derive_calc_flag = 1;

    return Flags->expr_calc_flag + Flags->expr_diff_flag + Flags->derive_calc_flag;
}