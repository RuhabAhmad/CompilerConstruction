# DPP Language Lexical Analyzer

A comprehensive lexical analyzer (scanner) implementation for **DPP** (Data Processing Plus) - a custom programming language designed for compiler construction education.

## Team Members
- Roll Number 1: 23i-0559
- Roll Number 2: 23i-0698
- Section: [Your Section]

## Language Overview

### Language Name
**DPP (Data Processing Plus)**

### File Extension
`.dpp`

### About DPP
DPP is a modern, educational programming language designed to teach compiler construction principles. It features clean syntax, strong typing conventions, and comprehensive error handling capabilities.

## Language Specifications

### Keywords (Case-Sensitive)

The following keywords are reserved and have special meanings in DPP:

| Keyword | Meaning |
|---------|---------|
| `start` | Program entry point (like main) |
| `finish` | Program exit point |
| `loop` | Loop construct (for/while equivalent) |
| `condition` | Conditional statement (if equivalent) |
| `declare` | Variable declaration |
| `output` | Output/print statement |
| `input` | Input statement |
| `function` | Function definition |
| `return` | Return from function |
| `break` | Break from loop |
| `continue` | Continue to next iteration |
| `else` | Else clause for conditions |

### Identifiers

**Rules:**
- Must start with an **UPPERCASE** letter (A-Z)
- Followed by lowercase letters, digits, or underscores
- Maximum length: **31 characters**

**Valid Examples:**
```
Count
Variable_name
X
Total_sum_2024
My_var_123
Data_processor
```

**Invalid Examples:**
```
count          # Starts with lowercase
Variable       # Second char is uppercase
2Count         # Starts with digit
myVariable     # Starts with lowercase
_private       # Starts with underscore
```

### Literals

#### Integer Literals
- **Pattern:** `[+-]?[0-9]+`
- **Examples:** `42`, `+100`, `-567`, `0`, `999999`

#### Floating-Point Literals
- **Pattern:** `[+-]?[0-9]+\.[0-9]{1,6}([eE][+-]?[0-9]+)?`
- **Rules:** 1-6 decimal places, optional exponent
- **Examples:** 
  ```
  3.14
  +2.5
  -0.123456
  1.5e10
  2.0E-3
  ```

#### String Literals
- **Pattern:** `"([^"\\\n]|\\["\\ntr])*"`
- **Escape Sequences:** `\"`, `\\`, `\n`, `\t`, `\r`
- **Examples:**
  ```
  "Hello, DPP!"
  "She said \"Hi\""
  "C:\\Program Files\\DPP"
  "Line1\nLine2"
  "Col1\tCol2"
  ```

#### Character Literals
- **Pattern:** `'([^'\\\n]|\\['\\ntr])'`
- **Escape Sequences:** `\'`, `\\`, `\n`, `\t`, `\r`
- **Examples:** `'a'`, `'5'`, `'\n'`, `'\''`, `' '`

#### Boolean Literals
- **Values:** `true`, `false` (case-sensitive)

### Operators

#### Arithmetic Operators
| Operator | Description | Example | Precedence |
|----------|-------------|---------|------------|
| `**` | Exponentiation | `2 ** 3 = 8` | Highest |
| `*` | Multiplication | `5 * 3 = 15` | High |
| `/` | Division | `10 / 2 = 5` | High |
| `%` | Modulo | `7 % 3 = 1` | High |
| `+` | Addition | `5 + 3 = 8` | Medium |
| `-` | Subtraction | `5 - 3 = 2` | Medium |

#### Relational Operators
| Operator | Description |
|----------|-------------|
| `==` | Equal to |
| `!=` | Not equal to |
| `<` | Less than |
| `>` | Greater than |
| `<=` | Less than or equal to |
| `>=` | Greater than or equal to |

#### Logical Operators
| Operator | Description |
|----------|-------------|
| `&&` | Logical AND |
| `||` | Logical OR |
| `!` | Logical NOT |

#### Assignment Operators
| Operator | Description |
|----------|-------------|
| `=` | Simple assignment |
| `+=` | Add and assign |
| `-=` | Subtract and assign |
| `*=` | Multiply and assign |
| `/=` | Divide and assign |

#### Increment/Decrement
| Operator | Description |
|----------|-------------|
| `++` | Increment |
| `--` | Decrement |

### Punctuators
```
( ) { } [ ] , ; :
```

### Comments

#### Single-Line Comments
```dpp
## This is a single-line comment
```

#### Multi-Line Comments
```dpp
#*
This is a multi-line comment
that can span multiple lines
*#
```

#### Nested Comments (BONUS Feature)
```dpp
#*
Outer comment
#* Inner comment *#
Back to outer
*#
```

## Sample DPP Programs

### Sample 1: Hello World
```dpp
start
    declare Message = "Hello, DPP World!";
    output(Message);
finish
```

### Sample 2: Factorial Calculation
```dpp
start
    declare N = 5;
    declare Factorial = 1;
    declare I = 1;
    
    loop (I = 1; I <= N; I++) {
        Factorial *= I;
    }
    
    output("Factorial of 5: ");
    output(Factorial);
finish
```

### Sample 3: Conditional Logic
```dpp
start
    declare Age = 20;
    
    condition (Age >= 18) {
        output("Adult");
    } else {
        output("Minor");
    }
finish
```

### Sample 4: Function Definition
```dpp
start
    function Calculate_sum(A, B) {
        declare Result = A + B;
        return Result;
    }
    
    declare Total = Calculate_sum(10, 20);
    output("Sum is: ");
    output(Total);
finish
```

### Sample 5: Array Processing
```dpp
start
    declare Numbers = [1, 2, 3, 4, 5];
    declare Sum = 0;
    declare I = 0;
    
    loop (I = 0; I < 5; I++) {
        Sum += Numbers[I];
    }
    
    output("Sum: ");
    output(Sum);
finish
```

## Project Structure

```
23i0559-23i0698-E/
├── src/
│   ├── ManualScanner.java      # Manual DFA-based scanner
│   ├── Token.java               # Token class
│   ├── TokenType.java           # Token type enumeration
│   ├── SymbolTable.java         # Symbol table implementation
│   ├── ErrorHandler.java        # Error handling
│   ├── Scanner.flex             # JFlex specification
│   └── JFlexDriver.java         # JFlex scanner driver
├── docs/
│   ├── Automata_Design.pdf     # NFA/DFA diagrams
│   ├── README.md               # This file
│   ├── LanguageGrammar.txt     # Grammar specification
│   └── Comparison.pdf          # Scanner comparison
├── tests/
│   ├── test1.dpp               # All valid tokens
│   ├── test2.dpp               # Complex expressions
│   ├── test3.dpp               # Strings with escapes
│   ├── test4.dpp               # Lexical errors
│   ├── test5.dpp               # Comments
│   └── TestResults.txt         # Test outputs
└── README.md
```

## Compilation and Execution

### Prerequisites
- Java Development Kit (JDK) 8 or higher
- JFlex (for Part 2)

### Compiling the Manual Scanner

```bash
# Navigate to src directory
cd src

# Compile all Java files
javac *.java

# Run the scanner on a DPP file
java ManualScanner ../tests/test1.dpp
```

### Generating and Compiling JFlex Scanner

```bash
# Navigate to src directory
cd src

# Generate scanner from JFlex specification
jflex Scanner.flex

# Compile generated scanner
javac Yylex.java JFlexDriver.java

# Run JFlex scanner on a DPP file
java JFlexDriver ../tests/test1.dpp
```

### Running Tests

```bash
# Run all tests with manual scanner
for file in tests/*.dpp; do
    echo "Testing $file"
    java -cp src ManualScanner "$file"
done
```

## Features

### Part 1: Manual Implementation
- ✅ Complete DFA-based token recognition
- ✅ All token types (keywords, identifiers, literals, operators, punctuators)
- ✅ Longest match principle
- ✅ Proper operator precedence
- ✅ Whitespace handling and line/column tracking
- ✅ Symbol table with identifier tracking
- ✅ Comprehensive statistics

### Part 2: JFlex Implementation
- ✅ Complete JFlex specification for DPP
- ✅ Matching all manual scanner capabilities
- ✅ Identical token output format
- ✅ Same pattern matching priority

### Part 3: Error Handling
- ✅ Invalid character detection
- ✅ Malformed literal detection
- ✅ Invalid identifier detection
- ✅ Unterminated string/character detection
- ✅ Unclosed comment detection
- ✅ Error recovery (continue scanning)
- ✅ Comprehensive error reporting

### Bonus Features
- ✅ GitHub repository with CI/CD
- ✅ Nested multi-line comments support
- ✅ Advanced string escape sequences
- ✅ DFA minimization implementation

## DPP Language Philosophy

DPP was designed with the following principles:
1. **Clear Syntax** - Easy to read and write
2. **Strong Conventions** - Uppercase identifiers promote visibility
3. **Educational Focus** - Perfect for learning compiler construction
4. **Modern Features** - Supports contemporary programming patterns
5. **Error Tolerance** - Helpful error messages for learning

## Implementation Details

### Token Recognition Priority
1. Multi-line comments (`#* ... *#`)
2. Single-line comments (`##...`)
3. Multi-character operators
4. Keywords
5. Boolean literals
6. Identifiers (uppercase start)
7. Floating-point literals
8. Integer literals
9. String and character literals
10. Single-character operators
11. Punctuators
12. Whitespace

### Symbol Table
Tracks all identifiers with:
- Identifier name
- Type (IDENTIFIER)
- First occurrence (line number)
- Frequency (number of occurrences)

### Error Handling
Comprehensive error detection and reporting:
- Error type classification
- Precise line and column information
- Descriptive error messages
- Error recovery to continue scanning

## Testing

The project includes 5 comprehensive test files:

1. **test1.dpp** - All valid token types
2. **test2.dpp** - Complex expressions and nested structures
3. **test3.dpp** - String and character literals with escape sequences
4. **test4.dpp** - Various lexical errors
5. **test5.dpp** - Comment handling (single-line, multi-line, nested)

## Why DPP?

DPP stands for **Data Processing Plus**, reflecting its focus on:
- **Data** - Strong typing and clear variable declarations
- **Processing** - Rich operator set and control flow
- **Plus** - Enhanced features beyond basic languages

The `.dpp` extension makes DPP programs instantly recognizable and distinguishes them from other educational languages.

## References

- Aho, A. V., Lam, M. S., Sethi, R., & Ullman, J. D. (2006). *Compilers: Principles, Techniques, and Tools* (2nd ed.). Addison-Wesley.
- JFlex Manual: https://jflex.de/manual.html
- GeeksforGeeks Compiler Design: https://www.geeksforgeeks.org/introduction-of-compiler-design/

## License

This project is created for educational purposes as part of CS4031 - Compiler Construction course.

**Welcome to DPP - Where Data Processing Gets a Plus!** 🚀