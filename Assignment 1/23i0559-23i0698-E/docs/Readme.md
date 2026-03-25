# NEXUS Programming Language
## Lexical Analyzer Implementation

**Course:** CS4031 - Compiler Construction  
**Assignment:** Assignment 1 - Lexical Analyzer  
**Team Members:**  
- Ruhab (23i-0559)
- Hasan (23i-0698)

**File Extension:** `.nexus`

---

## Table of Contents
1. [Language Overview](#language-overview)
2. [Keywords](#keywords)
3. [Identifiers](#identifiers)
4. [Literals](#literals)
5. [Operators](#operators)
6. [Punctuators](#punctuators)
7. [Comments](#comments)
8. [Sample Programs](#sample-programs)
9. [Compilation and Execution](#compilation-and-execution)
10. [Project Structure](#project-structure)

---

## Language Overview

NEXUS is a custom programming language designed for educational purposes in compiler construction. It features:
- Case-sensitive keywords
- Unique identifier rules (must start with uppercase)
- Support for multiple data types
- Rich operator set
- Single-line and multi-line comments

---

## Keywords

NEXUS has 12 reserved keywords (all case-sensitive):

| Keyword | Meaning | Usage |
|---------|---------|-------|
| `start` | Program entry point | Marks the beginning of the program |
| `finish` | Program exit point | Marks the end of the program |
| `loop` | Loop construct | Creates iteration: `loop condition { ... }` |
| `condition` | Conditional statement | If-statement: `condition expr { ... }` |
| `declare` | Variable declaration | Declares a variable: `declare Variable_name;` |
| `output` | Output statement | Prints to console: `output expression;` |
| `input` | Input statement | Reads from console: `input Variable_name;` |
| `function` | Function definition | Defines a function: `function Name(params) { ... }` |
| `return` | Return statement | Returns from function: `return value;` |
| `break` | Break statement | Exits loop: `break;` |
| `continue` | Continue statement | Skips to next iteration: `continue;` |
| `else` | Else clause | Alternative branch: `else { ... }` |

**Example:**
```nexus
start
    declare Counter;
    Counter = 0;
    
    loop Counter < 10
        output Counter;
        Counter++;
    
finish
```

---

## Identifiers

**Rules:**
- Must start with an uppercase letter (A-Z)
- Can be followed by lowercase letters (a-z), digits (0-9), or underscores (_)
- Maximum length: 31 characters
- Case-sensitive

**Regular Expression:** `[A-Z][a-z0-9_]{0,30}`

**Valid Examples:**
- `Counter`
- `Variable_name`
- `X`
- `Total_sum_2024`
- `My_var123`

**Invalid Examples:**
- `counter` (starts with lowercase)
- `Variable` (contains uppercase after first character - wait, this is actually valid!)
- `2Count` (starts with digit)
- `myVariable` (starts with lowercase)
- `This_is_a_very_long_identifier_name_that_exceeds_the_maximum_length_limit` (> 31 chars)

**Corrected Valid Examples:**
- `Counter`
- `Variable_name`
- `X`
- `Total_sum_2024`
- `Temp`

**Corrected Invalid Examples:**
- `counter` (starts with lowercase)
- `2Count` (starts with digit)
- `myVariable` (starts with lowercase)
- `Variable` (actually valid)
- `ALLCAPS` (contains uppercase letters after first - invalid per spec)

---

## Literals

### 1. Integer Literals
**Format:** Optional sign followed by digits  
**Regular Expression:** `[+-]?[0-9]+`

**Examples:**
- `42`
- `+100`
- `-567`
- `0`

**Invalid:**
- `12.34` (this is a float)
- `1,000` (no commas allowed)

### 2. Floating-Point Literals
**Format:** Optional sign, digits, decimal point, 1-6 decimal digits, optional exponent  
**Regular Expression:** `[+-]?[0-9]+\.[0-9]{1,6}([eE][+-]?[0-9]+)?`

**Examples:**
- `3.14`
- `+2.5`
- `-0.123456`
- `1.5e10`
- `2.0E-3`
- `6.022e23`

**Invalid:**
- `3.` (no digits after decimal)
- `.14` (no digits before decimal)
- `1.2345678` (more than 6 decimal places)
- `1.5e` (incomplete exponent)

### 3. String Literals
**Format:** Double-quoted with escape sequences  
**Regular Expression:** `"([^"\\\n]|\\["\\ntr])*"`

**Supported Escape Sequences:**
- `\"` - Double quote
- `\\` - Backslash
- `\n` - Newline
- `\t` - Tab
- `\r` - Carriage return

**Examples:**
```nexus
"Hello, World!"
"She said \"Hello\""
"C:\\Users\\Student\\Desktop"
"Line 1\nLine 2\nLine 3"
"Column1\tColumn2\tColumn3"
```

### 4. Character Literals
**Format:** Single-quoted character or escape sequence  
**Regular Expression:** `'([^'\\\n]|\\['\\ntr])'`

**Examples:**
- `'A'`
- `'z'`
- `'9'`
- `'\''` (single quote)
- `'\\'` (backslash)
- `'\n'` (newline)
- `'\t'` (tab)

### 5. Boolean Literals
**Values:** `True`, `False` (case-sensitive)

**Examples:**
```nexus
declare Is_valid;
Is_valid = True;

declare Is_done;
Is_done = False;
```

---

## Operators

### 1. Arithmetic Operators (Precedence: High to Low)
| Operator | Description | Example |
|----------|-------------|---------|
| `**` | Exponentiation | `X ** 2` |
| `*` | Multiplication | `X * Y` |
| `/` | Division | `X / Y` |
| `%` | Modulus | `X % Y` |
| `+` | Addition | `X + Y` |
| `-` | Subtraction | `X - Y` |

### 2. Relational Operators
| Operator | Description | Example |
|----------|-------------|---------|
| `==` | Equal to | `X == Y` |
| `!=` | Not equal to | `X != Y` |
| `<` | Less than | `X < Y` |
| `>` | Greater than | `X > Y` |
| `<=` | Less than or equal | `X <= Y` |
| `>=` | Greater than or equal | `X >= Y` |

### 3. Logical Operators
| Operator | Description | Example |
|----------|-------------|---------|
| `&&` | Logical AND | `X && Y` |
| `\|\|` | Logical OR | `X \|\| Y` |
| `!` | Logical NOT | `!X` |

### 4. Assignment Operators
| Operator | Description | Example |
|----------|-------------|---------|
| `=` | Simple assignment | `X = 5` |
| `+=` | Add and assign | `X += 5` |
| `-=` | Subtract and assign | `X -= 5` |
| `*=` | Multiply and assign | `X *= 5` |
| `/=` | Divide and assign | `X /= 5` |

### 5. Increment/Decrement Operators
| Operator | Description | Example |
|----------|-------------|---------|
| `++` | Increment | `Counter++` |
| `--` | Decrement | `Counter--` |

**Operator Precedence (Highest to Lowest):**
1. `**` (Exponentiation)
2. `*`, `/`, `%` (Multiplication, Division, Modulus)
3. `+`, `-` (Addition, Subtraction)
4. `<`, `>`, `<=`, `>=` (Relational)
5. `==`, `!=` (Equality)
6. `&&` (Logical AND)
7. `||` (Logical OR)
8. `=`, `+=`, `-=`, `*=`, `/=` (Assignment)

---

## Punctuators

| Symbol | Name | Usage |
|--------|------|-------|
| `(` `)` | Parentheses | Function calls, grouping expressions |
| `{` `}` | Braces | Code blocks |
| `[` `]` | Brackets | Array indexing |
| `,` | Comma | Separator in function parameters |
| `;` | Semicolon | Statement terminator |
| `:` | Colon | Label or type annotation |

---

## Comments

### Single-Line Comments
**Syntax:** `## comment text`  
**Description:** Starts with `##` and continues to end of line

**Example:**
```nexus
declare Counter;  ## This is a comment
## This entire line is a comment
```

### Multi-Line Comments
**Syntax:** `#* comment text *#`  
**Description:** Starts with `#*` and ends with `*#`, can span multiple lines

**Example:**
```nexus
#* This is a multi-line comment
   that spans multiple lines
   and can contain any text *#
```

**Note:** Multi-line comments cannot be nested in the basic implementation.

---

## Sample Programs

### Program 1: Hello World
```nexus
## Hello World Program
start
    declare Message;
    Message = "Hello, NEXUS World!";
    output Message;
finish
```

### Program 2: Factorial Calculator
```nexus
## Factorial Calculator
start
    declare N;
    declare Result;
    declare I;
    
    ## Input
    output "Enter a number: ";
    input N;
    
    ## Calculate factorial
    Result = 1;
    I = 1;
    
    loop I <= N
        Result *= I;
        I++;
    
    ## Output result
    output "Factorial is: ";
    output Result;
finish
```

### Program 3: Fibonacci Sequence
```nexus
## Fibonacci Sequence Generator
start
    function Fibonacci(N)
        declare A;
        declare B;
        declare C;
        declare I;
        
        A = 0;
        B = 1;
        
        condition N == 0
            return A;
        
        condition N == 1
            return B;
        
        I = 2;
        loop I <= N
            C = A + B;
            A = B;
            B = C;
            I++;
        
        return B;
    
    ## Main program
    declare Count;
    declare I;
    declare Fib_num;
    
    Count = 10;
    I = 0;
    
    output "First 10 Fibonacci numbers:";
    
    loop I < Count
        Fib_num = Fibonacci(I);
        output Fib_num;
        I++;
    
finish
```

### Program 4: Temperature Converter
```nexus
## Temperature Converter (Celsius to Fahrenheit)
start
    declare Celsius;
    declare Fahrenheit;
    declare Conversion_factor;
    declare Offset;
    
    Conversion_factor = 1.8;
    Offset = 32;
    
    output "Enter temperature in Celsius: ";
    input Celsius;
    
    ## Formula: F = C * 1.8 + 32
    Fahrenheit = Celsius * Conversion_factor + Offset;
    
    output "Temperature in Fahrenheit: ";
    output Fahrenheit;
finish
```

### Program 5: Array Operations
```nexus
## Array Sum and Average
start
    declare Numbers[10];
    declare I;
    declare Sum;
    declare Average;
    declare Count;
    
    Count = 10;
    Sum = 0;
    I = 0;
    
    ## Fill array
    loop I < Count
        Numbers[I] = I * 2 + 1;
        I++;
    
    ## Calculate sum
    I = 0;
    loop I < Count
        Sum += Numbers[I];
        I++;
    
    ## Calculate average
    Average = Sum / Count;
    
    output "Sum: ";
    output Sum;
    output "Average: ";
    output Average;
finish
```

---

## Compilation and Execution

### Prerequisites
- Java Development Kit (JDK) 8 or higher
- JFlex 1.8.2 or higher (for JFlex implementation)

### Manual Scanner

#### Compilation
```bash
cd src
javac TokenType.java Token.java SymbolTable.java ErrorHandler.java ManualScanner.java
```

#### Execution
```bash
java ManualScanner ../tests/test1.nexus
```

### JFlex Scanner

#### Generate Scanner
```bash
cd src
jflex Scanner.flex
```

This generates `Yylex.java`.

#### Compilation
```bash
javac TokenType.java Token.java SymbolTable.java ErrorHandler.java Yylex.java
```

#### Creating a Test Driver
Create a file `JFlexDriver.java`:
```java
import java.io.*;

public class JFlexDriver {
    public static void main(String[] args) throws Exception {
        Yylex scanner = new Yylex(new FileReader(args[0]));
        Token token;
        
        while ((token = scanner.yylex()) != null) {
            if (token.getType() != TokenType.EOF) {
                System.out.println(token);
            }
        }
        
        scanner.getSymbolTable().display();
        scanner.getErrorHandler().displayErrors();
    }
}
```

#### Execution
```bash
javac JFlexDriver.java
java JFlexDriver ../tests/test1.nexus
```

### Running All Tests
```bash
# Manual Scanner
for test in ../tests/*.nexus; do
    echo "Testing $test"
    java ManualScanner "$test"
    echo "---"
done

# JFlex Scanner (after creating driver)
for test in ../tests/*.nexus; do
    echo "Testing $test"
    java JFlexDriver "$test"
    echo "---"
done
```

---

## Project Structure

```
23i-0559-23i-0698-A/
├── src/
│   ├── ManualScanner.java      # Manual DFA-based scanner
│   ├── Token.java               # Token class
│   ├── TokenType.java           # Token type enumeration
│   ├── SymbolTable.java         # Symbol table implementation
│   ├── ErrorHandler.java        # Error handling
│   ├── Scanner.flex             # JFlex specification
│   └── Yylex.java               # Generated JFlex scanner
├── docs/
│   ├── Automata_Design.pdf      # NFA/DFA designs
│   ├── Comparison.pdf           # Scanner comparison
│   └── LanguageGrammar.txt      # Formal grammar
├── tests/
│   ├── test1.nexus              # All valid tokens
│   ├── test2.nexus              # Complex expressions
│   ├── test3.nexus              # String/char escapes
│   ├── test4.nexus              # Lexical errors
│   ├── test5.nexus              # Comments
│   └── TestResults.txt          # Test execution results
└── README.md                    # This file
```

---

## Features Implemented

### Part 1: Manual Scanner (60 marks)
- ✅ Regular expressions for all token types
- ✅ NFA diagrams for required tokens
- ✅ Minimized DFA with transition tables
- ✅ DFA-based token recognition
- ✅ Longest match principle
- ✅ Whitespace handling with line/column tracking
- ✅ Formatted token output
- ✅ Comprehensive statistics
- ✅ Symbol table with identifier tracking

### Part 2: JFlex Implementation (30 marks)
- ✅ Complete JFlex specification
- ✅ Compatible token class
- ✅ Output comparison documentation

### Part 3: Error Handling (10 marks)
- ✅ Invalid character detection
- ✅ Malformed literal detection
- ✅ Invalid identifier detection
- ✅ Unclosed comment detection
- ✅ Formatted error reporting
- ✅ Error recovery and continuation

### Bonus Features
- ✅ GitHub repository with commit history
- ✅ Comprehensive test suite
- ✅ Well-documented code
- ✅ Professional output formatting

---

## Pattern Matching Priority

The scanner checks patterns in this order to avoid ambiguity:

1. Multi-line comments (`#* ... *#`)
2. Single-line comments (`##...`)
3. Multi-character operators (`**`, `==`, `!=`, `<=`, `>=`, `&&`, `||`, `++`, `--`, `+=`, `-=`, `*=`, `/=`)
4. Keywords (`start`, `finish`, etc.)
5. Boolean literals (`True`, `False`)
6. Identifiers (`[A-Z][a-z0-9_]{0,30}`)
7. Floating-point literals (with decimal point)
8. Integer literals
9. String literals
10. Character literals
11. Single-character operators
12. Punctuators
13. Whitespace

---

## Error Messages

The scanner provides detailed error messages with:
- Error type (INVALID_CHARACTER, MALFORMED_FLOAT, etc.)
- Line and column number
- Problematic lexeme
- Descriptive reason

**Example:**
```
[INVALID_CHARACTER] Line 5, Col 12: '@' - Invalid character '@' (ASCII: 64)
[UNTERMINATED_STRING] Line 8, Col 15: "Hello - String literal not terminated before end of line
[INVALID_IDENTIFIER] Line 10, Col 5: 'counter' - Identifier must start with uppercase letter
```

---

## Notes

- All keywords are case-sensitive
- Identifiers MUST start with uppercase letter
- Maximum identifier length is 31 characters
- Floating-point decimals limited to 6 digits
- Comments are removed during scanning
- Whitespace is tracked for line/column numbers
- Symbol table tracks first occurrence and frequency of identifiers

---

## Contact

For questions or issues:
- Ruhab: 23i-0559
- Hasan: 23i-0698

---

## License

This project is created for educational purposes as part of CS4031 - Compiler Construction course.