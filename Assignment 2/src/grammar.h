#ifndef GRAMMAR_H
#define GRAMMAR_H

#define MAX_NT 128
#define MAX_ALTS 256
#define MAX_SYMBOLS 256
#define MAX_LINE 1024
#define MAX_NAME 64
#define MAX_RHS 512
#define MAX_TOKENS 128
#define EPSILON "epsilon"

typedef struct {
	char name[MAX_NAME];
	char alts[MAX_ALTS][MAX_RHS];
	int altCount;
} Rule;

typedef struct {
	char items[MAX_SYMBOLS][MAX_NAME];
	int count;
	int hasDollar;
} Set;

extern Rule grammar[MAX_NT];
extern int ruleCount;

void die(const char *msg);

int isEpsilonToken(const char *tok);
int findRuleIndex(const char *name);
int tokenize(const char *rhs, char out[MAX_TOKENS][MAX_NAME]);

void parseCfg(const char *filePath);
void printGrammar(const char *title);
void leftFactorAll(void);
void removeLeftRecursionAll(void);

#endif
