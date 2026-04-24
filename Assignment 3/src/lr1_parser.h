// Author: Ruhab Ahmad (23I-0559)
// Assignment 03: Bottom-Up Parser Design & Implementation
#ifndef LR1_PARSER_H
#define LR1_PARSER_H

#include "slr_parser.h" // Reuse Action enums and structs

class LR1Parser {
private:
    Grammar g;
    ItemCollection items;
    std::map<std::pair<int, std::string>, Action> action_table;
    std::map<std::pair<int, std::string>, int> goto_table;

public:
    LR1Parser(const Grammar& grammar);
    void buildTable(); // Uses exact lookaheads instead of FOLLOW
    bool parse(const std::vector<std::string>& inputTokens);
    void reportConflicts();
};

#endif