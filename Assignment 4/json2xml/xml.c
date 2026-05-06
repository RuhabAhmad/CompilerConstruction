#include <stdio.h>
#include "xml.h"

void print_indent(int n) {
    for (int i = 0; i < n; i++) printf("  ");
}

void xml_escape(char *str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] == '&') printf("&amp;");
        else if (str[i] == '<') printf("&lt;");
        else if (str[i] == '>') printf("&gt;");
        else printf("%c", str[i]);
    }
}

void generate_xml_node(ASTNode *node, int indent) {
    while (node) {
        if (node->type == NODE_PAIR) {
            print_indent(indent);
            printf("<%s>", node->key);

            ASTNode *val = node->child;

            if (val->type == NODE_NULL) {
                printf("</%s>\n", node->key);
            }
            else if (val->type == NODE_STRING || val->type == NODE_NUMBER || val->type == NODE_BOOL) {
                xml_escape(val->value);
                printf("</%s>\n", node->key);
            }
            else if (val->type == NODE_ARRAY || val->type == NODE_OBJECT) {
                printf("\n");
                generate_xml_node(val, indent + 1);
                print_indent(indent);
                printf("</%s>\n", node->key);
            }
            else {
                printf("</%s>\n", node->key);
            }
        }
        else if (node->type == NODE_OBJECT) {
            generate_xml_node(node->child, indent);
        }
        else if (node->type == NODE_ARRAY) {
            ASTNode *child = node->child;
            while (child) {
                print_indent(indent);
                printf("<item>");
                if (child->type == NODE_STRING || child->type == NODE_NUMBER || child->type == NODE_BOOL) {
                    xml_escape(child->value);
                } else if (child->type == NODE_NULL) {
                    /* keep empty item */
                } else {
                    printf("\n");
                    generate_xml_node(child, indent + 1);
                    print_indent(indent);
                }
                printf("</item>\n");
                child = child->next;
            }
        }
        node = node->next;
    }
}

void generate_xml(ASTNode *root) {
    printf("<root>");
    if (root->type == NODE_OBJECT) {
        printf("\n");
        generate_xml_node(root->child, 1);
        printf("</root>\n");
    } else if (root->type == NODE_ARRAY) {
        printf("\n");
        generate_xml_node(root, 1);
        printf("</root>\n");
    } else if (root->type == NODE_STRING || root->type == NODE_NUMBER || root->type == NODE_BOOL) {
        xml_escape(root->value);
        printf("</root>\n");
    } else if (root->type == NODE_NULL) {
        printf("</root>\n");
    } else {
        printf("</root>\n");
    }
}