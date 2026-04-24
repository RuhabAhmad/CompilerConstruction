# Makefile for Compiler Construction Assignment 3
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = .

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
EXECUTABLE = parser_exec

all: directories $(EXECUTABLE)

directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p output

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(EXECUTABLE) output/*

run_slr: all
	./$(EXECUTABLE) --slr input/grammar1.txt input/input_valid.txt

run_lr1: all
	./$(EXECUTABLE) --lr1 input/grammar1.txt input/input_valid.txt

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXECUTABLE) --slr input/grammar1.txt input/input_valid.txt