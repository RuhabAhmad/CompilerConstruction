# JSON to XML Translator

## Build
make

## Run
./json2xml < input.json
./json2xml --ast < input.json

## Description
- Uses Flex for lexical analysis
- Uses Bison for parsing
- Builds AST during parsing
- Traverses AST to generate XML

## AST Design
- NODE_OBJECT
- NODE_ARRAY
- NODE_STRING
- NODE_NUMBER
- NODE_BOOL
- NODE_NULL
- NODE_PAIR

## Features
- Supports objects, arrays, strings, numbers, booleans, null
- Proper nesting
- XML escaping
- Unicode escape decoding in strings (\uXXXX)
- Numbers with fractional/exponent notation
- Line/column error reporting
- Optional AST printing with --ast

## Limitations
- Unicode escape decoding does not handle surrogate pairs