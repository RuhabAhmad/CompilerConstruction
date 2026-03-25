/**
 * JFlex Scanner Specification for NEXUS Language
 * 
 * @author Ruhab (23i-0559), Hasan (23i-0698)
 * @course CS4031 - Compiler Construction
 * @assignment Assignment 1 - Lexical Analyzer
 */

import java.io.*;

%%

/* ==================== USER CODE SECTION ==================== */
%{
    // Token tracking
    private int tokenCount = 0;
    private int lineCount = 1;
    private int commentCount = 0;
    
    // Symbol table and error handler
    private SymbolTable symbolTable = new SymbolTable();
    private ErrorHandler errorHandler = new ErrorHandler();
    
    // Helper method to create tokens
    private Token createToken(TokenType type, String lexeme) {
        tokenCount++;
        
        // Add identifiers to symbol table
        if (type == TokenType.IDENTIFIER) {
            symbolTable.addIdentifier(lexeme, yyline + 1);
        }
        
        return new Token(type, lexeme, yyline + 1, yycolumn + 1);
    }
    
    // Helper method to report errors
    private Token reportError(String lexeme, String reason) {
        errorHandler.reportError("LEXICAL_ERROR", lexeme, yyline + 1, yycolumn + 1, reason);
        return null;
    }
    
    // Public methods to get statistics
    public int getTokenCount() { return tokenCount; }
    public int getLineCount() { return lineCount; }
    public int getCommentCount() { return commentCount; }
    public SymbolTable getSymbolTable() { return symbolTable; }
    public ErrorHandler getErrorHandler() { return errorHandler; }
%}

/* ==================== OPTIONS AND DECLARATIONS ==================== */
%class Yylex
%public
%line
%column
%type Token

/* ==================== MACRO DEFINITIONS ==================== */

/* Basic character classes */
DIGIT = [0-9]
UPPERCASE = [A-Z]
LOWERCASE = [a-z]
LETTER = [a-zA-Z]
UNDERSCORE = "_"

/* Whitespace */
WHITESPACE = [ \t\r\n]+
NEWLINE = \r|\n|\r\n

/* Keywords - exact match, case-sensitive */
KEYWORD = start|finish|loop|condition|declare|output|input|function|return|break|continue|else

/* Boolean literals */
BOOLEAN = true|false

/* Identifiers: Start with uppercase, followed by lowercase/digits/underscores, max 31 chars */
IDENTIFIER = {UPPERCASE}[a-z0-9_]{0,30}

/* Integer literals: optional sign followed by digits */
INTEGER = [+-]?{DIGIT}+

/* Floating-point literals: optional sign, digits, dot, 1-6 decimals, optional exponent */
FLOAT = [+-]?{DIGIT}+\.{DIGIT}{1,6}([eE][+-]?{DIGIT}+)?

/* String literals with escape sequences */
STRING_CHAR = [^\"\\\n]
ESCAPE_SEQ = \\[\"\\\ntr]
STRING = \"({STRING_CHAR}|{ESCAPE_SEQ})*\"

/* Character literals with escape sequences */
CHAR_CHAR = [^'\\\n]
CHAR_ESCAPE = \\['\\\ntr]
CHAR = '({CHAR_CHAR}|{CHAR_ESCAPE})'

/* Comments */
SINGLE_COMMENT = ##[^\n]*
MULTI_COMMENT = "#*" [^#]* "*#"

/* Operators - Multi-character first */
EXPONENT = \*\*
EQ = ==
NEQ = \!=
LTE = <=
GTE = >=
AND = &&
OR = \|\|
INC = \+\+
DEC = --
PLUS_EQ = \+=
MINUS_EQ = -=
MULT_EQ = \*=
DIV_EQ = \/=

/* Single-character operators */
PLUS = \+
MINUS = -
MULT = \*
DIV = \/
MOD = %
LT = <
GT = >
ASSIGN = =
NOT = \!

/* Punctuators */
LPAREN = \(
RPAREN = \)
LBRACE = \{
RBRACE = \}
LBRACKET = \[
RBRACKET = \]
COMMA = ,
SEMICOLON = ;
COLON = :

%%

/* ==================== LEXICAL RULES ==================== */

/* Comments - Highest priority, consume and don't return token */
{MULTI_COMMENT}     { commentCount++; /* Skip multi-line comments */ }
{SINGLE_COMMENT}    { commentCount++; /* Skip single-line comments */ }

/* Whitespace - Skip but track line numbers */
{NEWLINE}           { /* Skip newline */ }
{WHITESPACE}        { /* Skip whitespace */ }

/* Multi-character operators - Must come before single-character */
{EXPONENT}          { return createToken(TokenType.ARITHMETIC_OP, yytext()); }
{EQ}                { return createToken(TokenType.RELATIONAL_OP, yytext()); }
{NEQ}               { return createToken(TokenType.RELATIONAL_OP, yytext()); }
{LTE}               { return createToken(TokenType.RELATIONAL_OP, yytext()); }
{GTE}               { return createToken(TokenType.RELATIONAL_OP, yytext()); }
{AND}               { return createToken(TokenType.LOGICAL_OP, yytext()); }
{OR}                { return createToken(TokenType.LOGICAL_OP, yytext()); }
{INC}               { return createToken(TokenType.INCREMENT_OP, yytext()); }
{DEC}               { return createToken(TokenType.DECREMENT_OP, yytext()); }
{PLUS_EQ}           { return createToken(TokenType.ASSIGNMENT_OP, yytext()); }
{MINUS_EQ}          { return createToken(TokenType.ASSIGNMENT_OP, yytext()); }
{MULT_EQ}           { return createToken(TokenType.ASSIGNMENT_OP, yytext()); }
{DIV_EQ}            { return createToken(TokenType.ASSIGNMENT_OP, yytext()); }

/* Keywords - Must come before identifiers */
{KEYWORD}           { return createToken(TokenType.KEYWORD, yytext()); }

/* Boolean literals - Must come before identifiers */
{BOOLEAN}           { return createToken(TokenType.BOOLEAN_LITERAL, yytext()); }

/* Identifiers */
{IDENTIFIER}        { return createToken(TokenType.IDENTIFIER, yytext()); }

/* Numeric literals */
{FLOAT}             { return createToken(TokenType.FLOAT_LITERAL, yytext()); }
{INTEGER}           { return createToken(TokenType.INTEGER_LITERAL, yytext()); }

/* String and character literals */
{STRING}            { return createToken(TokenType.STRING_LITERAL, yytext()); }
{CHAR}              { return createToken(TokenType.CHAR_LITERAL, yytext()); }

/* Single-character operators */
{PLUS}              { return createToken(TokenType.ARITHMETIC_OP, yytext()); }
{MINUS}             { return createToken(TokenType.ARITHMETIC_OP, yytext()); }
{MULT}              { return createToken(TokenType.ARITHMETIC_OP, yytext()); }
{DIV}               { return createToken(TokenType.ARITHMETIC_OP, yytext()); }
{MOD}               { return createToken(TokenType.ARITHMETIC_OP, yytext()); }
{LT}                { return createToken(TokenType.RELATIONAL_OP, yytext()); }
{GT}                { return createToken(TokenType.RELATIONAL_OP, yytext()); }
{ASSIGN}            { return createToken(TokenType.ASSIGNMENT_OP, yytext()); }
{NOT}               { return createToken(TokenType.LOGICAL_OP, yytext()); }

/* Punctuators */
{LPAREN}            { return createToken(TokenType.PUNCTUATOR, yytext()); }
{RPAREN}            { return createToken(TokenType.PUNCTUATOR, yytext()); }
{LBRACE}            { return createToken(TokenType.PUNCTUATOR, yytext()); }
{RBRACE}            { return createToken(TokenType.PUNCTUATOR, yytext()); }
{LBRACKET}          { return createToken(TokenType.PUNCTUATOR, yytext()); }
{RBRACKET}          { return createToken(TokenType.PUNCTUATOR, yytext()); }
{COMMA}             { return createToken(TokenType.PUNCTUATOR, yytext()); }
{SEMICOLON}         { return createToken(TokenType.PUNCTUATOR, yytext()); }
{COLON}             { return createToken(TokenType.PUNCTUATOR, yytext()); }

/* Error handling - Catch-all for invalid characters */
.                   { return reportError(yytext(), "Invalid character"); }

/* End of file */
<<EOF>>             { return createToken(TokenType.EOF, ""); }
