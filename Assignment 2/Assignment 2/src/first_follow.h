#ifndef FIRST_FOLLOW_H
#define FIRST_FOLLOW_H

#include "grammar.h"

extern Set firstSets[MAX_NT];
extern Set followSets[MAX_NT];

int setContains(Set *s, const char *item);
int setAdd(Set *s, const char *item);
int setAddAll(Set *dst, Set *src);
int firstOfSequenceTokens(char tokens[MAX_TOKENS][MAX_NAME], int tc, Set *out, int *hasEpsilon);

void computeFirstSets(void);
void computeFollowSets(void);
void printFirstSets(void);
void printFollowSets(void);

#endif
