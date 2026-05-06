#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

ASTNode* create_node(NodeType type) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->key = NULL;
    node->value = NULL;
    node->child = NULL;
    node->next = NULL;
    return node;
}

ASTNode* create_string(char *val) {
    ASTNode *n = create_node(NODE_STRING);
    n->value = strdup(val);
    return n;
}

ASTNode* create_number(char *val) {
    ASTNode *n = create_node(NODE_NUMBER);
    n->value = strdup(val);
    return n;
}

ASTNode* create_bool(char *val) {
    ASTNode *n = create_node(NODE_BOOL);
    n->value = strdup(val);
    return n;
}

ASTNode* create_null() {
    return create_node(NODE_NULL);
}

ASTNode* create_pair(char *key, ASTNode *value) {
    ASTNode *n = create_node(NODE_PAIR);
    n->key = strdup(key);
    n->child = value;
    return n;
}

void add_child(ASTNode *parent, ASTNode *child) {
    if (!parent->child) {
        parent->child = child;
    } else {
        ASTNode *temp = parent->child;
        while (temp->next) temp = temp->next;
        temp->next = child;
    }
}

void print_ast(ASTNode *node, int indent) {
    while (node) {
        for (int i = 0; i < indent; i++) printf("  ");
        switch (node->type) {
            case NODE_OBJECT:
                printf("NODE_OBJECT\n");
                print_ast(node->child, indent + 1);
                break;
            case NODE_ARRAY:
                printf("NODE_ARRAY\n");
                print_ast(node->child, indent + 1);
                break;
            case NODE_PAIR:
                printf("NODE_PAIR key=%s\n", node->key ? node->key : "");
                print_ast(node->child, indent + 1);
                break;
            case NODE_STRING:
                printf("NODE_STRING value=%s\n", node->value ? node->value : "");
                break;
            case NODE_NUMBER:
                printf("NODE_NUMBER value=%s\n", node->value ? node->value : "");
                break;
            case NODE_BOOL:
                printf("NODE_BOOL value=%s\n", node->value ? node->value : "");
                break;
            case NODE_NULL:
                printf("NODE_NULL\n");
                break;
        }
        node = node->next;
    }
}