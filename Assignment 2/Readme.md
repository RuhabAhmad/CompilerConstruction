# LL(1) Parser Project (Assignment 02)

## Team Members
1. Roll Number: 22i1234
2. Roll Number: 22i5678

## Programming Language
C

## Project Overview
This project implements an LL(1) parser pipeline for compiler construction tasks:
1. Grammar transformation (left factoring, left recursion removal)
2. FIRST and FOLLOW set computation
3. LL(1) parsing table construction
4. Stack-based predictive parsing with step trace
5. Error detection and panic-mode style recovery
6. Parse tree generation for accepted strings

## Directory Structure
1. src/: C source and header files
2. input/: grammar and input-string files
3. output/: generated traces, sets, and parser outputs
4. docs/: additional documentation assets

## Compilation Instructions
From project root:

gcc based build via Makefile:
1. make clean
2. make

Generated executable:
1. parser_tool

## Execution Instructions
General command format:
1. ./parser_tool <grammar_file> <input_file> <trace_output_file> <tree_output_file>

Example runs:
1. ./parser_tool input/grammar1.txt input/input_valid.txt output/parsing_trace1.txt output/parse_trees.txt
2. ./parser_tool input/grammar1.txt input/input_errors.txt output/parsing_trace2.txt output/parse_trees_errors.txt

If optional arguments are omitted, defaults are used:
1. Grammar: input/grammar1.txt
2. Input strings: input/input_valid.txt
3. Trace output: output/parsing_trace1.txt
4. Tree output: output/parse_trees.txt

## Input File Format Specification

### Grammar File Format
1. One production per line
2. Format: NonTerminal -> production1 | production2 | ...
3. Non-terminals must be multi-character and start with uppercase (for example Expr, Term, Factor)
4. Single-character non-terminals (for example E, T, F) are not accepted
5. Epsilon can be written as epsilon or @
6. Tokens inside productions should be space-separated (recommended)

Example:
1. Expr -> Expr + Term | Term
2. Term -> Term * Factor | Factor
3. Factor -> ( Expr ) | id

### Input String File Format
1. One input string per line
2. Tokens are separated by spaces
3. Tokens should belong to grammar terminals

Example:
1. id + id * id
2. ( id + id ) * id
3. id * id + id

## Sample Grammar and Input Files
1. input/grammar1.txt: expression grammar (used for base validation)
2. input/grammar2.txt: reserved for additional testing grammar
3. input/grammar3.txt: reserved for additional testing grammar (can include indirect left recursion case)
4. input/input_valid.txt: valid strings for acceptance tests
5. input/input_errors.txt: invalid strings for error handling and recovery tests
6. input/input_edge_cases.txt: boundary and unusual input patterns

## Output Files
1. output/grammar_transformed.txt: transformed grammar snapshots (if exported)
2. output/first_follow_sets.txt: FIRST/FOLLOW output (if exported)
3. output/parsing_table.txt: LL(1) table output (if exported)
4. output/parsing_trace1.txt: parsing steps for valid-input run
5. output/parsing_trace2.txt: parsing steps for invalid-input run
6. output/parse_trees.txt: parse trees for accepted strings

## Known Limitations
1. Parse tree output is generated only for successfully accepted input strings.
2. Current parser uses panic-style recovery and may produce different recovery paths depending on grammar shape.
3. Output export for grammar transformations, FIRST/FOLLOW, and parsing table is currently printed to console unless redirected or additionally wired to files.
4. Grammar symbol naming and spacing conventions should follow documented format for best results.
