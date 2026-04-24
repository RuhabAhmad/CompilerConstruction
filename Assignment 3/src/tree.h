// Author: Ruhab Ahmad (23I-0559)
// Assignment 03: Bottom-Up Parser Design & Implementation
#ifndef TREE_H
#define TREE_H

#include <string>
#include <vector>

struct TreeNode {
    std::string label;
    std::vector<TreeNode*> children;
    
    TreeNode(const std::string& l) : label(l) {}
    ~TreeNode() {
        for (auto child : children) delete child;
    }
};

class ParseTree {
public:
    TreeNode* root;
    
    ParseTree() : root(nullptr) {}
    ~ParseTree() { delete root; }
    
    void printTree(TreeNode* node, int depth = 0) const;
};

#endif