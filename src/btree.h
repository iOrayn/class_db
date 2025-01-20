#ifndef BTREE_H
#define BTREE_H

#include "repl.h"

typedef struct TreeNode {
    Row row;                  
    struct TreeNode* left;    
    struct TreeNode* right;   
} TreeNode;


TreeNode* create_node(Row* row);
bool insert_row(Table* table, Row* row);
void print_in_order(TreeNode* node);
void btree_select(Table* table);
Row* find_by_id(TreeNode* root, int id);
bool id_exists(TreeNode* root, int id);
TreeNode* delete_by_id(TreeNode* root, int id);

#endif
