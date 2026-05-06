#include <stdio.h>
#include <string.h>
#include "first_follow.h"

Set firstSets[MAX_NT];
Set followSets[MAX_NT];

int setContains(Set *s, const char *item) {
	int i;
	for (i = 0; i < s->count; i++) {
		if (strcmp(s->items[i], item) == 0) {
			return 1;
		}
	}
	return 0;
}

int setAdd(Set *s, const char *item) {
	if (setContains(s, item)) {
		return 0;
	}

	if (s->count >= MAX_SYMBOLS) {
		die("Set capacity exceeded");
	}

	strcpy(s->items[s->count], item);
	s->count++;
	return 1;
}

int setAddAll(Set *dst, Set *src) {
	int changed = 0;
	int i;

	for (i = 0; i < src->count; i++) {
		if (setAdd(dst, src->items[i])) {
			changed = 1;
		}
	}
	return changed;
}

int firstOfSequenceTokens(char tokens[MAX_TOKENS][MAX_NAME], int tc, Set *out, int *hasEpsilon) {
	int i;
	int allEps = 1;

	*hasEpsilon = 0;
	if (tc == 0) {
		*hasEpsilon = 1;
		return 1;
	}

	for (i = 0; i < tc; i++) {
		int ntIdx;
		int k;

		if (strcmp(tokens[i], EPSILON) == 0) {
			continue;
		}

		ntIdx = findRuleIndex(tokens[i]);
		if (ntIdx == -1) {
			setAdd(out, tokens[i]);
			allEps = 0;
			break;
		}

		for (k = 0; k < firstSets[ntIdx].count; k++) {
			if (strcmp(firstSets[ntIdx].items[k], EPSILON) != 0) {
				setAdd(out, firstSets[ntIdx].items[k]);
			}
		}

		if (!setContains(&firstSets[ntIdx], EPSILON)) {
			allEps = 0;
			break;
		}
	}

	if (allEps) {
		*hasEpsilon = 1;
	}
	return allEps;
}

void computeFirstSets(void) {
	int i;
	int changed;

	for (i = 0; i < ruleCount; i++) {
		firstSets[i].count = 0;
		firstSets[i].hasDollar = 0;
	}

	do {
		changed = 0;

		for (i = 0; i < ruleCount; i++) {
			Rule *r = &grammar[i];
			int j;

			for (j = 0; j < r->altCount; j++) {
				char tokens[MAX_TOKENS][MAX_NAME];
				int tc = tokenize(r->alts[j], tokens);
				Set temp;
				int hasEpsilon = 0;

				temp.count = 0;
				temp.hasDollar = 0;

				firstOfSequenceTokens(tokens, tc, &temp, &hasEpsilon);
				if (setAddAll(&firstSets[i], &temp)) {
					changed = 1;
				}
				if (hasEpsilon && setAdd(&firstSets[i], EPSILON)) {
					changed = 1;
				}
			}
		}
	} while (changed);
}

void computeFollowSets(void) {
	int i;
	int changed;

	for (i = 0; i < ruleCount; i++) {
		followSets[i].count = 0;
		followSets[i].hasDollar = 0;
	}

	followSets[0].hasDollar = 1;

	do {
		changed = 0;

		for (i = 0; i < ruleCount; i++) {
			Rule *A = &grammar[i];
			int p;

			for (p = 0; p < A->altCount; p++) {
				char tokens[MAX_TOKENS][MAX_NAME];
				int tc = tokenize(A->alts[p], tokens);
				int pos;

				for (pos = 0; pos < tc; pos++) {
					int b = findRuleIndex(tokens[pos]);
					if (b == -1) {
						continue;
					}

					{
						Set firstBeta;
						int betaHasEpsilon;
						int x;

						firstBeta.count = 0;
						firstBeta.hasDollar = 0;

						firstOfSequenceTokens(tokens + pos + 1, tc - pos - 1, &firstBeta, &betaHasEpsilon);

						for (x = 0; x < firstBeta.count; x++) {
							if (strcmp(firstBeta.items[x], EPSILON) != 0 &&
								setAdd(&followSets[b], firstBeta.items[x])) {
								changed = 1;
							}
						}

						if (betaHasEpsilon) {
							if (setAddAll(&followSets[b], &followSets[i])) {
								changed = 1;
							}
							if (followSets[i].hasDollar && !followSets[b].hasDollar) {
								followSets[b].hasDollar = 1;
								changed = 1;
							}
						}
					}
				}
			}
		}
	} while (changed);
}

void printFirstSets(void) {
	int i, j;

	printf("\nFIRST Sets (Tabular)\n");
	printf("%-20s %-60s\n", "Non-Terminal", "FIRST");
	printf("%-20s %-60s\n", "------------", "-----");

	for (i = 0; i < ruleCount; i++) {
		printf("%-20s { ", grammar[i].name);
		for (j = 0; j < firstSets[i].count; j++) {
			printf("%s", firstSets[i].items[j]);
			if (j < firstSets[i].count - 1) {
				printf(", ");
			}
		}
		printf(" }\n");
	}
}

void printFollowSets(void) {
	int i, j;

	printf("\nFOLLOW Sets (Tabular)\n");
	printf("%-20s %-60s\n", "Non-Terminal", "FOLLOW");
	printf("%-20s %-60s\n", "------------", "------");

	for (i = 0; i < ruleCount; i++) {
		printf("%-20s { ", grammar[i].name);
		for (j = 0; j < followSets[i].count; j++) {
			printf("%s", followSets[i].items[j]);
			if (j < followSets[i].count - 1 || followSets[i].hasDollar) {
				printf(", ");
			}
		}
		if (followSets[i].hasDollar) {
			printf("$");
		}
		printf(" }\n");
	}
}
