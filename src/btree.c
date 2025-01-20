#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "btree.h"

TreeNode* create_node(Row* row) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node == NULL) {
        printf("Error: Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    node->row = *row;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void insert_row(Table* table, Row* row) {
    if (table->root == NULL) {
        table->root = create_node(row);
        table->num_rows++;
        return;
    }

    TreeNode* current = table->root;
    while (true) {
        if (row->id < current->row.id) {
            if (current->left == NULL) {
                current->left = create_node(row);
                table->num_rows++;
                return;
            }
            current = current->left;
        } else {
            if (current->right == NULL) {
                current->right = create_node(row);
                table->num_rows++;
                return;
            }
            current = current->right;
        }
    }
}

void print_in_order(TreeNode* node) {
    if (node == NULL) {
        return;
    }
    print_in_order(node->left);
    printf("(%d, %s, %s)\n", node->row.id, node->row.username, node->row.email);
    print_in_order(node->right);
}

void btree_select(Table* table) {
    if (table->root == NULL) {
        printf("Table is empty.\n");
        return;
    }
    print_in_order(table->root);
}
