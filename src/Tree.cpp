#include "Tree.h"

static TreeElem_t VarList[MAX_VARS] = {
    0,
    5
};

static int CountVars = 0;

TreeErr_t PreOrderCtor(Tree_t *Tree, const char *file_name)
{
    char *buf = NULL;
    ReadFromFile(&buf, file_name);
    
    char *temp = buf;
    BuildFromPreOrder(Tree, Tree->root, &temp);

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

TreeNode_t *_MUL(TreeNode_t *left, TreeNode_t *right) 
{ 
    return NewNode(OP, MUL, left, right); 
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

int TreeLogInit(LogCtx_t *LogCtx)
{
    const int MAX_PICTS = 16;
    int DirNotFound = system("[ -d TreeLog ]");

    if (DirNotFound)
    {
        int err = system("mkdir TreeLog");
        if (err)
        {
            return EOF;
        }
    }
    else 
    {
        system("rm -f TreeLog/*");
    }

    LogCtx->pict_index = (char **)calloc(MAX_PICTS, sizeof(char *));
    if (!LogCtx->pict_index)
        return EOF;

    return 0;
}

int TreeLogDeinit(LogCtx_t *LogCtx)
{
    for (size_t i = 0; i < LogCtx->num_picts; i++)
        free(LogCtx->pict_index[i]);
    free(LogCtx->pict_index);

    return 0;
}

int TreeDump(Tree_t *Tree, LogCtx_t *LogCtx)
{
    const int PATH_SIZE    = 30;
    const int COMMAND_SIZE = 80;

    char dot_path[PATH_SIZE]   = {};
    char png_path[PATH_SIZE]   = {};
    char command[COMMAND_SIZE] = {};

    size_t num_picts = LogCtx->num_picts;

    SlowPrintf("Dump processing...\n");

    snprintf(dot_path, PATH_SIZE, "TreeLog/pict_%lu.dot", num_picts);

    FILE *dot_fp = fopen(dot_path, "w");
    if (!dot_fp)
        return EOF;

    DescribeTree(Tree, dot_fp);
    fclose(dot_fp);

    snprintf(command, COMMAND_SIZE, "dot %s -Tpng -o TreeLog/pict_%lu.png", dot_path, num_picts);
    system(command);

    snprintf(png_path, PATH_SIZE, "pict_%lu.png", num_picts);

    char *temp = strdup(png_path);
    if (!temp)
        return EOF;

    LogCtx->pict_index[num_picts] = temp;
    LogCtx->num_picts++;

    SlowPrintf("Dump complete!\n");

    return 0;
}

void DescribeTree(Tree_t *Tree, FILE *fp)
{
    if (!fp)
        return;

    fprintf(fp, "digraph {\n");
    fprintf(fp, "bgcolor=\"#FFE4E1\";\n");
    fprintf(fp, "rankdir=TB;\n");      
    fprintf(fp, "nodesep=0.8;\n");
    fprintf(fp, "ranksep=0.9;\n");

    DrawTree(Tree, Tree->root, fp);

    fprintf(fp, "}\n");
}

void DrawTree(Tree_t *Tree, TreeNode_t *Curr, FILE *fp)
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
        DrawTree(Tree, Curr->left, fp);
    if (Curr->right)
        DrawTree(Tree, Curr->right, fp);

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
            }
            break;

            case '-':
            {
                Curr->type = OP;
                Curr->data = SUB;
            }
            break;

            case '*':
            {
                Curr->type = OP;
                Curr->data = MUL;
            }
            break;

            case '/':
            {
                Curr->type = OP;
                Curr->data = DIV;
            }
            break;

            default:
                ;
        }

        if (isdigit(data[0]))
        {
            Curr->type = NUM;
            Curr->data = atof(data);
        }
        else if (isalpha(data[0]))
        {
            Curr->type = VAR;
            Curr->data = CountVars++;
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