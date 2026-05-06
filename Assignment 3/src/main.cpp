// Author: Ruhab Ahmad (23I-0559)
// Assignment 03: Bottom-Up Parser Design & Implementation

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "grammar.h"
#include "slr_parser.h"
#include "lr1_parser.h"

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: ./parser_exec [--slr | --lr1] <grammar_file> <input_file>\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string grammarFile = argv[2];
    std::string inputFile = argv[3];

    Grammar g;
    g.loadFromFile(grammarFile);
    g.augmentGrammar();
    g.printAugmentedGrammar();

    // Read input tokens from file instead of hardcoded input
    std::vector<std::string> inputTokens;
    std::ifstream fin(inputFile);

    if (!fin.is_open()) {
        std::cerr << "Error opening input file: " << inputFile << "\n";
        return 1;
    }

    std::string token;
    while (fin >> token) {
        inputTokens.push_back(token);
    }

    fin.close();

    // Add end marker if not already present
    if (inputTokens.empty() || inputTokens.back() != "$") {
        inputTokens.push_back("$");
    }

    if (mode == "--slr") {
        std::cout << "\n--- Building SLR(1) Parser ---\n";
        SLRParser slr(g);
        slr.buildTable();
        slr.reportConflicts();
        slr.parse(inputTokens);

    } else if (mode == "--lr1") {
        std::cout << "\n--- Building LR(1) Parser ---\n";
        LR1Parser lr1(g);
        lr1.buildTable();
        lr1.reportConflicts();
        lr1.parse(inputTokens);

    } else {
        std::cerr << "Invalid mode. Use --slr or --lr1\n";
    }

    return 0;
}