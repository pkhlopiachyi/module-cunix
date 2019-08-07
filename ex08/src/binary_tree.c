#include "../include/binary_tree.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

node_t *allocnode(){
    node_t *node = malloc(sizeof(node_t));
    node -> key = NULL;
    node -> data = NULL;
    node -> left = NULL;
    node -> right = NULL;
    
    return node;
}

node_t *insert(node_t *root, char *key, void *data){
    if(root == NULL){
        root = malloc(sizeof(node_t));
        root -> key = key;
        root -> data = data;
        root -> left = NULL;
        root -> right = NULL;
        return root;
    }
    if(strcmp(root -> key, key)>0){
        root -> left = insert(root -> left, key, data);
    }
    else{
        root -> right = insert(root -> left, key, data);
    }
    
    return root;
}

void print_node(node_t *node){
    if(node == NULL){
        return;
    }
    print_node(node -> left);
    print_node(node -> right);
    printf("%s\n", (char *)node -> key);
}

void visit_tree(node_t *node, void (*fp)(node_t *root)){
    if(node == NULL){
        return;
    }
    fp(node);
    visit_tree(node -> left, fp);
    visit_tree(node -> right, fp);
}

void destroy_tree(node_t *node, void (*fdestroy)(node_t *root)){
    if(node == NULL){
        return;
    }
    destroy_tree(node -> left, fdestroy);
    destroy_tree(node -> right, fdestroy);
    fdestroy(node);
}
