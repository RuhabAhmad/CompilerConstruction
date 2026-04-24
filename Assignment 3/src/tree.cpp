// Author: Ruhab Ahmad (23I-0559)
// Assignment 03: Bottom-Up Parser Design & Implementation
#include "tree.h"
#include <iostream>

void ParseTree::printTree(TreeNode* node, int depth) const {
    if (!node) return;
    
    for (int i = 0; i < depth; ++i) std::cout << "  ";
    std::cout << "|- " << node->label << "\n";
    
    for (TreeNode* child : node->children) {
        printTree(child, depth + 1);
    }
}