#include "Tree.h"
#include "Diff.h"

void Differentiator(DiffCtx_t *DiffCtx, TreeLogCtx_t *TreeLogCtx, LatexLogCtx_t *LatexLogCtx)
{
    const int MAX_LENGTH = 1000;
    char buf[MAX_LENGTH] = {};

    int expr_calc_flag = 0;
    int expr_diff_flag = 0;
    int derive_calc_flag = 0;

    int got_vars = 0;

    Greet();
  
    while(1)
    {
        /// todo struct of flags 
        expr_calc_flag = 0;
        expr_diff_flag = 0;
        derive_calc_flag = 0;
        got_vars = 0;

        SlowPrintf("Enter an expression: ");

        ////// здесь будет функция запроса ввода выражения и получения выражения GetExpression()
        scanf("%s", buf);
        if (buf[0] == 'q')
            return;
        ClearBuf();
        printf("\n");
        Problem_t *Problem = NULL;
        Problem = (Problem_t *)calloc(1, sizeof(Problem_t));
        Problem->Expression = (Tree_t *)calloc(1, sizeof(Tree_t));

        char *temp = buf;
        BuildFromPreOrder(Problem->Expression, Problem->Expression->root, &temp);
        
        DiffCtx->ProblemTable[DiffCtx->num_problems] = Problem;
        //////

        /// todo pass a struct of flags
        int num_flags = SelectOption(&expr_calc_flag, &expr_diff_flag, &derive_calc_flag);
        if (num_flags == 0)
            return;

        int count_vars = FindVariables(Problem->Expression, Problem->Expression->root);
        if (expr_calc_flag) 
        {
            if (count_vars != 0)
            {
                got_vars = GetVars(Problem->Expression);
                if (got_vars == 0)
                    return;
            }

            Problem->ExpressionVal = Calc(Problem->Expression, Problem->Expression->root);
        } 

        if (expr_diff_flag)
        {
            Problem->Derivative = (Tree_t *)calloc(1, sizeof(Tree_t));
            Problem->Derivative->root = Diff(Problem->Expression, Problem->Expression->root);
        }

        if (derive_calc_flag)
        {
            if (Problem->Derivative == NULL)
            {
                Problem->Derivative = (Tree_t *)calloc(1, sizeof(Tree_t));
                Problem->Derivative->root = Diff(Problem->Expression, Problem->Expression->root);
            }

            count_vars = FindVariables(Problem->Derivative, Problem->Derivative->root);

            if (count_vars != 0)
            {
                got_vars = GetVars(Problem->Derivative);
                if (got_vars == 0)
                    return;
            }

            Problem->DerivativeVal = Calc(Problem->Derivative, Problem->Derivative->root);
        }

        if (Problem->Expression)
            TreeDump(Problem->Expression, TreeLogCtx);
        if (Problem->Derivative)    
            TreeDump(Problem->Derivative, TreeLogCtx);

        DiffCtx->num_problems++;
    }
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
    switch(Curr->type)
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
    switch(Curr->type)
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

int DblCmp(TreeElem_t a, TreeElem_t b)
{
    const double EPSILON = 1e-10;
    
    if (fabs(a - b) < EPSILON)
        return 0;

    else if (a - b > EPSILON)
        return 1;

    else 
        return -1;
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

void Greet(void)
{
    SlowPrintf("###########################\n");
    SlowPrintf("-----DIFFERENTIATOR.AI-----\n");
    SlowPrintf("###########################\n\n");   
}

int ClearBuf(void)
{
	int garbage = 0;
	int ch = 0;
	while ((ch = getchar()) != '\n')
		if (!isspace(ch))
			garbage = 1;
	return garbage;	
}

int SelectOption(int *expr_calc_flag, int *expr_diff_flag, int *derive_calc_flag)
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
        *expr_calc_flag = 1;
    if (strchr(buf, '2') != NULL)
        *expr_diff_flag = 1;
    if (strchr(buf, '3') != NULL)
        *derive_calc_flag = 1;

    return *expr_calc_flag + *expr_diff_flag + *derive_calc_flag;
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