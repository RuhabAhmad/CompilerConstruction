# Nexus Programming Language - Lexical Analyzer

**CS4031 - Compiler Construction Assignment 01**

## Team Members
- **Student 1**: Roll Number 1 - Section A
- **Student 2**: Roll Number 2 - Section A

---

## Table of Contents
1. [Language Overview](#language-overview)
2. [Language Specifications](#language-specifications)
3. [Sample Programs](#sample-programs)
4. [Compilation and Execution](#compilation-and-execution)
5. [Project Structure](#project-structure)
6. [Implementation Details](#implementation-details)

---

## Language Overview

### Language Name
**Nexus** - A modern, statically-typed programming language designed for educational purposes.

### File Extension
`.nex`

### Language Philosophy
Nexus is designed to demonstrate fundamental compiler construction concepts while maintaining a clean, readable syntax. It enforces strict lexical rules and supports modern programming constructs.

---

## Language Specifications

### 1. Keywords (Case-Sensitive)

All keywords in Nexus are **lowercase** and **case-sensitive**.

| Keyword | Description |
|---------|-------------|
| `start` | Program entry point (like `main` in C) |
| `finish` | Program exit point |
| `loop` | Loop construct (infinite loop, use break to exit) |
| `condition` | Conditional statement (like `if`) |
| `declare` | Variable declaration |
| `output` | Output statement (like `print`) |
| `input` | Input statement |
| `function` | Function declaration |
| `return` | Return from function |
| `break` | Break from loop |
| `continue` | Continue to next loop iteration |
| `else` | Alternative branch in condition |

**Examples:**
```nexus
start
    declare X = 10;
    condition (X > 5) {
        output "X is greater than 5";
    } else {
        output "X is 5 or less";
    }
finish
```

### 2. Identifiers

**Rules:**
- Must start with an **uppercase letter** (A-Z)
- Followed by zero or more lowercase letters, digits, or underscores
- Maximum length: **31 characters**
- Case-sensitive

**Valid Examples:**
- `Count`
- `Variable_name`
- `X`
- `Total_sum_2024`
- `Array_index_1`

**Invalid Examples:**
- `count` (starts with lowercase)
- `Variable` (contains uppercase after first character)
- `2Count` (starts with digit)
- `myVariable` (contains lowercase followed by uppercase)

### 3. Literals

#### Integer Literals
- **Format**: `[+-]?[0-9]+`
- Optional sign followed by one or more digits

**Examples:**
- `42`
- `+100`
- `-567`
- `0`

**Invalid:**
- `12.34` (use float instead)
- `1,000` (no comma separators)

#### Floating-Point Literals
- **Format**: `[+-]?[0-9]+\.[0-9]{1,6}([eE][+-]?[0-9]+)?`
- Optional sign
- Integer part
- Decimal point
- 1 to 6 decimal digits
- Optional exponent (e or E with optional sign and digits)

**Examples:**
- `3.14`
- `+2.5`
- `-0.123456`
- `1.5e10`
- `2.0E-3`

**Invalid:**
- `3.` (no fractional part)
- `.14` (no integer part)
- `1.2345678` (more than 6 decimal digits)

#### String Literals
- **Format**: Enclosed in double quotes `"`
- **Escape Sequences**: `\"`, `\\`, `\n`, `\t`, `\r`

**Examples:**
```nexus
"Hello, World!"
"Path: C:\\Users\\Data"
"She said \"Hello\""
"Line 1\nLine 2"
```

#### Character Literals
- **Format**: Single character enclosed in single quotes `'`
- **Escape Sequences**: `\'`, `\\`, `\n`, `\t`, `\r`

**Examples:**
```nexus
'A'
'5'
'\n'
'\t'
'\\'
```

#### Boolean Literals
- **Values**: `true`, `false`
- Case-sensitive (lowercase only)

**Examples:**
```nexus
declare Is_valid = true;
declare Is_empty = false;
```

### 4. Operators

#### Arithmetic Operators (Precedence: High to Low)

| Operator | Description | Example |
|----------|-------------|---------|
| `**` | Exponentiation | `2 ** 3` = 8 |
| `*` | Multiplication | `5 * 3` = 15 |
| `/` | Division | `10 / 2` = 5 |
| `%` | Modulo | `10 % 3` = 1 |
| `+` | Addition | `5 + 3` = 8 |
| `-` | Subtraction | `5 - 3` = 2 |

#### Relational Operators

| Operator | Description | Example |
|----------|-------------|---------|
| `==` | Equal to | `X == 10` |
| `!=` | Not equal to | `X != 5` |
| `<` | Less than | `X < 100` |
| `<=` | Less than or equal | `X <= 50` |
| `>` | Greater than | `X > 0` |
| `>=` | Greater than or equal | `X >= 10` |

#### Logical Operators

| Operator | Description | Example |
|----------|-------------|---------|
| `&&` | Logical AND | `true && false` = false |
| `\|\|` | Logical OR | `true \|\| false` = true |
| `!` | Logical NOT | `!true` = false |

#### Assignment Operators

| Operator | Description | Example |
|----------|-------------|---------|
| `=` | Simple assignment | `X = 10` |
| `+=` | Add and assign | `X += 5` ≡ `X = X + 5` |
| `-=` | Subtract and assign | `X -= 3` ≡ `X = X - 3` |
| `*=` | Multiply and assign | `X *= 2` ≡ `X = X * 2` |
| `/=` | Divide and assign | `X /= 4` ≡ `X = X / 4` |

#### Increment/Decrement Operators

| Operator | Description | Example |
|----------|-------------|---------|
| `++` | Increment | `X++` or `++X` |
| `--` | Decrement | `X--` or `--X` |

### 5. Punctuators

| Symbol | Description |
|--------|-------------|
| `(` `)` | Parentheses |
| `{` `}` | Braces (code blocks) |
| `[` `]` | Brackets (arrays) |
| `,` | Comma (separator) |
| `;` | Semicolon (statement terminator) |
| `:` | Colon |

### 6. Comments

#### Single-line Comments
- **Syntax**: `##` followed by comment text
- Continues until end of line

**Example:**
```nexus
## This is a single-line comment
declare X = 10;  ## Inline comment
```

#### Multi-line Comments
- **Syntax**: Starts with `#*` and ends with `*#`
- Can span multiple lines
- Can contain any text except the closing sequence

**Example:**
```nexus
#*
This is a multi-line comment.
It can span multiple lines.
Everything here is ignored.
*#
```

---

## Sample Programs

### Sample 1: Hello World
```nexus
start
    output "Hello, World!";
finish
```

### Sample 2: Variable Operations
```nexus
start
    declare X = 10;
    declare Y = 20;
    declare Sum = X + Y;
    
    output "Sum: ";
    output Sum;
finish
```

### Sample 3: Conditional Logic
```nexus
start
    declare Age;
    input Age;
    
    condition (Age >= 18) {
        output "You are an adult";
    } else {
        output "You are a minor";
    }
finish
```

### Sample 4: Loop Example
```nexus
start
    declare I = 0;
    
    loop {
        output I;
        I++;
        
        condition (I >= 10) {
            break;
        }
    }
    
    output "Loop finished";
finish
```

### Sample 5: Function Example
```nexus
start
    declare Result = Calculate_factorial(5);
    output Result;
finish

function Calculate_factorial(N) {
    declare Fact = 1;
    declare I = 1;
    
    loop {
        condition (I > N) {
            break;
        }
        
        Fact *= I;
        I++;
    }
    
    return Fact;
}
```

### Sample 6: Array Operations
```nexus
start
    declare Numbers[10];
    declare I = 0;
    
    ## Initialize array
    loop {
        condition (I >= 10) {
            break;
        }
        
        Numbers[I] = I * 2;
        I++;
    }
    
    ## Print array
    I = 0;
    loop {
        condition (I >= 10) {
            break;
        }
        
        output Numbers[I];
        I++;
    }
finish
```

### Sample 7: Complex Expression
```nexus
start
    declare A = 5;
    declare B = 10;
    declare C = 15;
    
    ## Calculate quadratic expression
    declare Result = A * B ** 2 + B * C - C / A;
    
    output "Result: ";
    output Result;
    
    ## Comparison
    condition (Result > 100 && Result < 1000) {
        output "Result is in range";
    } else {
        output "Result is out of range";
    }
finish
```

---

## Compilation and Execution

### Prerequisites
- Java JDK 8 or higher
- JFlex (for JFlex implementation)

### Manual Scanner Compilation

```bash
# Navigate to src directory
cd src

# Compile all Java files
javac *.java

# Run the manual scanner
java ManualScanner ../tests/test1.nex
```

### JFlex Scanner Compilation

```bash
# Navigate to src directory
cd src

# Generate Yylex.java from Scanner.flex
jflex Scanner.flex

# Compile all Java files
javac *.java

# Run the JFlex scanner
java Yylex ../tests/test1.nex
```

### Running All Tests

```bash
# Test all files
for file in ../tests/*.nex; do
    echo "Testing: $file"
    java ManualScanner "$file"
    echo "-----------------------------------"
done
```

---

## Project Structure

```
nexus-compiler/
│
├── src/
│   ├── ManualScanner.java       # Manual DFA-based scanner
│   ├── Token.java                # Token class
│   ├── TokenType.java            # Token type enumeration
│   ├── SymbolTable.java          # Symbol table implementation
│   ├── ErrorHandler.java         # Error detection and reporting
│   ├── Scanner.flex              # JFlex specification
│   └── Yylex.java                # Generated by JFlex
│
├── docs/
│   ├── Automata_Design.pdf       # NFA/DFA diagrams and transition tables
│   ├── Comparison.pdf            # Scanner comparison document
│   ├── README.md                 # This file
│   └── LanguageGrammar.txt       # Formal grammar specification
│
├── tests/
│   ├── test1.nex                 # All valid tokens
│   ├── test2.nex                 # Complex expressions
│   ├── test3.nex                 # Strings/chars with escapes
│   ├── test4.nex                 # Lexical errors
│   ├── test5.nex                 # Comments
│   └── TestResults.txt           # Test execution results
│
└── README.md                     # Project readme (this file)
```

---

## Implementation Details

### Token Recognition Order

The scanner checks patterns in the following priority order (critical for correct tokenization):

1. **Multi-line comments** (`#*...*#`)
2. **Single-line comments** (`##...`)
3. **Multi-character operators** (`**`, `==`, `!=`, `<=`, `>=`, `&&`, `||`, `++`, `--`, `+=`, `-=`, `*=`, `/=`)
4. **Keywords** (start, finish, loop, etc.)
5. **Boolean literals** (true, false)
6. **Identifiers** ([A-Z][a-z0-9_]{0,30})
7. **Floating-point literals** (must come before integers)
8. **Integer literals**
9. **String literals**
10. **Character literals**
11. **Single-character operators** (+, -, *, /, %, <, >, =, !)
12. **Punctuators** (, ), {, }, [, ], ,, ;, :)
13. **Whitespace** (skip but track line numbers)

### DFA-Based Approach

The manual scanner implements a deterministic finite automaton (DFA) approach for each token type:

- **State-based recognition**: Each token type has its own recognition logic
- **Longest match**: Always matches the longest possible token
- **Backtracking**: Supports backtracking when needed (e.g., distinguishing floats from integers)
- **Error recovery**: Continues scanning after errors to report all issues

### Symbol Table

The symbol table stores information about identifiers:
- Identifier name
- Type (currently just "IDENTIFIER")
- First occurrence (line number)
- Frequency (number of occurrences)

### Error Handling

The error handler detects and reports:
- Invalid characters
- Malformed literals
- Invalid identifiers
- Unterminated strings/chars
- Unclosed multi-line comments
- Invalid escape sequences

All errors include:
- Error type
- Line and column numbers
- Problematic lexeme
- Descriptive reason

---

## Testing

### Test Coverage

1. **test1.nex**: Comprehensive test of all valid token types
2. **test2.nex**: Complex expressions and nested structures
3. **test3.nex**: Extensive string and character literal testing
4. **test4.nex**: Various lexical error scenarios
5. **test5.nex**: Comprehensive comment testing

### Expected Output

The scanner produces:
1. **Token List**: All recognized tokens with line/column information
2. **Statistics**: Token counts, lines processed, comments removed
3. **Symbol Table**: Unique identifiers with occurrence information
4. **Error Report**: Detailed error information (if any errors found)

---

## Features

### Implemented Features
- ✅ Complete DFA-based manual scanner
- ✅ JFlex-based scanner for validation
- ✅ All token types from specification
- ✅ Comprehensive error handling
- ✅ Symbol table with identifier tracking
- ✅ Detailed statistics reporting
- ✅ Proper escape sequence handling
- ✅ Comment removal and tracking
- ✅ Line and column tracking
- ✅ Multi-character operator recognition

### Design Decisions

1. **Uppercase Identifiers**: Enforces code readability and distinguishes identifiers from keywords
2. **Strict Float Format**: Prevents precision ambiguity
3. **Limited Escape Sequences**: Keeps the language simple while supporting essential formatting
4. **Comment Syntax**: Uses `##` and `#*...*#` to avoid conflicts with typical operators
5. **Case-Sensitive Keywords**: Maintains consistency and prevents ambiguity

---

## Future Enhancements

Potential extensions for future assignments:
- Semantic analyzer
- Parser (syntax analysis)
- Intermediate code generation
- Optimization passes
- Code generation

---

## License

This project is created for educational purposes as part of CS4031 - Compiler Construction course.

---

## Acknowledgments

- Course: CS4031 - Compiler Construction
- Assignment: 01 - Lexical Analyzer Implementation
- Semester: Spring 2026
- Textbook: Compilers: Principles, Techniques, and Tools (Dragon Book) by Aho, Lam, Sethi, and Ullman

---

**Last Updated**: February 2026