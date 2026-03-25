#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_RULES 100
#define MAX_LEN 100
#define MAX_SYMBOLS 100

typedef struct {
    char nonTerminal[MAX_LEN];
    char productions[MAX_RULES][MAX_LEN];
    int prodCount;
} Rule;

Rule grammar[MAX_RULES];
int ruleCount = 0;
int newNonTerminalCounter = 1;


void trimSpaces(char *str) {
    char *end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
}


void generateNewNonTerminal(char *newNT, const char *base) {
    sprintf(newNT, "%s%d", base, newNonTerminalCounter++);
}


int isValidNonTerminal(const char *str) {
    if (!isupper(str[0])) return 0;
    
    for (int i = 1; str[i]; i++) {
        if (islower(str[i])) return 0;
    }
    return 1;
}

void eliminateLeftRecursion(int ruleIdx) {
    Rule *rule = &grammar[ruleIdx];
    char newNonTerminal[MAX_LEN];
    generateNewNonTerminal(newNonTerminal, rule->nonTerminal);

    char newProds[MAX_RULES][MAX_LEN];
    char recursiveProds[MAX_RULES][MAX_LEN];
    int newProdCount = 0, recCount = 0;

    for (int i = 0; i < rule->prodCount; i++) {
        trimSpaces(rule->productions[i]);
        if (strncmp(rule->productions[i], rule->nonTerminal, strlen(rule->nonTerminal)) == 0) {
            strcpy(recursiveProds[recCount], rule->productions[i] + strlen(rule->nonTerminal));
            strcat(recursiveProds[recCount], " ");
            strcat(recursiveProds[recCount], newNonTerminal);
            recCount++;
        } else {
            strcpy(newProds[newProdCount], rule->productions[i]);
            strcat(newProds[newProdCount], " ");
            strcat(newProds[newProdCount], newNonTerminal);
            newProdCount++;
        }
    }

    if (recCount > 0) {
        rule->prodCount = newProdCount;
        for (int i = 0; i < newProdCount; i++) {
            strcpy(rule->productions[i], newProds[i]);
        }

        Rule newRule;
        strcpy(newRule.nonTerminal, newNonTerminal);
        newRule.prodCount = recCount + 1;

        for (int i = 0; i < recCount; i++) {
            strcpy(newRule.productions[i], recursiveProds[i]);
        }
        strcpy(newRule.productions[recCount], "ε");

        grammar[ruleCount++] = newRule;
    }
}


void leftFactor(int ruleIdx) {
    Rule *rule = &grammar[ruleIdx];
    char commonPrefix[MAX_LEN] = "";
    int commonLen = 0;

    for (int i = 0; i < rule->prodCount - 1; i++) {
        for (int j = i + 1; j < rule->prodCount; j++) {
            int len = 0;
            while (rule->productions[i][len] && rule->productions[j][len] &&
                   rule->productions[i][len] == rule->productions[j][len]) {
                len++;
            }
            if (len > commonLen) {
                commonLen = len;
                strncpy(commonPrefix, rule->productions[i], len);
                commonPrefix[len] = '\0';
            }
        }
    }

    if (commonLen > 0) {
        char newNonTerminal[MAX_LEN];
        generateNewNonTerminal(newNonTerminal, rule->nonTerminal);
        Rule newRule;
        strcpy(newRule.nonTerminal, newNonTerminal);
        newRule.prodCount = 0;

        int newProdIndex = 0;
        for (int i = 0; i < rule->prodCount; i++) {
            if (strncmp(rule->productions[i], commonPrefix, commonLen) == 0) {
                char *suffix = rule->productions[i] + commonLen;
                if (*suffix == '\0') {
                    strcpy(newRule.productions[newRule.prodCount++], "ε");
                } else {
                    strcpy(newRule.productions[newRule.prodCount++], suffix);
                }
            } else {
                strcpy(rule->productions[newProdIndex++], rule->productions[i]);
            }
        }

        sprintf(rule->productions[newProdIndex], "%s %s", commonPrefix, newNonTerminal);
        rule->prodCount = newProdIndex + 1;
        grammar[ruleCount++] = newRule;
    }
}

void printGrammar() {
    printf("\nTransformed Grammar:\n");
    for (int i = 0; i < ruleCount; i++) {
        printf("%s -> ", grammar[i].nonTerminal);
        for (int j = 0; j < grammar[i].prodCount; j++) {
            printf("%s", grammar[i].productions[j]);
            if (j < grammar[i].prodCount - 1) printf(" | ");
        }
        printf("\n");
    }
}

void parseInputFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    char line[MAX_LEN];
    while (fgets(line, sizeof(line), file)) {
        trimSpaces(line);
        if (strlen(line) <= 1) continue;

        Rule rule;
        char *token;
        char *arrow = strstr(line, "->");
        if (!arrow) {
            printf("Syntax Error: Missing '->' in rule: %s\n", line);
            continue;
        }

        strncpy(rule.nonTerminal, line, arrow - line);
        rule.nonTerminal[arrow - line] = '\0';
        trimSpaces(rule.nonTerminal);

        if (!isValidNonTerminal(rule.nonTerminal)) {
            printf("Error: Invalid non-terminal: %s\n", rule.nonTerminal);
            continue;
        }

        rule.prodCount = 0;
        char *prodStr = arrow + 2;
        while (*prodStr) {
            while (isspace((unsigned char)*prodStr)) prodStr++;
            if (*prodStr == '\0') break;

            char *end = strchr(prodStr, '|');
            if (end) {
                *end = '\0';
                trimSpaces(prodStr);
                strcpy(rule.productions[rule.prodCount++], prodStr);
                prodStr = end + 1;
            } else {
                trimSpaces(prodStr);
                strcpy(rule.productions[rule.prodCount++], prodStr);
                break;
            }
        }

        grammar[ruleCount++] = rule;
    }
    fclose(file);
}

int isEpsilon(const char *str) {
    return (strcmp(str, "ε") == 0 || strcmp(str, "epsilon") == 0);
}

int isTerminalChar(char c) {
    return !isupper(c) && c != '\0' && c != ' ';
}

int isTerminalStr(const char *str) {
    return *str != '\0' && !isupper(str[0]) && !isEpsilon(str);
}

int isNonTerminalStr(const char *str) {
    return *str != '\0' && isupper(str[0]);
}

int getNonTerminalIndex(const char *nt) {
    for (int i = 0; i < ruleCount; i++) {
        if (strcmp(grammar[i].nonTerminal, nt) == 0) {
            return i;
        }
    }
    return -1;
}

typedef struct {
    char nonTerminal[MAX_LEN];
    char firstSet[MAX_SYMBOLS][MAX_LEN];
    int firstCount;
    int hasEpsilon;
} FirstSet;

FirstSet firstSets[MAX_RULES];
int firstSetCount = 0;


typedef struct {
    char nonTerminal[MAX_LEN];
    char followSet[MAX_SYMBOLS][MAX_LEN];
    int followCount;
    int hasEndMarker;
} FollowSet;

FollowSet followSets[MAX_RULES];
int followSetCount = 0;

int containsTerminal(FirstSet *set, const char *terminal) {
    for (int i = 0; i < set->firstCount; i++) {
        if (strcmp(set->firstSet[i], terminal) == 0) {
            return 1;
        }
    }
    return 0;
}

int containsTerminalInFollow(FollowSet *set, const char *terminal) {
    for (int i = 0; i < set->followCount; i++) {
        if (strcmp(set->followSet[i], terminal) == 0) {
            return 1;
        }
    }
    return 0;
}

int addToFirstSet(FirstSet *set, const char *terminal) {
    if (!containsTerminal(set, terminal)) {
        strcpy(set->firstSet[set->firstCount++], terminal);
        return 1;
    }
    return 0;
}

int addToFollowSet(FollowSet *set, const char *terminal) {
    if (!containsTerminalInFollow(set, terminal)) {
        strcpy(set->followSet[set->followCount++], terminal);
        return 1;
    }
    return 0;
}

int addAllToFollowSet(FollowSet *dest, FollowSet *src) {
    int changed = 0;
    for (int i = 0; i < src->followCount; i++) {
        if (addToFollowSet(dest, src->followSet[i])) {
            changed = 1;
        }
    }
    
    if (src->hasEndMarker && !dest->hasEndMarker) {
        dest->hasEndMarker = 1;
        changed = 1;
    }
    
    return changed;
}

void computeFirstSets() {
    for (int i = 0; i < ruleCount; i++) {
        strcpy(firstSets[i].nonTerminal, grammar[i].nonTerminal);
        firstSets[i].firstCount = 0;
        firstSets[i].hasEpsilon = 0;
    }
    firstSetCount = ruleCount;

    int changed;
    do {
        changed = 0;

        for (int i = 0; i < ruleCount; i++) {
            Rule *rule = &grammar[i];
            FirstSet *firstSet = &firstSets[i];

            for (int j = 0; j < rule->prodCount; j++) {
                char prodCopy[MAX_LEN];
                strcpy(prodCopy, rule->productions[j]);

                if (isEpsilon(prodCopy)) {
                    if (!firstSet->hasEpsilon) {
                        firstSet->hasEpsilon = 1;
                        changed = 1;
                    }
                    continue;
                }

                char *context = NULL;
                char *token = strtok_r(prodCopy, " ", &context);
                int allDeriveEpsilon = 1;
                
                while (token && allDeriveEpsilon) {
                    if (isTerminalStr(token)) {
                        if (addToFirstSet(firstSet, token)) {
                            changed = 1;
                        }
                        allDeriveEpsilon = 0;
                        break;
                    }
                    
                    if (isNonTerminalStr(token)) {
                        int ntIndex = getNonTerminalIndex(token);
                        if (ntIndex != -1) {
                            FirstSet *tokenFirstSet = &firstSets[ntIndex];

                            for (int k = 0; k < tokenFirstSet->firstCount; k++) {
                                if (addToFirstSet(firstSet, tokenFirstSet->firstSet[k])) {
                                    changed = 1;
                                }
                            }

                            if (!tokenFirstSet->hasEpsilon) {
                                allDeriveEpsilon = 0;
                                break;
                            }
                        }
                    }

                    token = strtok_r(NULL, " ", &context);
                }

                if (allDeriveEpsilon && !firstSet->hasEpsilon) {
                    firstSet->hasEpsilon = 1;
                    changed = 1;
                }
            }
        }
    } while (changed);
}

int getFirstOfSequence(char *sequence, char firstSymbols[][MAX_LEN], int *firstCount, int *hasEpsilon) {
    if (sequence == NULL || sequence[0] == '\0') {
        *hasEpsilon = 1;
        return 1;
    }

    char seqCopy[MAX_LEN];
    strcpy(seqCopy, sequence);

    *firstCount = 0;
    *hasEpsilon = 0;

    char *token;
    char *rest = seqCopy;
    int allCanDeriveEpsilon = 1;
    
    token = strtok(rest, " ");

    while (token != NULL && allCanDeriveEpsilon) {
        if (isEpsilon(token)) {
            token = strtok(NULL, " ");
            continue;
        }

        if (isTerminalStr(token)) {
            int found = 0;
            for (int i = 0; i < *firstCount; i++) {
                if (strcmp(firstSymbols[i], token) == 0) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                strcpy(firstSymbols[*firstCount], token);
                (*firstCount)++;
            }
            allCanDeriveEpsilon = 0;
            break;
        }

        int ntIndex = getNonTerminalIndex(token);
        if (ntIndex != -1) {
            for (int i = 0; i < firstSets[ntIndex].firstCount; i++) {
                int found = 0;
                for (int j = 0; j < *firstCount; j++) {
                    if (strcmp(firstSymbols[j], firstSets[ntIndex].firstSet[i]) == 0) {
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    strcpy(firstSymbols[*firstCount], firstSets[ntIndex].firstSet[i]);
                    (*firstCount)++;
                }
            }

            if (!firstSets[ntIndex].hasEpsilon) {
                allCanDeriveEpsilon = 0;
                break;
            }
        }

        token = strtok(NULL, " ");
    }

    if (allCanDeriveEpsilon) {
        *hasEpsilon = 1;
    }
    
    return allCanDeriveEpsilon;
}

void getRestOfProduction(char *production, const char *nonTerminal, char *rest) {
    char prodCopy[MAX_LEN];
    strcpy(prodCopy, production);

    char tokens[MAX_SYMBOLS][MAX_LEN];
    int tokenCount = 0;
    
    char *token = strtok(prodCopy, " ");
    while (token != NULL) {
        strcpy(tokens[tokenCount++], token);
        token = strtok(NULL, " ");
    }

    int foundPos = -1;
    for (int i = 0; i < tokenCount; i++) {
        if (strcmp(tokens[i], nonTerminal) == 0) {
            foundPos = i;
            break;
        }
    }

    if (foundPos != -1 && foundPos < tokenCount - 1) {
        rest[0] = '\0';
        for (int i = foundPos + 1; i < tokenCount; i++) {
            strcat(rest, tokens[i]);
            if (i < tokenCount - 1) {
                strcat(rest, " ");
            }
        }
    } else {
        rest[0] = '\0';
    }
}

void computeFollowSets() {
    followSetCount = ruleCount;
    for (int i = 0; i < ruleCount; i++) {
        strcpy(followSets[i].nonTerminal, grammar[i].nonTerminal);
        followSets[i].followCount = 0;
        followSets[i].hasEndMarker = 0;
    }

    followSets[0].hasEndMarker = 1;

    int changed;
    do {
        changed = 0;

        for (int i = 0; i < ruleCount; i++) {
            Rule *ruleA = &grammar[i];

            for (int j = 0; j < ruleA->prodCount; j++) {
                if (isEpsilon(ruleA->productions[j])) {
                    continue;
                }

                char prodCopy[MAX_LEN];
                strcpy(prodCopy, ruleA->productions[j]);

                char tokens[MAX_SYMBOLS][MAX_LEN];
                int tokenCount = 0;
                
                char *token = strtok(prodCopy, " ");
                while (token != NULL && tokenCount < MAX_SYMBOLS) {
                    strcpy(tokens[tokenCount++], token);
                    token = strtok(NULL, " ");
                }

                for (int k = 0; k < tokenCount; k++) {
                    if (isNonTerminalStr(tokens[k])) {
                        int ntBIndex = getNonTerminalIndex(tokens[k]);
                        if (ntBIndex == -1) continue;

                        if (k < tokenCount - 1) {
                            char firstOfBeta[MAX_SYMBOLS][MAX_LEN];
                            int firstCount = 0;
                            int hasEpsilon = 0;

                            char beta[MAX_LEN] = "";
                            for (int l = k + 1; l < tokenCount; l++) {
                                strcat(beta, tokens[l]);
                                if (l < tokenCount - 1) {
                                    strcat(beta, " ");
                                }
                            }

                            getFirstOfSequence(beta, firstOfBeta, &firstCount, &hasEpsilon);

                            for (int l = 0; l < firstCount; l++) {
                                if (addToFollowSet(&followSets[ntBIndex], firstOfBeta[l])) {
                                    changed = 1;
                                }
                            }

                            if (hasEpsilon) {
                                if (addAllToFollowSet(&followSets[ntBIndex], &followSets[i])) {
                                    changed = 1;
                                }
                            }
                        } 
                        else {
                            if (addAllToFollowSet(&followSets[ntBIndex], &followSets[i])) {
                                changed = 1;
                            }
                        }
                    }
                }
            }
        }
    } while (changed);
}

void printFirstSets() {
    printf("\nFirst Sets:\n");
    for (int i = 0; i < firstSetCount; i++) {
        printf("First(%s) = { ", firstSets[i].nonTerminal);
        for (int j = 0; j < firstSets[i].firstCount; j++) {
            printf("%s", firstSets[i].firstSet[j]);
            if (j < firstSets[i].firstCount - 1 || firstSets[i].hasEpsilon) {
                printf(", ");
            }
        }
        if (firstSets[i].hasEpsilon) {
            printf("ε");
        }
        printf(" }\n");
    }
}

void printFollowSets() {
    printf("\nFollow Sets:\n");
    for (int i = 0; i < followSetCount; i++) {
        printf("Follow(%s) = { ", followSets[i].nonTerminal);
        for (int j = 0; j < followSets[i].followCount; j++) {
            printf("%s", followSets[i].followSet[j]);
            if (j < followSets[i].followCount - 1 || followSets[i].hasEndMarker) {
                printf(", ");
            }
        }
        if (followSets[i].hasEndMarker) {
            printf("$");
        }
        printf(" }\n");
    }
}

typedef struct {
    char nonTerminal[MAX_LEN];
    char terminal[MAX_LEN];
    char production[MAX_LEN];
} TableEntry;

TableEntry parseTable[MAX_RULES * MAX_SYMBOLS];
int tableEntryCount = 0;

int containsEpsilon(const char *production) {
    return strstr(production, "ε") != NULL;
}

char* getProduction(const char *nonTerminal, const char *terminal) {
    for (int i = 0; i < tableEntryCount; i++) {
        if (strcmp(parseTable[i].nonTerminal, nonTerminal) == 0 && 
            strcmp(parseTable[i].terminal, terminal) == 0) {
            return parseTable[i].production;
        }
    }
    return NULL;
}

void buildParsingTable() {
    tableEntryCount = 0;

    for (int i = 0; i < ruleCount; i++) {
        Rule *rule = &grammar[i];
        const char *A = rule->nonTerminal;

        for (int j = 0; j < rule->prodCount; j++) {
            const char *alpha = rule->productions[j];

            char firstOfAlpha[MAX_SYMBOLS][MAX_LEN];
            int firstCount = 0;
            int alphaHasEpsilon = 0;

            getFirstOfSequence((char *)alpha, firstOfAlpha, &firstCount, &alphaHasEpsilon);

            for (int k = 0; k < firstCount; k++) {
                if (strcmp(firstOfAlpha[k], "ε") != 0) {
                    TableEntry entry;
                    strcpy(entry.nonTerminal, A);
                    strcpy(entry.terminal, firstOfAlpha[k]);
                    strcpy(entry.production, alpha);
                    parseTable[tableEntryCount++] = entry;
                }
            }

            if (alphaHasEpsilon || containsEpsilon(alpha)) {
                int followIndex = -1;
                for (int k = 0; k < followSetCount; k++) {
                    if (strcmp(followSets[k].nonTerminal, A) == 0) {
                        followIndex = k;
                        break;
                    }
                }

                if (followIndex != -1) {
                    FollowSet *followSet = &followSets[followIndex];

                    for (int k = 0; k < followSet->followCount; k++) {
                        TableEntry entry;
                        strcpy(entry.nonTerminal, A);
                        strcpy(entry.terminal, followSet->followSet[k]);
                        strcpy(entry.production, alpha);
                        parseTable[tableEntryCount++] = entry;
                    }

                    if (followSet->hasEndMarker) {
                        TableEntry entry;
                        strcpy(entry.nonTerminal, A);
                        strcpy(entry.terminal, "$");
                        strcpy(entry.production, alpha);
                        parseTable[tableEntryCount++] = entry;
                    }
                }
            }
        }
    }
}

void checkTableConflicts() {
    printf("\nChecking for conflicts in the LL(1) parsing table...\n");
    int hasConflict = 0;

    char terminals[MAX_SYMBOLS][MAX_LEN];
    int terminalCount = 0;
    int hasEndMarker = 0;

    for (int i = 0; i < firstSetCount; i++) {
        for (int j = 0; j < firstSets[i].firstCount; j++) {

            int found = 0;
            for (int k = 0; k < terminalCount; k++) {
                if (strcmp(terminals[k], firstSets[i].firstSet[j]) == 0) {
                    found = 1;
                    break;
                }
            }

            if (!found && strcmp(firstSets[i].firstSet[j], "ε") != 0) {
                strcpy(terminals[terminalCount++], firstSets[i].firstSet[j]);
            }
        }
    }

    for (int i = 0; i < followSetCount; i++) {
        if (followSets[i].hasEndMarker) {
            hasEndMarker = 1;
            break;
        }
    }

    for (int i = 0; i < ruleCount; i++) {
        const char *nt = grammar[i].nonTerminal;

        for (int j = 0; j < terminalCount; j++) {
            const char *terminal = terminals[j];

            int count = 0;
            char foundProductions[MAX_RULES][MAX_LEN];
            int prodCount = 0;

            for (int l = 0; l < tableEntryCount; l++) {
                if (strcmp(parseTable[l].nonTerminal, nt) == 0 &&
                    strcmp(parseTable[l].terminal, terminal) == 0) {
                    count++;
                    strcpy(foundProductions[prodCount++], parseTable[l].production);
                }
            }

            if (count > 1) {
                hasConflict = 1;
                printf("Conflict found for M[%s, %s]: Multiple productions ", nt, terminal);
                for (int l = 0; l < prodCount; l++) {
                    printf("%s ", foundProductions[l]);
                    if (l < prodCount - 1) printf("and ");
                }
                printf("\n");
            }
        }

        if (hasEndMarker) {
            int count = 0;
            char foundProductions[MAX_RULES][MAX_LEN];
            int prodCount = 0;

            for (int l = 0; l < tableEntryCount; l++) {
                if (strcmp(parseTable[l].nonTerminal, nt) == 0 &&
                    strcmp(parseTable[l].terminal, "$") == 0) {
                    count++;
                    strcpy(foundProductions[prodCount++], parseTable[l].production);
                }
            }

            if (count > 1) {
                hasConflict = 1;
                printf("Conflict found for M[%s, $]: Multiple productions ", nt);
                for (int l = 0; l < prodCount; l++) {
                    printf("%s ", foundProductions[l]);
                    if (l < prodCount - 1) printf("and ");
                }
                printf("\n");
            }
        }
    }

    if (!hasConflict) {
        printf("No conflicts found. The grammar is LL(1).\n");
    } else {
        printf("The grammar is not LL(1) due to the conflicts.\n");
    }
}

void printParsingTable() {
    printf("\nLL(1) Parsing Table:\n");

    char terminals[MAX_SYMBOLS][MAX_LEN];
    int terminalCount = 0;
    int hasEndMarker = 0;

    for (int i = 0; i < firstSetCount; i++) {
        for (int j = 0; j < firstSets[i].firstCount; j++) {
            int found = 0;
            for (int k = 0; k < terminalCount; k++) {
                if (strcmp(terminals[k], firstSets[i].firstSet[j]) == 0) {
                    found = 1;
                    break;
                }
            }

            if (!found && strcmp(firstSets[i].firstSet[j], "ε") != 0) {
                strcpy(terminals[terminalCount++], firstSets[i].firstSet[j]);
            }
        }
    }

    for (int i = 0; i < followSetCount; i++) {
        if (followSets[i].hasEndMarker) {
            hasEndMarker = 1;
            break;
        }
    }

    printf("%-10s ", "");
    for (int i = 0; i < terminalCount; i++) {
        printf("%-15s ", terminals[i]);
    }
    if (hasEndMarker) {
        printf("%-15s ", "$");
    }
    printf("\n");

    for (int i = 0; i < terminalCount + 1 + (hasEndMarker ? 1 : 0); i++) {
        printf("--------------- ");
    }
    printf("\n");

    for (int i = 0; i < ruleCount; i++) {
        const char *nt = grammar[i].nonTerminal;
        printf("%-10s ", nt);

        for (int j = 0; j < terminalCount; j++) {
            const char *terminal = terminals[j];
            char *production = getProduction(nt, terminal);

            if (production != NULL) {
                printf("%-15s ", production);
            } else {
                printf("%-15s ", "");
            }
        }

        if (hasEndMarker) {
            char *production = getProduction(nt, "$");
            if (production != NULL) {
                printf("%-15s ", production);
            } else {
                printf("%-15s ", "");
            }
        }

        printf("\n");
    }
}

int main() {

    const char* inputFile = "cfg.txt";
    
    parseInputFile(inputFile);
    printf("\nOriginal Grammar:\n");
    printGrammar();
    
    for (int i = 0; i < ruleCount; i++) {
        eliminateLeftRecursion(i);
    }
    printf("\nLeft Recursion Removed Grammar:\n");
    printGrammar();
    
    for (int i = 0; i < ruleCount; i++) {
        leftFactor(i);
    }
    printf("\nLeft Factored Grammar:\n");
    printGrammar();
    
    computeFirstSets();
    printFirstSets();
    
    computeFollowSets();
    printFollowSets();

    buildParsingTable();
    checkTableConflicts();
    printParsingTable();
    
    return 0;
}