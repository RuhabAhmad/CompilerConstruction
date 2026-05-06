%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "xml.h"

ASTNode *root;
int yylex();
void yyerror(const char *s);
extern int yylineno;
extern int yycolumn;
%}

%code requires {
    #include "ast.h"
}

%union {
    char *str;
    ASTNode *node;
}

%token <str> STRING NUMBER
%token TRUE FALSE NULLTOK

%type <node> value object array pair members elements

%%

json:
    value { root = $1; }
;

value:
      object { $$ = $1; }
    | array { $$ = $1; }
    | STRING { $$ = create_string($1); }
    | NUMBER { $$ = create_number($1); }
    | TRUE   { $$ = create_bool("true"); }
    | FALSE  { $$ = create_bool("false"); }
    | NULLTOK { $$ = create_null(); }
;

object:
    '{' '}' {
        $$ = create_node(NODE_OBJECT);
    }
    | '{' members '}' {
        $$ = create_node(NODE_OBJECT);
        $$->child = $2;
    }
;

members:
      pair { $$ = $1; }
    | members ',' pair {
        ASTNode *temp = $1;
        while (temp->next) temp = temp->next;
        temp->next = $3;
        $$ = $1;
    }
;

pair:
    STRING ':' value {
        $$ = create_pair($1, $3);
    }
;

array:
    '[' ']' {
        $$ = create_node(NODE_ARRAY);
    }
    | '[' elements ']' {
        $$ = create_node(NODE_ARRAY);
        $$->child = $2->child;
    }
;

elements:
      value {
          $$ = create_node(NODE_ARRAY);
          add_child($$, $1);
      }
    | elements ',' value {
          add_child($1, $3);
          $$ = $1;
      }
;

%%

void yyerror(const char *s) {
    printf("Error: %s at %d:%d\n", s, yylineno, yycolumn);
    exit(1);
}