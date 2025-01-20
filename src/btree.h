#ifndef BTREE_H
#define BTREE_H

#include "repl.h"

typedef struct TreeNode {
    Row row;                  
    struct TreeNode* left;    
    struct TreeNode* right;   
} TreeNode;


TreeNode* create_node(Row* row);
void insert_row(Table* table, Row* row);
void print_in_order(TreeNode* node);
void btree_select(Table* table);

#endif
