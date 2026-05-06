#ifndef STACK_H
#define STACK_H

#include "grammar.h"

typedef struct {
	char items[MAX_SYMBOLS][MAX_NAME];
	int top;
} SymbolStack;

void stackInit(SymbolStack *s);
int stackIsEmpty(const SymbolStack *s);
int stackPush(SymbolStack *s, const char *item);
int stackPop(SymbolStack *s, char *out);
int stackTop(const SymbolStack *s, char *out);

#endif
