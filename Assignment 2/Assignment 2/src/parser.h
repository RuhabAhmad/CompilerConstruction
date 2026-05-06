#ifndef PARSER_H
#define PARSER_H

#include "grammar.h"

int buildLL1Table(void);
void printLL1Table(int isLL1);
const char *parserLookupProduction(const char *nonTerminal, const char *terminal);
int parserCollectExpected(const char *nonTerminal, char outExpected[][MAX_NAME], int maxExpected);
void parseInputFile(const char *inputPath, const char *traceOutPath, const char *treeOutPath);

#endif
