#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include "grammar.h"

typedef struct TreeNode {
	char symbol[MAX_NAME];
	struct TreeNode *firstChild;
	struct TreeNode *nextSibling;
} TreeNode;

TreeNode *treeCreateNode(const char *symbol);
void treeAddChild(TreeNode *parent, TreeNode *child);
void treePrintIndented(TreeNode *root, int depth, FILE *out);
void treeFree(TreeNode *root);

#endif
