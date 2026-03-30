#include <stdio.h>
#include "grammar.h"
#include "first_follow.h"
#include "parser.h"
#include "error_handler.h"

int main(int argc, char **argv) {
    const char *cfgPath = "input/grammar1.txt";
    const char *inputPath = "input/input_valid.txt";
    const char *traceOutPath = "output/parsing_trace1.txt";
    const char *treeOutPath = "output/parse_trees.txt";
    int isLL1;

    if (argc > 1) {
        cfgPath = argv[1];
    }
    if (argc > 2) {
        inputPath = argv[2];
    }
    if (argc > 3) {
        traceOutPath = argv[3];
    }
    if (argc > 4) {
        treeOutPath = argv[4];
    }

    parseCfg(cfgPath);
    if (ruleCount == 0) {
        die("No valid grammar rules found");
    }

    printGrammar("Original Grammar (Task 1.1)");

    leftFactorAll();
    printGrammar("After Left Factoring (Task 1.2)");

    removeLeftRecursionAll();
    printGrammar("After Left Recursion Removal (Task 1.3)");

    computeFirstSets();
    printFirstSets();

    computeFollowSets();
    printFollowSets();

    isLL1 = buildLL1Table();
    printLL1Table(isLL1);

    if (!isLL1) {
        reportWarning("Grammar is not LL(1). Stack parsing run may be ambiguous.");
    }

    parseInputFile(inputPath, traceOutPath, treeOutPath);

    return 0;
}
