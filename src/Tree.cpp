#include "Tree.h"

TreeErr_t PreOrderCtor(Tree_t *Tree, const char *file_name)
{
    char *buf = NULL;
    ReadFromFile(&buf, file_name);
    
    char *temp = buf;
    BuildFromPreOrder(Tree, Tree->root, &temp);

    for (size_t i = 0; i < MAX_VARS; i++)
        Tree->VarList[i] = POIZON_VAL;

    free(buf);

    return TREE_OK;
}

TreeNode_t *NewNode(Type_t type, TreeElem_t data, TreeNode_t *left, TreeNode_t *right)
{
    TreeNode_t *Node = (TreeNode_t *)calloc(1, sizeof(TreeNode_t));
    if (Node)
    {
        Node->type = type;
        Node->data = data;
        Node->left = left;
        Node->right = right;
    }
    return Node;
}

TreeNode_t *FindNode(TreeNode_t *Curr, const char *data)
{   
    //if (!Curr)
    //    return NULL;
//
    //if (strcmp(Curr->data, data) == 0)
    //    return Curr;
//
    //else if (strcmp(Curr->data, data) > 0 && Curr->left)
    //    return FindNode(Curr->left, data);
//
    //else
    //    return FindNode(Curr->right, data);
}

void PrintPreOrder(TreeNode_t *Node)
{
    if (!Node)
        return;

    printf("(");

    switch(Node->type)
    {
        case NUM:
            printf("%lg", Node->data);
            break;
        
        case OP:
        {
            switch((Operator_t)Node->data)
            {
                case ADD:
                    printf("+");
                    break;
                case SUB:
                    printf("-");
                    break;
                case MUL:
                    printf("*");
                    break;
                case DIV:
                    printf("/");
                    break;
                case SIN:
                    printf("sin");
                    break;
                case COS:
                    printf("cos");
                    break;
                case LOG:
                    printf("log");
                    break;
            }
        }
        break;
        
        case VAR:
            printf("%c", 'x' + (int)Node->data);
        break;

        default:
            ;
    }

    if (Node->left)
        PrintPreOrder(Node->left);
    else if (Node->left == NULL && Node->right != NULL)
        printf("()");
    if (Node->right)
        PrintPreOrder(Node->right);
    else if (Node->right == NULL && Node->left != NULL)
        printf("()");

    printf(")");
}

TreeErr_t TreeDtor(Tree_t *Tree)
{
    return TreeFree(Tree->root);
}

TreeErr_t TreeFree(TreeNode_t *Curr)
{
    if (!Curr)
        return BAD_PTR;

    if (Curr->left)
        TreeFree(Curr->left);
    if (Curr->right)
        TreeFree(Curr->right);

    free(Curr);

    return TREE_OK;
}

int LogInit(TreeLogCtx_t *TreeLogCtx, LatexLogCtx_t *LatexLogCtx)
{
    const int MAX_PICTS = 16;
    const int MAX_PDFS = MAX_PICTS;
    int DirNotFound = system("[ -d Log ]");

    if (DirNotFound)
    {
        int err = system("mkdir Log");
        if (err)
        {
            return EOF;
        }
    }
    else 
    {
        system("rm -f Log/*");
    }

    TreeLogCtx->pict_index = (char **)calloc(MAX_PICTS, sizeof(char *));
    if (!TreeLogCtx->pict_index)
        return EOF;

    LatexLogCtx->pdf_index = (char **)calloc(MAX_PDFS, sizeof(char *));
    if (!LatexLogCtx->pdf_index)
        return EOF;

    return 0;
}

int LogDeinit(TreeLogCtx_t *TreeLogCtx, LatexLogCtx_t *LatexLogCtx)
{
    for (size_t i = 0; i < TreeLogCtx->num_picts && i < LatexLogCtx->num_pdfs; i++)
    {
        free(TreeLogCtx->pict_index[i]);
        free(LatexLogCtx->pdf_index[i]);
    }
    free(TreeLogCtx->pict_index);
    free(LatexLogCtx->pdf_index);

    return 0;
}

int TreeDump(Tree_t *Tree, TreeLogCtx_t *TreeLogCtx)
{
    const int PATH_SIZE    = 30;
    const int COMMAND_SIZE = 80;

    char dot_path[PATH_SIZE]   = {};
    char png_path[PATH_SIZE]   = {};
    char command[COMMAND_SIZE] = {};

    size_t num_picts = TreeLogCtx->num_picts;

    SlowPrintf("TreeDump processing...\n");

    snprintf(dot_path, PATH_SIZE, "Log/pict_%lu.dot", num_picts);

    FILE *dot_fp = fopen(dot_path, "w");
    if (!dot_fp)
        return EOF;

    ConstructDot(Tree, dot_fp);
    fclose(dot_fp);

    snprintf(command, COMMAND_SIZE, "dot %s -Tpng -o Log/pict_%lu.png", dot_path, num_picts);
    system(command);

    snprintf(png_path, PATH_SIZE, "pict_%lu.png", num_picts);

    char *temp = strdup(png_path);
    if (!temp)
        return EOF;

    TreeLogCtx->pict_index[num_picts] = temp;
    TreeLogCtx->num_picts++;

    SlowPrintf("Dump complete!\n");

    return 0;
}

void ConstructDot(Tree_t *Tree, FILE *fp)
{
    if (!fp)
        return;

    fprintf(fp, "digraph {\n");
    fprintf(fp, "bgcolor=\"#FFE4E1\";\n");
    fprintf(fp, "rankdir=TB;\n");      
    fprintf(fp, "nodesep=0.8;\n");
    fprintf(fp, "ranksep=0.9;\n");

    DescribeTree(Tree, Tree->root, fp);

    fprintf(fp, "}\n");
}

void DescribeTree(Tree_t *Tree, TreeNode_t *Curr, FILE *fp)
{  
    if (Curr->type == OP)
    {
        switch((Operator_t)Curr->data)
        {
            case ADD:
            {
                fprintf(fp, "node_%p [shape=Mrecord; style=filled; fillcolor=\"#DB7093\"; fontcolor=\"#FFFFFF\";\n", Curr);
                fprintf(fp, "       label=\"  { OP | ADD }\"];\n");
            }
            break;

            case SUB:
            {
                fprintf(fp, "node_%p [shape=Mrecord; style=filled; fillcolor=\"#DB7093\"; fontcolor=\"#FFFFFF\";\n", Curr);
                fprintf(fp, "       label=\"  { OP | SUB }\"];\n");
            }
            break;

            case MUL:
            {
                fprintf(fp, "node_%p [shape=Mrecord; style=filled; fillcolor=\"#DB7093\"; fontcolor=\"#FFFFFF\";\n", Curr);
                fprintf(fp, "       label=\"  { OP | MUL }\"];\n");
            }
            break;

            case DIV:
            {
                fprintf(fp, "node_%p [shape=Mrecord; style=filled; fillcolor=\"#DB7093\"; fontcolor=\"#FFFFFF\";\n", Curr);
                fprintf(fp, "       label=\"  { OP | DIV }\"];\n");
            }
            break;

            case SIN:
            {
                fprintf(fp, "node_%p [shape=Mrecord; style=filled; fillcolor=\"#DB7093\"; fontcolor=\"#FFFFFF\";\n", Curr);
                fprintf(fp, "       label=\"  { OP | SIN }\"];\n");
            }
            break;

            case COS:
            {
                fprintf(fp, "node_%p [shape=Mrecord; style=filled; fillcolor=\"#DB7093\"; fontcolor=\"#FFFFFF\";\n", Curr);
                fprintf(fp, "       label=\"  { OP | COS }\"];\n");
            }
            break;

            case LOG:
            {
                fprintf(fp, "node_%p [shape=Mrecord; style=filled; fillcolor=\"#DB7093\"; fontcolor=\"#FFFFFF\";\n", Curr);
                fprintf(fp, "       label=\"  { OP | LOG }\"];\n");
            }
            break;

            default:
                ;
        }
    }
    else if (Curr->type == NUM)
    {
        fprintf(fp, "node_%p [shape=Mrecord; style=filled; fillcolor=\"#DB7093\"; fontcolor=\"#FFFFFF\";\n", Curr);
        fprintf(fp, "       label=\"  { NUM | %lg }\"];\n", Curr->data);
    }
    else
    {
        fprintf(fp, "node_%p [shape=Mrecord; style=filled; fillcolor=\"#DB7093\"; fontcolor=\"#FFFFFF\";\n", Curr);
        fprintf(fp, "       label=\"  { VAR | %lg }\"];\n", Curr->data);
    }

    if (Curr == Tree->root)
        fprintf(fp, "{ rank=source; node_%p };\n", Tree->root);

    if (Curr->left)
        DescribeTree(Tree, Curr->left, fp);
    if (Curr->right)
        DescribeTree(Tree, Curr->right, fp);

    if (!(Curr->left) && !(Curr->right))
        fprintf(fp, "{ rank=sink; node_%p };\n", Curr);
    
    if (Curr != Tree->root)
    {
        if (Curr == Curr->parent->left)
        {
            fprintf(fp, "edge [color=\"#FFFFFF\"];\n");
            fprintf(fp, "node_%p:sw -> node_%p:n [color=\"#228B22\"]\n;", Curr->parent, Curr);
        }
        else if (Curr == Curr->parent->right)
        {
            fprintf(fp, "edge [color=\"#FFFFFF\"];\n");
            fprintf(fp, "node_%p:se -> node_%p:n [color=\"#228B22\"]\n;", Curr->parent, Curr);
        }
    }
    
    return;
}

int LatexDump(Tree_t *Tree, LatexLogCtx_t *LatexLogCtx)
{
    const int PATH_SIZE = 30;
    const int COMMAND_SIZE = 100;

    char latex_path[PATH_SIZE] = {};
    char command[COMMAND_SIZE] = {};
    char pdf_path[PATH_SIZE]   = {};

    SlowPrintf("LatexDump processing...\n");

    snprintf(latex_path, PATH_SIZE, "Log/texdump_%lu.tex", LatexLogCtx->num_pdfs);

    FILE *latex_fp = fopen(latex_path, "w");
    if (!latex_fp)
        return EOF;

    ConstructTex(Tree, latex_fp);
    fclose(latex_fp);

    snprintf(command, COMMAND_SIZE, "pdflatex -interaction=batchmode -output-directory=Log %s > /dev/null", latex_path);
    system(command);

    char *temp = strdup(pdf_path);
    if (!temp)
        return EOF;

    LatexLogCtx->pdf_index[LatexLogCtx->num_pdfs] = temp;
    LatexLogCtx->num_pdfs++;

    SlowPrintf("LatexDump complete!\n");

    return 0;
}

void ConstructTex(Tree_t *Tree, FILE *fp)
{
    if (!fp)
        return;

    fprintf(fp, "\\documentclass[a4paper,12pt]{article}\n");
    fprintf(fp, "\\usepackage[utf8]{inputenc}\n");
    fprintf(fp, "\\usepackage{listings}\n");
    fprintf(fp, "\\usepackage{xcolor}\n");
    fprintf(fp, "\\usepackage{geometry}\n");
    fprintf(fp, "\\geometry{margin=2cm}\n\n");
 
    fprintf(fp, "\\begin{document}\n");
    
    fprintf(fp, "$");
    DescribeExpression(Tree, fp);
    fprintf(fp, "$");
    
    fprintf(fp, "\\end{document}\n");

    fprintf(fp, "\\end{document}\n");
}

void DescribeExpression(Tree_t *Tree, FILE *fp)
{
    Operator_t root_op = (Operator_t)Tree->root->data;

    if (root_op == DIV)
        PreOrderExpression(Tree, Tree->root, fp);
    else 
        InOrderExpression(Tree, Tree->root, fp);
}

void PreOrderExpression(Tree_t *Tree, TreeNode_t *Curr, FILE *fp)
{
    switch(Curr->type)
    {
        case OP:
        {
            switch((Operator_t)Curr->data)
            {
                case ADD:
                    InOrderExpression(Tree, Curr, fp);
                return;
                
                case SUB:
                    InOrderExpression(Tree, Curr, fp);
                return;

                case MUL:
                    InOrderExpression(Tree, Curr, fp);
                return;

                case DIV:
                {
                    fprintf(fp, "\\frac");
                    if (Curr->left)
                    {
                        fprintf(fp, "{");
                        PreOrderExpression(Tree, Curr->left, fp);
                        fprintf(fp, "}");
                    }
                    if (Curr->right)
                    {
                        fprintf(fp, "{");
                        PreOrderExpression(Tree, Curr->right, fp);
                        fprintf(fp, "}");
                    }
                }
                return;

                case SIN:
                {
                    fprintf(fp, "\\sin");
                    if (Curr->left)
                    {
                        fprintf(fp, "(");
                        PreOrderExpression(Tree, Curr->left, fp);
                        fprintf(fp, ")");
                    } 
                }
                return;

                case COS:
                {
                    fprintf(fp, "\\cos");
                    if (Curr->left)
                    {
                        fprintf(fp, "(");
                        PreOrderExpression(Tree, Curr->left, fp);
                        fprintf(fp, ")");
                    } 
                }
                return;

                case LOG:
                {
                    fprintf(fp, "\\log");
                    if (Curr->left)
                    {
                        fprintf(fp, "(");
                        PreOrderExpression(Tree, Curr->left, fp);
                        fprintf(fp, ")");
                    } 
                }
                return;

                default:
                    ;
            }
        }
        return;

        case NUM:
            fprintf(fp, "{%lg}", Curr->data);
        return;

        case VAR:
            fprintf(fp, "{%c}", 'x' + (int)Curr->data);
        return;

        default:
            ;
    }
}

void InOrderExpression(Tree_t *Tree, TreeNode_t *Curr, FILE *fp)
{
    switch(Curr->type)
    {
        case OP:
        {
            switch((Operator_t)Curr->data)
            {
                case ADD:
                {
                    if (Curr->left)
                        InOrderExpression(Tree, Curr->left, fp);
                    fprintf(fp, "+");
                    if (Curr->right)
                        InOrderExpression(Tree, Curr->right, fp);
                }
                return;

                case SUB:
                {
                    if (Curr->left)
                        InOrderExpression(Tree, Curr->left, fp);
                    fprintf(fp, "-");
                    if (Curr->right)
                        InOrderExpression(Tree, Curr->right, fp);
                }
                return;

                case MUL:
                {
                    if (Curr->left)
                        InOrderExpression(Tree, Curr->left, fp);
                    fprintf(fp, "\\cdot");
                    if (Curr->right)
                        InOrderExpression(Tree, Curr->right, fp);
                }
                return;

                case DIV:
                    PreOrderExpression(Tree, Curr, fp);
                return;

                case SIN:
                    PreOrderExpression(Tree, Curr, fp);
                return;

                case COS:
                    PreOrderExpression(Tree, Curr, fp);
                return;

                case LOG:
                    PreOrderExpression(Tree, Curr, fp);
                return;

                default:
                    ;
            }
        }
        return;

        case NUM:
            fprintf(fp, "{%lg}", Curr->data);
        return;

        case VAR:
            fprintf(fp, "{%c}", 'x' + (int)Curr->data);
        return;

        default:
            ;
    }
}

ssize_t GetFileSize(const char *file_name)
{
    struct stat stat_buf = {};
    stat(file_name, &stat_buf);

    return stat_buf.st_size;
}

size_t ReadFromFile(char **buf, const char *file_name)
{   
    FILE *fp = fopen(file_name, "r");
    if (!fp)
        return 0;

    ssize_t file_size = GetFileSize(file_name);

    *buf = (char *)calloc((size_t)file_size, sizeof(char));
    if (!buf)
        return 0;

    size_t count = fread(*buf, sizeof(char), (size_t)file_size, fp);

    fclose(fp);

    return count;
}

TreeNode_t *BuildFromPreOrder(Tree_t *Tree, TreeNode_t *Curr, char **buf)
{
    char data[DATA_SIZE] = {};
    size_t len = 0;
    int num_elems = 0;
    int flag = 0;

    if (**buf == '(')
    {
        Curr = NewNode(UNKNOWN_TYPE, POIZON_VAL, NULL, NULL);
        if (!Tree->root)
            Tree->root = Curr;
        (*buf)++; 
    }

    num_elems = sscanf(*buf, "%[^()]", data);
    if (num_elems == 1)
    {
        switch((*buf)[0])
        {
            case '+':
            {
                Curr->type = OP;
                Curr->data = ADD;
                flag = 1;
            }
            break;

            case '-':
            {
                Curr->type = OP;
                Curr->data = SUB;
                flag = 1;
            }
            break;

            case '*':
            {
                Curr->type = OP;
                Curr->data = MUL;
                flag = 1;
            }
            break;

            case '/':
            {
                Curr->type = OP;
                Curr->data = DIV;
                flag = 1;
            }
            break;

            default:
                ;
        }

        if (isalpha(data[0]) && flag == 0)
        {
            if (strcmp(data, "sin") == 0)
            {
                Curr->type = OP;
                Curr->data = SIN;
                flag = 1;
            }
            else if (strcmp(data, "cos") == 0)
            {
                Curr->type = OP;
                Curr->data = COS;
                flag = 1;
            }
            else if (strcmp(data, "log") == 0)
            {
                Curr->type = OP;
                Curr->data = LOG;
                flag = 1;
            }
        }

        if (isalpha(data[0]) && flag == 0)
        {
            Curr->type = VAR;
            Curr->data = data[0] - 'x';
            flag = 1;
        }

        if (flag == 0)
        {
            char *end = data;
            TreeElem_t temp_val = strtod(data, &end);
            if (end == data)
                return NULL;
            Curr->type = NUM;
            Curr->data = temp_val;
        }

        len = strlen(data);
        (*buf) += len;
    }
    else
    {
        (*buf)++;
        return NULL;
    }

    if (**buf == '(')
    {
        Curr->left = BuildFromPreOrder(Tree, Curr->left, buf);
        if (Curr->left)
            Curr->left->parent = Curr;
    }

    if (**buf == ')')
    {
        (*buf)++;
        return Curr;
    }

    if (**buf == '(')
    {
        Curr->right = BuildFromPreOrder(Tree, Curr->right, buf);
        if (Curr->right)
            Curr->right->parent = Curr;
    }

    if (**buf == ')')
    {
        (*buf)++;
        return Curr;
    }

    return Curr;
}

void SlowPrintf(const char *fmt, ...)
{
    const int MAX_STR = 256;
    const int SLEEP_TIME = 10000;

    char buf[MAX_STR] = {};
    va_list args = {};

    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
    
    char *ptr = buf;

    for (; *ptr; ptr++)
    {
        printf("%c", *ptr);
        fflush(stdout);

        usleep(SLEEP_TIME);
    }
}