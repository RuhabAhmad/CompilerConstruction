#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "grammar.h"
#include "first_follow.h"
#include "parser.h"
#include "stack.h"
#include "tree.h"
#include "error_handler.h"

static char terminals[MAX_SYMBOLS][MAX_NAME];
static int terminalCount = 0;

static char table[MAX_NT][MAX_SYMBOLS + 1][MAX_RHS];
static int tableFilled[MAX_NT][MAX_SYMBOLS + 1];
static int tableConflict[MAX_NT][MAX_SYMBOLS + 1];

static int terminalIndex(const char *t) {
    int i;
    for (i = 0; i < terminalCount; i++) {
        if (strcmp(terminals[i], t) == 0) {
            return i;
        }
    }
    return -1;
}

static void addTerminal(const char *t) {
    if (strcmp(t, EPSILON) == 0 || strcmp(t, "$") == 0) {
        return;
    }
    if (terminalIndex(t) != -1) {
        return;
    }
    if (terminalCount >= MAX_SYMBOLS) {
        die("Too many terminals");
    }

    strcpy(terminals[terminalCount], t);
    terminalCount++;
}

static void collectTerminals(void) {
    int i, j, k;

    terminalCount = 0;

    for (i = 0; i < ruleCount; i++) {
        for (j = 0; j < grammar[i].altCount; j++) {
            char toks[MAX_TOKENS][MAX_NAME];
            int tc = tokenize(grammar[i].alts[j], toks);
            for (k = 0; k < tc; k++) {
                if (findRuleIndex(toks[k]) == -1 && strcmp(toks[k], EPSILON) != 0) {
                    addTerminal(toks[k]);
                }
            }
        }
    }

    for (i = 0; i < ruleCount; i++) {
        for (j = 0; j < firstSets[i].count; j++) {
            addTerminal(firstSets[i].items[j]);
        }
        for (j = 0; j < followSets[i].count; j++) {
            addTerminal(followSets[i].items[j]);
        }
    }
}

static void addTableEntry(int row, int col, const char *rhs) {
    if (!tableFilled[row][col]) {
        strcpy(table[row][col], rhs);
        tableFilled[row][col] = 1;
        tableConflict[row][col] = 0;
        return;
    }

    if (strcmp(table[row][col], rhs) == 0) {
        return;
    }

    tableConflict[row][col] = 1;
    if (strstr(table[row][col], rhs) == NULL) {
        if ((int)(strlen(table[row][col]) + strlen(rhs) + 4) >= MAX_RHS) {
            die("Parsing table cell overflow");
        }
        strcat(table[row][col], " / ");
        strcat(table[row][col], rhs);
    }
}

int buildLL1Table(void) {
    int a, p;
    int hasConflict = 0;

    memset(table, 0, sizeof(table));
    memset(tableFilled, 0, sizeof(tableFilled));
    memset(tableConflict, 0, sizeof(tableConflict));

    collectTerminals();

    for (a = 0; a < ruleCount; a++) {
        Rule *A = &grammar[a];

        for (p = 0; p < A->altCount; p++) {
            char toks[MAX_TOKENS][MAX_NAME];
            int tc = tokenize(A->alts[p], toks);
            Set firstAlpha;
            int hasEpsilon;
            int i;

            firstAlpha.count = 0;
            firstAlpha.hasDollar = 0;
            hasEpsilon = 0;

            firstOfSequenceTokens(toks, tc, &firstAlpha, &hasEpsilon);

            for (i = 0; i < firstAlpha.count; i++) {
                int c;
                if (strcmp(firstAlpha.items[i], EPSILON) == 0) {
                    continue;
                }
                c = terminalIndex(firstAlpha.items[i]);
                if (c != -1) {
                    addTableEntry(a, c, A->alts[p]);
                }
            }

            if (hasEpsilon) {
                for (i = 0; i < followSets[a].count; i++) {
                    int c = terminalIndex(followSets[a].items[i]);
                    if (c != -1) {
                        addTableEntry(a, c, A->alts[p]);
                    }
                }
                if (followSets[a].hasDollar) {
                    addTableEntry(a, terminalCount, A->alts[p]);
                }
            }
        }
    }

    for (a = 0; a < ruleCount; a++) {
        int c;
        for (c = 0; c <= terminalCount; c++) {
            if (tableConflict[a][c]) {
                hasConflict = 1;
            }
        }
    }

    return !hasConflict;
}

void printLL1Table(int isLL1) {
    int i, t;

    printf("\nLL(1) Parsing Table\n");
    printf("%-20s", "NT/T");
    for (t = 0; t < terminalCount; t++) {
        printf("| %-18s", terminals[t]);
    }
    printf("| %-18s\n", "$");

    for (i = 0; i < ruleCount; i++) {
        printf("%-20s", grammar[i].name);
        for (t = 0; t < terminalCount; t++) {
            if (tableFilled[i][t]) {
                printf("| %-18s", table[i][t]);
            } else {
                printf("| %-18s", "");
            }
        }
        if (tableFilled[i][terminalCount]) {
            printf("| %-18s", table[i][terminalCount]);
        } else {
            printf("| %-18s", "");
        }
        printf("\n");
    }

    if (isLL1) {
        printf("\nGrammar Status: LL(1)\n");
    } else {
        printf("\nGrammar Status: NOT LL(1)\n");
        for (i = 0; i < ruleCount; i++) {
            for (t = 0; t < terminalCount; t++) {
                if (tableConflict[i][t]) {
                    printf("Conflict at M[%s, %s] => %s\n", grammar[i].name, terminals[t], table[i][t]);
                }
            }
            if (tableConflict[i][terminalCount]) {
                printf("Conflict at M[%s, $] => %s\n", grammar[i].name, table[i][terminalCount]);
            }
        }
    }
}

const char *parserLookupProduction(const char *nonTerminal, const char *terminal) {
    int row = findRuleIndex(nonTerminal);
    int col;

    if (row == -1) {
        return NULL;
    }

    if (strcmp(terminal, "$") == 0) {
        col = terminalCount;
    } else {
        col = terminalIndex(terminal);
    }

    if (col < 0 || col > terminalCount) {
        return NULL;
    }

    if (!tableFilled[row][col]) {
        return NULL;
    }

    return table[row][col];
}

int parserCollectExpected(const char *nonTerminal, char outExpected[][MAX_NAME], int maxExpected) {
    int row = findRuleIndex(nonTerminal);
    int count = 0;
    int c;

    if (row == -1 || maxExpected <= 0) {
        return 0;
    }

    for (c = 0; c <= terminalCount && count < maxExpected; c++) {
        if (!tableFilled[row][c]) {
            continue;
        }

        if (c == terminalCount) {
            strcpy(outExpected[count], "$");
        } else {
            strcpy(outExpected[count], terminals[c]);
        }
        count++;
    }

    return count;
}

static void trimInPlace(char *s) {
    int n = (int)strlen(s);
    int i = 0;
    int j;

    while (i < n && isspace((unsigned char)s[i])) {
        i++;
    }
    if (i == n) {
        s[0] = '\0';
        return;
    }

    j = n - 1;
    while (j >= i && isspace((unsigned char)s[j])) {
        j--;
    }

    if (i > 0) {
        memmove(s, s + i, (size_t)(j - i + 1));
    }
    s[j - i + 1] = '\0';
}

static int splitInputTokens(const char *line, char tokens[MAX_SYMBOLS][MAX_NAME]) {
    char copy[MAX_LINE];
    char *tok;
    int count = 0;

    strncpy(copy, line, sizeof(copy) - 1);
    copy[sizeof(copy) - 1] = '\0';

    tok = strtok(copy, " \t\r\n");
    while (tok) {
        if (count >= MAX_SYMBOLS - 1) {
            break;
        }
        strncpy(tokens[count], tok, MAX_NAME - 1);
        tokens[count][MAX_NAME - 1] = '\0';
        count++;
        tok = strtok(NULL, " \t\r\n");
    }

    strcpy(tokens[count], "$");
    return count + 1;
}

static void formatStack(const SymbolStack *s, char *buf, int bufSize) {
    int i;

    buf[0] = '\0';
    for (i = 0; i <= s->top; i++) {
        if ((int)(strlen(buf) + strlen(s->items[i]) + 2) >= bufSize) {
            break;
        }
        strcat(buf, s->items[i]);
        if (i < s->top) {
            strcat(buf, " ");
        }
    }
}

static void formatRemainingInput(char tokens[MAX_SYMBOLS][MAX_NAME], int total, int ip, char *buf, int bufSize) {
    int i;

    buf[0] = '\0';
    for (i = ip; i < total; i++) {
        if ((int)(strlen(buf) + strlen(tokens[i]) + 2) >= bufSize) {
            break;
        }
        strcat(buf, tokens[i]);
        if (i < total - 1) {
            strcat(buf, " ");
        }
    }
}

static int pushWithNode(SymbolStack *stack, TreeNode **nodeRefs, const char *sym, TreeNode *node) {
    if (!stackPush(stack, sym)) {
        return 0;
    }
    nodeRefs[stack->top] = node;
    return 1;
}

static int popWithNode(SymbolStack *stack, TreeNode **nodeRefs, char *symOut, TreeNode **nodeOut) {
    if (stackIsEmpty(stack)) {
        return 0;
    }

    if (nodeOut) {
        *nodeOut = nodeRefs[stack->top];
    }
    nodeRefs[stack->top] = NULL;

    return stackPop(stack, symOut);
}

static int isInFollowSet(const char *nonTerminal, const char *lookahead) {
    int idx = findRuleIndex(nonTerminal);
    int j;

    if (idx == -1) {
        return 0;
    }

    if (strcmp(lookahead, "$") == 0) {
        return followSets[idx].hasDollar;
    }

    for (j = 0; j < followSets[idx].count; j++) {
        if (strcmp(followSets[idx].items[j], lookahead) == 0) {
            return 1;
        }
    }

    return 0;
}

static void expectedToString(const char *nonTerminal, char *buf, int bufSize) {
    char expected[MAX_SYMBOLS][MAX_NAME];
    int n = parserCollectExpected(nonTerminal, expected, MAX_SYMBOLS);
    int i;

    buf[0] = '\0';
    for (i = 0; i < n; i++) {
        if ((int)(strlen(buf) + strlen(expected[i]) + 3) >= bufSize) {
            break;
        }
        strcat(buf, expected[i]);
        if (i < n - 1) {
            strcat(buf, ", ");
        }
    }

    if (n == 0) {
        strncpy(buf, "<none>", (size_t)bufSize - 1);
        buf[bufSize - 1] = '\0';
    }
}

static int parseOneLine(const char *line,
                        int lineNo,
                        FILE *traceOut,
                        FILE *treeOut) {
    char inputTokens[MAX_SYMBOLS][MAX_NAME];
    int inputCount;
    int ip = 0;
    SymbolStack stack;
    TreeNode *nodeRefs[MAX_SYMBOLS];
    TreeNode *root;
    int step = 1;
    int errors = 0;
    int maxSteps = 2000;

    int i;
    for (i = 0; i < MAX_SYMBOLS; i++) {
        nodeRefs[i] = NULL;
    }

    inputCount = splitInputTokens(line, inputTokens);

    stackInit(&stack);
    pushWithNode(&stack, nodeRefs, "$", NULL);

    root = treeCreateNode(grammar[0].name);
    if (!root) {
        reportError("Failed to allocate parse tree root");
        return 0;
    }
    pushWithNode(&stack, nodeRefs, grammar[0].name, root);

    fprintf(traceOut, "\n=== Input Line %d ===\n", lineNo);
    fprintf(traceOut, "Input: %s\n", line);
    printf("\n=== Input Line %d ===\n", lineNo);
    printf("Input: %s\n", line);

    fprintf(traceOut, "%-6s | %-40s | %-40s | %s\n", "Step", "Stack(bottom->top)", "Remaining Input", "Action");
    printf("%-6s | %-40s | %-40s | %s\n", "Step", "Stack(bottom->top)", "Remaining Input", "Action");

    while (maxSteps-- > 0) {
        char X[MAX_NAME];
        const char *a = inputTokens[ip];
        char stackStr[1024];
        char inputStr[1024];
        char action[512];

        if (!stackTop(&stack, X)) {
            reportParseError(traceOut, lineNo, ip + 1, "StackUnderflow", "<non-empty stack>", a);
            reportParseError(stdout, lineNo, ip + 1, "StackUnderflow", "<non-empty stack>", a);
            errors++;
            break;
        }

        formatStack(&stack, stackStr, (int)sizeof(stackStr));
        formatRemainingInput(inputTokens, inputCount, ip, inputStr, (int)sizeof(inputStr));

        if (strcmp(X, "$") == 0 && strcmp(a, "$") == 0) {
            snprintf(action, sizeof(action), "ACCEPT");
            int shownStep = step++;
            fprintf(traceOut, "%-6d | %-40s | %-40s | %s\n", shownStep, stackStr, inputStr, action);
            printf("%-6d | %-40s | %-40s | %s\n", shownStep, stackStr, inputStr, action);
            break;
        }

        if (findRuleIndex(X) != -1) {
            const char *prod = parserLookupProduction(X, a);

            if (prod) {
                TreeNode *parent = NULL;
                char popped[MAX_NAME];
                char rhsTokens[MAX_TOKENS][MAX_NAME];
                int tc;

                popWithNode(&stack, nodeRefs, popped, &parent);
                tc = tokenize(prod, rhsTokens);

                if (tc == 0) {
                    TreeNode *epsNode = treeCreateNode(EPSILON);
                    treeAddChild(parent, epsNode);
                } else {
                    TreeNode *children[MAX_TOKENS];
                    int k;

                    for (k = 0; k < tc; k++) {
                        children[k] = treeCreateNode(rhsTokens[k]);
                        treeAddChild(parent, children[k]);
                    }

                    for (k = tc - 1; k >= 0; k--) {
                        if (strcmp(rhsTokens[k], EPSILON) != 0) {
                            if (!pushWithNode(&stack, nodeRefs, rhsTokens[k], children[k])) {
                                reportError("Stack overflow while expanding production");
                                errors++;
                                break;
                            }
                        }
                    }
                }

                snprintf(action, sizeof(action), "Expand: %s -> %s", X, prod);
            } else {
                char expected[512];
                expectedToString(X, expected, (int)sizeof(expected));

                reportParseError(traceOut, lineNo, ip + 1, "EmptyTableEntry", expected, a);
                reportParseError(stdout, lineNo, ip + 1, "EmptyTableEntry", expected, a);
                errors++;

                if (isInFollowSet(X, a)) {
                    char popped[MAX_NAME];
                    TreeNode *tmp = NULL;
                    popWithNode(&stack, nodeRefs, popped, &tmp);
                    snprintf(action, sizeof(action), "Panic recovery: pop %s (sync by FOLLOW)", X);
                } else if (strcmp(a, "$") != 0) {
                    ip++;
                    snprintf(action, sizeof(action), "Panic recovery: skip input token '%s'", a);
                } else {
                    char popped[MAX_NAME];
                    TreeNode *tmp = NULL;
                    popWithNode(&stack, nodeRefs, popped, &tmp);
                    snprintf(action, sizeof(action), "Premature end: pop %s", X);
                }
            }
        } else {
            if (strcmp(X, a) == 0) {
                char popped[MAX_NAME];
                TreeNode *tmp = NULL;
                popWithNode(&stack, nodeRefs, popped, &tmp);
                ip++;
                snprintf(action, sizeof(action), "Match '%s'", a);
            } else {
                if (strcmp(X, "$") == 0 && strcmp(a, "$") != 0) {
                    reportParseError(traceOut, lineNo, ip + 1, "UnexpectedSymbol", "$", a);
                    reportParseError(stdout, lineNo, ip + 1, "UnexpectedSymbol", "$", a);
                    errors++;
                    ip++;
                    snprintf(action, sizeof(action), "Recovery: skip extra input '%s'", a);
                } else {
                    char popped[MAX_NAME];
                    TreeNode *tmp = NULL;

                    if (strcmp(a, "$") == 0) {
                        reportParseError(traceOut, lineNo, ip + 1, "PrematureEnd", X, a);
                        reportParseError(stdout, lineNo, ip + 1, "PrematureEnd", X, a);
                    } else {
                        reportParseError(traceOut, lineNo, ip + 1, "UnexpectedSymbol", X, a);
                        reportParseError(stdout, lineNo, ip + 1, "UnexpectedSymbol", X, a);
                    }
                    errors++;

                    popWithNode(&stack, nodeRefs, popped, &tmp);
                    snprintf(action, sizeof(action), "Recovery: insert missing '%s' (pop stack)", X);
                }
            }
        }

        {
            int shownStep = step++;
            fprintf(traceOut, "%-6d | %-40s | %-40s | %s\n", shownStep, stackStr, inputStr, action);
            printf("%-6d | %-40s | %-40s | %s\n", shownStep, stackStr, inputStr, action);
        }

        if (ip >= inputCount) {
            break;
        }
    }

    if (maxSteps <= 0) {
        reportParseError(traceOut, lineNo, ip + 1, "Abort", "<finite parse>", "step-limit-reached");
        reportParseError(stdout, lineNo, ip + 1, "Abort", "<finite parse>", "step-limit-reached");
        errors++;
    }

    if (errors == 0) {
        fprintf(traceOut, "Result: ACCEPTED\n");
        printf("Result: ACCEPTED\n");
        fprintf(treeOut, "\n=== Parse Tree for Input Line %d ===\n", lineNo);
        fprintf(treeOut, "Input: %s\n", line);
        treePrintIndented(root, 0, treeOut);
        treePrintIndented(root, 0, stdout);
    } else {
        fprintf(traceOut, "Result: COMPLETED WITH %d ERROR(S)\n", errors);
        printf("Result: COMPLETED WITH %d ERROR(S)\n", errors);
    }

    treeFree(root);
    return errors == 0;
}

void parseInputFile(const char *inputPath, const char *traceOutPath, const char *treeOutPath) {
    FILE *in = fopen(inputPath, "r");
    FILE *traceOut;
    FILE *treeOut;
    char line[MAX_LINE];
    int lineNo = 0;

    if (!in) {
        perror("Cannot open parser input file");
        return;
    }

    traceOut = fopen(traceOutPath, "w");
    treeOut = fopen(treeOutPath, "w");

    if (!traceOut || !treeOut) {
        if (traceOut) {
            fclose(traceOut);
        }
        if (treeOut) {
            fclose(treeOut);
        }
        fclose(in);
        perror("Cannot open parser output files");
        return;
    }

    fprintf(traceOut, "LL(1) Stack Parsing Trace\n");
    fprintf(treeOut, "Parse Trees (successful inputs only)\n");

    while (fgets(line, sizeof(line), in)) {
        lineNo++;
        trimInPlace(line);
        if (line[0] == '\0') {
            continue;
        }
        parseOneLine(line, lineNo, traceOut, treeOut);
    }

    fclose(treeOut);
    fclose(traceOut);
    fclose(in);
}
