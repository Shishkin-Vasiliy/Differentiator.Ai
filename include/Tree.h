#ifndef TREE_H
#define TREE_H

#include "Common.h"

TreeErr_t PreOrderCtor(Tree_t *Tree, const char *file_name);

TreeErr_t TreeDtor(Tree_t *Tree);
TreeErr_t TreeFree(TreeNode_t *Curr);

TreeNode_t *NewNode(Type_t type, TreeElem_t data, TreeNode_t *left, TreeNode_t *right);

TreeNode_t *BuildFromPreOrder(Tree_t *Tree, TreeNode_t *Curr, char **buf);

TreeNode_t *Copy(TreeNode_t *Curr);

#endif