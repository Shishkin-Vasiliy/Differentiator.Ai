#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdarg.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

typedef double TreeElem_t;

const TreeElem_t POIZON_VAL = 0x0D01B0EB;

const int DATA_SIZE = 64;
const int MAX_VARS = 3;
const int MAX_PROBLEMS = 16;
const TreeElem_t ERROR_VAL = 0xFFFFFFFF;
const TreeElem_t TEMP_VAL = 0x00BADA55;

enum Type_t {
    OP,
    NUM,
    VAR,
    UNKNOWN_TYPE
};

enum Operator_t {
    ADD,
    SUB,
    MUL,
    DIV,
    POW,
    SIN,
    COS,
    LOG,
    OP_COUNT
};

enum IsOptimized_t {
    TREE_IS_OPTIMIZED,
    TREE_IS_CHANGING
};

struct TreeNode_t {
    Type_t type;
    TreeElem_t data;
    TreeNode_t *left;
    TreeNode_t *right;
};

struct Tree_t {
    TreeNode_t *root;
    TreeElem_t VarList[MAX_VARS];
};

enum TreeErr_t {
    TREE_OK,
    CALLOC_FAIL,
    BAD_PTR,
    FILE_OPEN_FAIL,
    TREE_EMPTY,
    TREE_CORRUPTED
};

struct TreeLogCtx_t {
    char **pict_index;
    size_t num_picts;
};

struct LatexLogCtx_t {
    char **pdf_index;
    size_t num_pdfs;
};



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

struct Flags_t {
    int expr_calc_flag;
    int expr_diff_flag;
    int derive_calc_flag;
};

#endif