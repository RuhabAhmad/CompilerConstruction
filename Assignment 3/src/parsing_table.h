// Author: Ruhab Ahmad (23I-0559)
// Assignment 03: Bottom-Up Parser Design & Implementation
#ifndef PARSING_TABLE_H
#define PARSING_TABLE_H

#include "slr_parser.h" // Action definitions
#include <map>
#include <string>
#include <vector>

class ParsingTableFormatter {
public:
    static void printTable(
        const std::map<std::pair<int, std::string>, Action>& action_table,
        const std::map<std::pair<int, std::string>, int>& goto_table,
        const std::vector<std::string>& terminals,
        const std::vector<std::string>& non_terminals,
        int num_states,
        const std::string& outputFile
    );
};

#endif