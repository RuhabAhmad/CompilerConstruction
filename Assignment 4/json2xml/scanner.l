%option noyywrap

%{
#include "parser.tab.h"
#include "ast.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern int yylineno;
int yycolumn = 1;

static void update_position(const char *text) {
    for (int i = 0; text[i]; i++) {
        if (text[i] == '\n') {
            yylineno++;
            yycolumn = 1;
        } else {
            yycolumn++;
        }
    }
}

static int hex_value(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
    if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
    return -1;
}

static void append_utf8(char **out, int codepoint) {
    if (codepoint <= 0x7F) {
        *(*out)++ = (char)codepoint;
    } else if (codepoint <= 0x7FF) {
        *(*out)++ = (char)(0xC0 | (codepoint >> 6));
        *(*out)++ = (char)(0x80 | (codepoint & 0x3F));
    } else {
        *(*out)++ = (char)(0xE0 | (codepoint >> 12));
        *(*out)++ = (char)(0x80 | ((codepoint >> 6) & 0x3F));
        *(*out)++ = (char)(0x80 | (codepoint & 0x3F));
    }
}

static char *unescape_json_string(const char *text) {
    size_t len = strlen(text);
    char *buf = (char *)malloc(len * 4 + 1);
    char *out = buf;

    for (size_t i = 1; i + 1 < len; i++) {
        if (text[i] == '\\') {
            i++;
            if (text[i] == '"') *out++ = '"';
            else if (text[i] == '\\') *out++ = '\\';
            else if (text[i] == '/') *out++ = '/';
            else if (text[i] == 'b') *out++ = '\b';
            else if (text[i] == 'f') *out++ = '\f';
            else if (text[i] == 'n') *out++ = '\n';
            else if (text[i] == 'r') *out++ = '\r';
            else if (text[i] == 't') *out++ = '\t';
            else if (text[i] == 'u') {
                if (i + 4 >= len) {
                    printf("Error: invalid unicode escape at %d:%d\n", yylineno, yycolumn);
                    exit(1);
                }
                int codepoint = 0;
                for (int k = 0; k < 4; k++) {
                    int hv = hex_value(text[i + 1 + k]);
                    if (hv < 0) {
                        printf("Error: invalid unicode escape at %d:%d\n", yylineno, yycolumn);
                        exit(1);
                    }
                    codepoint = (codepoint << 4) | hv;
                }
                append_utf8(&out, codepoint);
                i += 4;
            } else {
                printf("Error: invalid escape sequence at %d:%d\n", yylineno, yycolumn);
                exit(1);
            }
        } else {
            *out++ = text[i];
        }
    }
    *out = '\0';
    return buf;
}
%}

%%

[ \t\n]+      { update_position(yytext); }

"true"        { update_position(yytext); return TRUE; }
"false"       { update_position(yytext); return FALSE; }
"null"        { update_position(yytext); return NULLTOK; }

\"([^\\\"]|\\.)*\" {
    update_position(yytext);
    yylval.str = unescape_json_string(yytext);
    return STRING;
}

-?(0|[1-9][0-9]*)(\.[0-9]+)?([eE][+-]?[0-9]+)? {
    update_position(yytext);
    yylval.str = strdup(yytext);
    return NUMBER;
}

"{"           { update_position(yytext); return '{'; }
"}"           { update_position(yytext); return '}'; }
"["           { update_position(yytext); return '['; }
"]"           { update_position(yytext); return ']'; }
":"           { update_position(yytext); return ':'; }
","           { update_position(yytext); return ','; }

. {
    update_position(yytext);
    printf("Error: invalid character '%s' at %d:%d\n", yytext, yylineno, yycolumn);
    exit(1);
}

%%