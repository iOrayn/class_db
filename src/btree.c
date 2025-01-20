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

TreeNode* find_min(TreeNode* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

bool id_exists(TreeNode* root, int id) {
    if (root == NULL) {
        return false; 
    }
    if (id == root->row.id) {
        return true; 
    } else if (id < root->row.id) {
        return id_exists(root->left, id);
    } else {
        return id_exists(root->right, id);
    }
}


bool insert_row(Table* table, Row* row) {
    if (id_exists(table->root, row->id)) {
        printf("Error: ID %d already exists.\n", row->id);
        return false; 
    }

    if (table->root == NULL) {
        table->root = create_node(row);
        table->num_rows++;
        return true;
    }

    TreeNode* current = table->root;
    while (true) {
        if (row->id < current->row.id) {
            if (current->left == NULL) {
                current->left = create_node(row);
                table->num_rows++;
                return true; 
            }
            current = current->left;
        } else {
            if (current->right == NULL) {
                current->right = create_node(row);
                table->num_rows++;
                return true;
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

Row* find_by_id(TreeNode* root, int id) {
    if (root == NULL) {
        return NULL;
    }

    if (id == root->row.id) {
        return &root->row; 
    } else if (id < root->row.id) {
        return find_by_id(root->left, id);
    } else {
        return find_by_id(root->right, id);
    }
}

TreeNode* delete_by_id(TreeNode* root, int id) {
    if (root == NULL) {
        printf("Error: ID %d not found.\n", id);
        return root;
    }
    
    if (id < root->row.id) {
        root->left = delete_by_id(root->left, id);
    } else if (id > root->row.id) {
        root->right = delete_by_id(root->right, id);
    } else {
        
        if (root->left == NULL) {
            TreeNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            TreeNode* temp = root->left;
            free(root);
            return temp;
        }

        
        TreeNode* temp = find_min(root->right);
        root->row = temp->row; 
        root->right = delete_by_id(root->right, temp->row.id); 
    }
    return root;
}
