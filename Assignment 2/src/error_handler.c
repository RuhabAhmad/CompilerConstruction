#include <stdio.h>
#include "error_handler.h"

void reportError(const char *message) {
	fprintf(stderr, "Error: %s\n", message);
}

void reportWarning(const char *message) {
	fprintf(stderr, "Warning: %s\n", message);
}

void reportParseError(FILE *out, int line, int column, const char *type, const char *expected, const char *found) {
	if (!out) {
		out = stderr;
	}
	fprintf(out,
		"[Parse Error] line=%d, column=%d, type=%s, expected=%s, found=%s\n",
		line,
		column,
		type ? type : "unknown",
		expected ? expected : "-",
		found ? found : "-");
}
