#include "Common.h"
#include "Differentiator.h"
#include "DSL.h"
#include "Dump.h"
#include "Tools.h"
#include "Tree.h"

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

// todo массив из строк со всеми названиями функций

static const char *OP_NAMES[OP_COUNT] = {
    "+",
    "-",
    "*",
    "/",
    "^",
    "sin",
    "cos",
    "log"
};

TreeNode_t *BuildFromPreOrder(Tree_t *Tree, TreeNode_t *Curr, char **buf)
{
    char data[DATA_SIZE] = {};
    size_t len           = 0;
    int num_elems        = 0;
    int flag             = 0;

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
        for (size_t i = 0; i < OP_COUNT; i++)
        {
            if (strcmp(data, OP_NAMES[i]) == 0)
            {
                Curr->type = OP;
                Curr->data = i;
                flag = 1;
                break;
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
        Curr->left = BuildFromPreOrder(Tree, Curr->left, buf);

    if (**buf == ')')
    {
        (*buf)++;
        return Curr;
    }

    if (**buf == '(')
        Curr->right = BuildFromPreOrder(Tree, Curr->right, buf);

    if (**buf == ')')
    {
        (*buf)++;
        return Curr;
    }

    return Curr;
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