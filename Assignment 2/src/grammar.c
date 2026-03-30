#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "grammar.h"

Rule grammar[MAX_NT];
int ruleCount = 0;

static int generatedCounter = 1;

void die(const char *msg) {
	fprintf(stderr, "Error: %s\n", msg);
	exit(1);
}

static void trim(char *s) {
	int n = (int)strlen(s);
	int start = 0;
	int end;

	while (start < n && isspace((unsigned char)s[start])) {
		start++;
	}

	if (start == n) {
		s[0] = '\0';
		return;
	}

	end = n - 1;
	while (end >= start && isspace((unsigned char)s[end])) {
		end--;
	}

	if (start > 0) {
		memmove(s, s + start, (size_t)(end - start + 1));
	}
	s[end - start + 1] = '\0';
}

int isEpsilonToken(const char *tok) {
	return strcmp(tok, "@")==0 || strcmp(tok, EPSILON)==0;
}

static void normalizeRhs(char *rhs) {
	trim(rhs);
	if (isEpsilonToken(rhs)) {
		strcpy(rhs, EPSILON);
	}
}

static int isValidNonTerminalName(const char *name) {
	size_t len = strlen(name);
	size_t i;

	if (len < 2) {
		return 0;
	}
	if (!isupper((unsigned char)name[0])) {
		return 0;
	}

	for (i = 1; i < len; i++) {
		unsigned char c = (unsigned char)name[i];
		if (!(isalnum(c) || c == '_' || c == '\'')) {
			return 0;
		}
	}
	return 1;
}

int findRuleIndex(const char *name) {
	int i;
	for (i = 0; i < ruleCount; i++) {
		if (strcmp(grammar[i].name, name) == 0) {
			return i;
		}
	}
	return -1;
}

static int addRule(const char *name) {
	if (ruleCount >= MAX_NT) {
		die("Too many non-terminals");
	}
	strcpy(grammar[ruleCount].name, name);
	grammar[ruleCount].altCount = 0;
	ruleCount++;
	return ruleCount - 1;
}

static int getOrCreateRule(const char *name) {
	int idx = findRuleIndex(name);
	if (idx != -1) {
		return idx;
	}
	return addRule(name);
}

static int altExists(const Rule *r, const char *rhs) {
	int i;
	for (i = 0; i < r->altCount; i++) {
		if (strcmp(r->alts[i], rhs) == 0) {
			return 1;
		}
	}
	return 0;
}

static void addAltUnique(Rule *r, const char *rhs) {
	char temp[MAX_RHS];

	strncpy(temp, rhs, sizeof(temp) - 1);
	temp[sizeof(temp) - 1] = '\0';
	normalizeRhs(temp);

	if (temp[0] == '\0') {
		strcpy(temp, EPSILON);
	}
	if (altExists(r, temp)) {
		return;
	}
	if (r->altCount >= MAX_ALTS) {
		die("Too many alternatives in a rule");
	}

	strcpy(r->alts[r->altCount], temp);
	r->altCount++;
}

int tokenize(const char *rhs, char out[MAX_TOKENS][MAX_NAME]) {
	char copy[MAX_RHS];
	char *tok;
	int count = 0;

	if (strcmp(rhs, EPSILON) == 0) {
		return 0;
	}

	strncpy(copy, rhs, sizeof(copy) - 1);
	copy[sizeof(copy) - 1] = '\0';

	tok = strtok(copy, " \t\r\n");
	while (tok) {
		if (count >= MAX_TOKENS) {
			die("Too many symbols in RHS");
		}

		if (isEpsilonToken(tok)) {
			strcpy(out[count], EPSILON);
		} else {
			strncpy(out[count], tok, MAX_NAME - 1);
			out[count][MAX_NAME - 1] = '\0';
		}
		count++;
		tok = strtok(NULL, " \t\r\n");
	}

	return count;
}

static void joinTokens(char tokens[MAX_TOKENS][MAX_NAME], int start, int end, char *dst) {
	int i;

	dst[0] = '\0';
	if (start >= end) {
		strcpy(dst, EPSILON);
		return;
	}

	for (i = start; i < end; i++) {
		if ((int)(strlen(dst) + strlen(tokens[i]) + 2) >= MAX_RHS) {
			die("RHS length overflow while joining tokens");
		}
		strcat(dst, tokens[i]);
		if (i < end - 1) {
			strcat(dst, " ");
		}
	}
}

static void generatePrimeName(const char *base, char *out) {
	snprintf(out, MAX_NAME, "%s'", base);
	if (findRuleIndex(out) == -1) {
		return;
	}

	do {
		snprintf(out, MAX_NAME, "%s'%d", base, generatedCounter++);
	} while (findRuleIndex(out) != -1);
}

void parseCfg(const char *filePath) {
	FILE *f = fopen(filePath, "r");
	char line[MAX_LINE];

	if (!f) {
		perror("Cannot open CFG file");
		exit(1);
	}

	while (fgets(line, sizeof(line), f)) {
		char *arrow;
		char lhs[MAX_NAME];
		char rhsAll[MAX_LINE];
		char *part;

		trim(line);
		if (line[0] == '\0') {
			continue;
		}

		arrow = strstr(line, "->");
		if (!arrow) {
			fprintf(stderr, "Skipping invalid line (missing ->): %s\n", line);
			continue;
		}

		{
			int lhsLen = (int)(arrow - line);
			if (lhsLen <= 0 || lhsLen >= MAX_NAME) {
				fprintf(stderr, "Skipping invalid LHS: %s\n", line);
				continue;
			}
			strncpy(lhs, line, (size_t)lhsLen);
			lhs[lhsLen] = '\0';
			trim(lhs);
		}

		if (!isValidNonTerminalName(lhs)) {
			fprintf(stderr, "Skipping invalid non-terminal (must be multi-char and start uppercase): %s\n", lhs);
			continue;
		}

		{
			int ri = getOrCreateRule(lhs);
			Rule *r = &grammar[ri];

			strncpy(rhsAll, arrow + 2, sizeof(rhsAll) - 1);
			rhsAll[sizeof(rhsAll) - 1] = '\0';

			part = strtok(rhsAll, "|");
			while (part) {
				char rhs[MAX_RHS];
				strncpy(rhs, part, sizeof(rhs) - 1);
				rhs[sizeof(rhs) - 1] = '\0';
				normalizeRhs(rhs);
				addAltUnique(r, rhs);
				part = strtok(NULL, "|");
			}
		}
	}

	fclose(f);
}

void printGrammar(const char *title) {
	int i, j;

	printf("\n%s\n", title);
	for (i = 0; i < ruleCount; i++) {
		printf("%s -> ", grammar[i].name);
		for (j = 0; j < grammar[i].altCount; j++) {
			printf("%s", grammar[i].alts[j]);
			if (j < grammar[i].altCount - 1) {
				printf(" | ");
			}
		}
		printf("\n");
	}
}

static int commonPrefixLen(char a[MAX_TOKENS][MAX_NAME], int ac, char b[MAX_TOKENS][MAX_NAME], int bc) {
	int n = ac < bc ? ac : bc;
	int i = 0;

	while (i < n && strcmp(a[i], b[i]) == 0) {
		i++;
	}
	return i;
}

static int leftFactorOneRule(int ri) {
	Rule *r = &grammar[ri];
	int i, j, k;
	int bestPrefix = 0;
	int selected[MAX_ALTS] = {0};
	int selectedCount = 0;
	char bestTokens[MAX_TOKENS][MAX_NAME];
	char tokA[MAX_TOKENS][MAX_NAME];
	char tokB[MAX_TOKENS][MAX_NAME];
	char toks[MAX_TOKENS][MAX_NAME];

	if (r->altCount < 2) {
		return 0;
	}

	for (i = 0; i < r->altCount; i++) {
		int ac = tokenize(r->alts[i], tokA);
		for (j = i + 1; j < r->altCount; j++) {
			int bc = tokenize(r->alts[j], tokB);
			int l = commonPrefixLen(tokA, ac, tokB, bc);
			if (l > bestPrefix) {
				bestPrefix = l;
				for (k = 0; k < l; k++) {
					strcpy(bestTokens[k], tokA[k]);
				}
			}
		}
	}

	if (bestPrefix == 0) {
		return 0;
	}

	for (i = 0; i < r->altCount; i++) {
		int tc = tokenize(r->alts[i], toks);
		int match = tc >= bestPrefix;

		for (k = 0; k < bestPrefix && match; k++) {
			if (strcmp(toks[k], bestTokens[k]) != 0) {
				match = 0;
			}
		}

		if (match) {
			selected[i] = 1;
			selectedCount++;
		}
	}

	if (selectedCount < 2) {
		return 0;
	}

	{
		char newName[MAX_NAME];
		int nri;
		Rule *nr;
		char kept[MAX_ALTS][MAX_RHS];
		int keptCount = 0;

		generatePrimeName(r->name, newName);
		nri = getOrCreateRule(newName);
		nr = &grammar[nri];
		nr->altCount = 0;

		for (i = 0; i < r->altCount; i++) {
			if (!selected[i]) {
				strcpy(kept[keptCount], r->alts[i]);
				keptCount++;
				continue;
			}

			{
				int tc = tokenize(r->alts[i], toks);
				char suffix[MAX_RHS];
				joinTokens(toks, bestPrefix, tc, suffix);
				addAltUnique(nr, suffix);
			}
		}

		{
			char prefixText[MAX_RHS];
			char folded[MAX_RHS];

			joinTokens(bestTokens, 0, bestPrefix, prefixText);
			if (strcmp(prefixText, EPSILON) == 0) {
				snprintf(folded, sizeof(folded), "%s", newName);
			} else {
				snprintf(folded, sizeof(folded), "%s %s", prefixText, newName);
			}

			r->altCount = 0;
			for (i = 0; i < keptCount; i++) {
				addAltUnique(r, kept[i]);
			}
			addAltUnique(r, folded);
		}
	}

	return 1;
}

void leftFactorAll(void) {
	int changed;

	do {
		int startCount = ruleCount;
		int i;
		changed = 0;

		for (i = 0; i < startCount; i++) {
			if (leftFactorOneRule(i)) {
				changed = 1;
			}
		}
	} while (changed);
}

static int startsWithNonTerminal(const char *rhs, const char *nt, char *restOut) {
	char toks[MAX_TOKENS][MAX_NAME];
	int tc = tokenize(rhs, toks);

	if (tc == 0) {
		return 0;
	}
	if (strcmp(toks[0], nt) != 0) {
		return 0;
	}

	joinTokens(toks, 1, tc, restOut);
	return 1;
}

static void replaceAiAj(int ai, int aj) {
	Rule *A = &grammar[ai];
	Rule *B = &grammar[aj];
	char newAlts[MAX_ALTS][MAX_RHS];
	int newCount = 0;
	int i, k;

	for (i = 0; i < A->altCount; i++) {
		char gamma[MAX_RHS];

		if (!startsWithNonTerminal(A->alts[i], B->name, gamma)) {
			strcpy(newAlts[newCount], A->alts[i]);
			newCount++;
			continue;
		}

		for (k = 0; k < B->altCount; k++) {
			char merged[MAX_RHS];

			if (strcmp(B->alts[k], EPSILON) == 0 && strcmp(gamma, EPSILON) == 0) {
				strcpy(merged, EPSILON);
			} else if (strcmp(B->alts[k], EPSILON) == 0) {
				strcpy(merged, gamma);
			} else if (strcmp(gamma, EPSILON) == 0) {
				strcpy(merged, B->alts[k]);
			} else {
				snprintf(merged, sizeof(merged), "%s %s", B->alts[k], gamma);
			}

			strcpy(newAlts[newCount], merged);
			newCount++;
		}
	}

	A->altCount = 0;
	for (i = 0; i < newCount; i++) {
		addAltUnique(A, newAlts[i]);
	}
}

static void eliminateDirectLeftRecursion(int i) {
	Rule *A = &grammar[i];
	char alpha[MAX_ALTS][MAX_RHS];
	char beta[MAX_ALTS][MAX_RHS];
	int aCount = 0;
	int bCount = 0;
	int p;

	for (p = 0; p < A->altCount; p++) {
		char rest[MAX_RHS];
		if (startsWithNonTerminal(A->alts[p], A->name, rest)) {
			strcpy(alpha[aCount], rest);
			aCount++;
		} else {
			strcpy(beta[bCount], A->alts[p]);
			bCount++;
		}
	}

	if (aCount == 0) {
		return;
	}

	{
		char Aprime[MAX_NAME];
		int aprIdx;
		Rule *Apr;
		int k;

		generatePrimeName(A->name, Aprime);
		aprIdx = getOrCreateRule(Aprime);
		Apr = &grammar[aprIdx];
		Apr->altCount = 0;

		A->altCount = 0;
		if (bCount == 0) {
			addAltUnique(A, Aprime);
		} else {
			for (k = 0; k < bCount; k++) {
				char out[MAX_RHS];
				if (strcmp(beta[k], EPSILON) == 0) {
					snprintf(out, sizeof(out), "%s", Aprime);
				} else {
					snprintf(out, sizeof(out), "%s %s", beta[k], Aprime);
				}
				addAltUnique(A, out);
			}
		}

		for (k = 0; k < aCount; k++) {
			char out[MAX_RHS];
			if (strcmp(alpha[k], EPSILON) == 0) {
				snprintf(out, sizeof(out), "%s", Aprime);
			} else {
				snprintf(out, sizeof(out), "%s %s", alpha[k], Aprime);
			}
			addAltUnique(Apr, out);
		}

		addAltUnique(Apr, EPSILON);
	}
}

void removeLeftRecursionAll(void) {
	int initialCount = ruleCount;
	int i, j;

	for (i = 0; i < initialCount; i++) {
		for (j = 0; j < i; j++) {
			replaceAiAj(i, j);
		}
		eliminateDirectLeftRecursion(i);
	}
}
