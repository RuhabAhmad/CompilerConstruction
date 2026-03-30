#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <stdio.h>

void reportError(const char *message);
void reportWarning(const char *message);
void reportParseError(FILE *out, int line, int column, const char *type, const char *expected, const char *found);

#endif
