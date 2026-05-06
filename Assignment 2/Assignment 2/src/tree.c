#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tree.h"

TreeNode *treeCreateNode(const char *symbol) {
	TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
	if (!node) {
		return NULL;
	}
	strncpy(node->symbol, symbol, MAX_NAME - 1);
	node->symbol[MAX_NAME - 1] = '\0';
	node->firstChild = NULL;
	node->nextSibling = NULL;
	return node;
}

void treeAddChild(TreeNode *parent, TreeNode *child) {
	TreeNode *cur;
	if (!parent || !child) {
		return;
	}

	if (!parent->firstChild) {
		parent->firstChild = child;
		return;
	}

	cur = parent->firstChild;
	while (cur->nextSibling) {
		cur = cur->nextSibling;
	}
	cur->nextSibling = child;
}

void treeFree(TreeNode *root) {
	if (!root) {
		return;
	}

	treeFree(root->firstChild);
	treeFree(root->nextSibling);
	free(root);
}

void treePrintIndented(TreeNode *root, int depth, FILE *out) {
	int i;
	TreeNode *child;

	if (!root || !out) {
		return;
	}

	for (i = 0; i < depth; i++) {
		fprintf(out, "  ");
	}
	fprintf(out, "%s\n", root->symbol);

	child = root->firstChild;
	while (child) {
		treePrintIndented(child, depth + 1, out);
		child = child->nextSibling;
	}
}
