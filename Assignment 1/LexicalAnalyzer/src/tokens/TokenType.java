package tokens;

/**
 * TokenType Enumeration
 * Defines all token types recognized by the NEXUS lexical analyzer
 * 
 * @author Ruhab (23i-0559), Hasan (23i-0698)
 * @course CS4031 - Compiler Construction
 * @assignment Assignment 1 - Lexical Analyzer
 */

public enum TokenType {
    // Keywords
    KEYWORD,
    
    // Identifiers
    IDENTIFIER,
    
    // Literals
    INTEGER_LITERAL,
    FLOAT_LITERAL,
    STRING_LITERAL,
    CHAR_LITERAL,
    BOOLEAN_LITERAL,
    
    // Operators
    ARITHMETIC_OP,      // +, -, *, /, %, **
    RELATIONAL_OP,      // ==, !=, <, >, <=, >=
    LOGICAL_OP,         // &&, ||, !
    ASSIGNMENT_OP,      // =, +=, -=, *=, /=
    INCREMENT_OP,       // ++
    DECREMENT_OP,       // --
    
    // Punctuators
    PUNCTUATOR,         // ( ) { } [ ] , ; :
    
    // Comments (for tracking purposes)
    SINGLE_LINE_COMMENT,
    MULTI_LINE_COMMENT,
    
    // Whitespace
    WHITESPACE,
    
    // Error
    ERROR,
    
    // End of file
    EOF
}