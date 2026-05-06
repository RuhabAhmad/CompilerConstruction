#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "xml.h"

extern ASTNode *root;
int yyparse();

int main(int argc, char **argv) {
    if (yyparse() == 0) {
        if (argc > 1 && strcmp(argv[1], "--ast") == 0) {
            print_ast(root, 0);
        }
        generate_xml(root);
    }
    return 0;
}

// ./json2xml --ast < tests/test2.json