#ifndef AST_H
#define AST_H
#define NODE_ITEM 100

typedef enum {
    NODE_OBJECT,
    NODE_ARRAY,
    NODE_STRING,
    NODE_NUMBER,
    NODE_BOOL,
    NODE_NULL,
    NODE_PAIR
} NodeType;

typedef struct ASTNode {
    NodeType type;

    char *key;   // for pairs
    char *value; // for string/number/bool

    struct ASTNode *child;   // first child
    struct ASTNode *next;    // sibling
} ASTNode;

// constructors
ASTNode* create_node(NodeType type);
ASTNode* create_string(char *val);
ASTNode* create_number(char *val);
ASTNode* create_bool(char *val);
ASTNode* create_null();
ASTNode* create_pair(char *key, ASTNode *value);

// utility
void add_child(ASTNode *parent, ASTNode *child);
void print_ast(ASTNode *node, int indent);

#endif