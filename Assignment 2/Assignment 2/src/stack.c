#include <string.h>
#include "stack.h"

void stackInit(SymbolStack *s) {
	s->top = -1;
}

int stackIsEmpty(const SymbolStack *s) {
	return s->top < 0;
}

int stackPush(SymbolStack *s, const char *item) {
	if (s->top >= MAX_SYMBOLS - 1) {
		return 0;
	}
	s->top++;
	strncpy(s->items[s->top], item, MAX_NAME - 1);
	s->items[s->top][MAX_NAME - 1] = '\0';
	return 1;
}

int stackPop(SymbolStack *s, char *out) {
	if (stackIsEmpty(s)) {
		return 0;
	}
	strcpy(out, s->items[s->top]);
	s->top--;
	return 1;
}

int stackTop(const SymbolStack *s, char *out) {
	if (stackIsEmpty(s)) {
		return 0;
	}
	strcpy(out, s->items[s->top]);
	return 1;
}
