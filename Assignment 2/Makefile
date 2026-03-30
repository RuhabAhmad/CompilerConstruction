CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -pedantic

SRC=src/main.c src/grammar.c src/first_follow.c src/parser.c src/stack.c src/tree.c src/error_handler.c
OBJ=$(SRC:.c=.o)
TARGET=parser_tool

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
