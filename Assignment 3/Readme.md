# Assignment 3: Bottom-Up Parser Design & Implementation

## Team Members
* Ruhab Ahmad (23I-0559)
* [Partner Name] ([Partner Roll Number])

## Programming Language
* C++ (Standard: C++17)

## Compilation Instructions
This project uses a standard Makefile and is designed for Linux/WSL2 environments.
To compile the codebase, navigate to the root directory and run:
`make all`

## Execution Instructions
The parser can be executed with command-line flags to specify the parsing mode (SLR or LR1), the grammar file, and the input file.

**Sample command to run SLR(1) parser:**
`./parser_exec --slr input/grammar1.txt input/input_valid.txt`

**Sample command to run LR(1) parser:**
`./parser_exec --lr1 input/grammar1.txt input/input_valid.txt`

## Input File Formats
### Grammar File Format
* One production per line.
* Format: `NonTerminal -> production1 | production2`
* Use `->` as the arrow symbol.
* Use `|` to separate alternatives.
* Epsilon productions use the keyword `epsilon` or `@`.

### Known Limitations
* Ensure grammar symbols are strictly space-separated in the text file for the tokenizer to read correctly.
* Single-character non-terminals are not supported as per assignment constraints. Ensure non-terminals are multi-character (e.g., `Expr`, `Term`).