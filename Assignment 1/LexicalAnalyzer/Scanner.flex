/* Scanner.flex - JFlex Specification for Custom Language Lexical Analyzer */

/* User Code Section - imports and helper code */
import java.io.*;

%%

/* Options and Declarations Section */
%class Yylex
%public
%unicode
%line
%column
%type Token

/* State declarations for handling multi-line comments */
%state MULTILINE_COMMENT

%{
    /* Helper fields */
    private int commentStartLine;
    private int commentStartColumn;
    private StringBuilder commentBuffer;
    
    /* Error handler for reporting lexical errors */
    private ErrorHandler errorHandler = new ErrorHandler();
    
    /**
     * Helper method to create tokens
     */
    private Token createToken(TokenType type, String lexeme) {
        return new Token(type, lexeme, yyline + 1, yycolumn + 1);
    }
    
    /**
     * Helper method to create tokens with custom lexeme
     */
    private Token token(TokenType type) {
        return createToken(type, yytext());
    }
    
    /**
     * Get the error handler
     */
    public ErrorHandler getErrorHandler() {
        return errorHandler;
    }
%}

/* Macro Definitions */
/* Whitespace */
WhiteSpace = [ \t\r\n]+
LineTerminator = \r|\n|\r\n

/* Comments */
SingleLineComment = "##"[^\r\n]*
MultiLineCommentStart = "#*"
MultiLineCommentEnd = "*#"

/* Letters and Digits */
UpperLetter = [A-Z]
LowerLetter = [a-z]
Letter = {UpperLetter}|{LowerLetter}
Digit = [0-9]
Underscore = "_"

/* Identifiers - must start with uppercase */
Identifier = {UpperLetter}({LowerLetter}|{Digit}|{Underscore}){0,30}

/* Integer Literals */
Sign = [+-]
Integer = {Sign}?{Digit}+

/* Floating-Point Literals */
DecimalPart = "."{Digit}{1,6}
Exponent = [eE]{Sign}?{Digit}+
Float = {Sign}?{Digit}+{DecimalPart}{Exponent}?

/* String Literals */
StringChar = [^\"\\\n]
EscapeSeq = \\([\"\\\ntr])
StringLiteral = \"({StringChar}|{EscapeSeq})*\"

/* Character Literals */
CharChar = [^'\\\n]
CharEscape = \\(['\\\ntr])
CharLiteral = '({CharChar}|{CharEscape})'

/* Boolean Literals */
BooleanLiteral = "true"|"false"

/* Keywords */
Keyword = "start"|"finish"|"loop"|"condition"|"declare"|"output"|"input"|"function"|"return"|"break"|"continue"|"else"

/* Multi-character Operators */
PowerOp = "**"
EqualOp = "=="
NotEqualOp = "!="
LessEqualOp = "<="
GreaterEqualOp = ">="
AndOp = "&&"
OrOp = "||"
IncrementOp = "++"
DecrementOp = "--"
PlusAssignOp = "+="
MinusAssignOp = "-="
MultAssignOp = "*="
DivAssignOp = "/="

%%

/* Lexical Rules Section */

/* CRITICAL: Pattern matching priority as specified */

<YYINITIAL> {
    /* 1. Multi-line comments (highest priority) */
    {MultiLineCommentStart}  { 
        commentStartLine = yyline + 1;
        commentStartColumn = yycolumn + 1;
        commentBuffer = new StringBuilder(yytext());
        yybegin(MULTILINE_COMMENT); 
    }
    
    /* 2. Single-line comments */
    {SingleLineComment}      { /* Skip single-line comments */ }
    
    /* 3. Multi-character operators (BEFORE single-char operators!) */
    {PowerOp}                { return token(TokenType.POWER); }
    {EqualOp}                { return token(TokenType.EQUAL); }
    {NotEqualOp}             { return token(TokenType.NOT_EQUAL); }
    {LessEqualOp}            { return token(TokenType.LESS_EQUAL); }
    {GreaterEqualOp}         { return token(TokenType.GREATER_EQUAL); }
    {AndOp}                  { return token(TokenType.AND); }
    {OrOp}                   { return token(TokenType.OR); }
    {IncrementOp}            { return token(TokenType.INCREMENT); }
    {DecrementOp}            { return token(TokenType.DECREMENT); }
    {PlusAssignOp}           { return token(TokenType.PLUS_ASSIGN); }
    {MinusAssignOp}          { return token(TokenType.MINUS_ASSIGN); }
    {MultAssignOp}           { return token(TokenType.MULT_ASSIGN); }
    {DivAssignOp}            { return token(TokenType.DIV_ASSIGN); }
    
    /* 4. Keywords (exact match, case-sensitive) */
    "start"                  { return token(TokenType.KEYWORD_START); }
    "finish"                 { return token(TokenType.KEYWORD_FINISH); }
    "loop"                   { return token(TokenType.KEYWORD_LOOP); }
    "condition"              { return token(TokenType.KEYWORD_CONDITION); }
    "declare"                { return token(TokenType.KEYWORD_DECLARE); }
    "output"                 { return token(TokenType.KEYWORD_OUTPUT); }
    "input"                  { return token(TokenType.KEYWORD_INPUT); }
    "function"               { return token(TokenType.KEYWORD_FUNCTION); }
    "return"                 { return token(TokenType.KEYWORD_RETURN); }
    "break"                  { return token(TokenType.KEYWORD_BREAK); }
    "continue"               { return token(TokenType.KEYWORD_CONTINUE); }
    "else"                   { return token(TokenType.KEYWORD_ELSE); }
    
    /* 5. Boolean literals */
    {BooleanLiteral}         { return token(TokenType.BOOLEAN_LITERAL); }
    
    /* 6. Identifiers (must start with uppercase) */
    {Identifier}             {
        String id = yytext();
        if (id.length() > 31) {
            errorHandler.reportIdentifierTooLong(yyline + 1, yycolumn + 1, id);
            return createToken(TokenType.ERROR, id);
        }
        return token(TokenType.IDENTIFIER);
    }
    
    /* 7. Floating-point literals (BEFORE integers!) */
    {Float}                  { 
        String floatStr = yytext();
        // Check decimal places
        int dotIndex = floatStr.indexOf('.');
        int eIndex = floatStr.toLowerCase().indexOf('e');
        int decimalEnd = (eIndex != -1) ? eIndex : floatStr.length();
        int decimalPlaces = decimalEnd - dotIndex - 1;
        
        if (decimalPlaces > 6) {
            errorHandler.reportTooManyDecimals(yyline + 1, yycolumn + 1, floatStr);
            return createToken(TokenType.ERROR, floatStr);
        }
        return token(TokenType.FLOAT_LITERAL); 
    }
    
    /* 8. Integer literals */
    {Integer}                { return token(TokenType.INTEGER_LITERAL); }
    
    /* 9. String and character literals */
    {StringLiteral}          { return token(TokenType.STRING_LITERAL); }
    {CharLiteral}            { return token(TokenType.CHAR_LITERAL); }
    
    /* Unterminated string literal */
    \"({StringChar}|{EscapeSeq})*  {
        errorHandler.reportUnterminatedString(yyline + 1, yycolumn + 1, yytext());
        return createToken(TokenType.ERROR, yytext());
    }
    
    /* Unterminated character literal */
    '({CharChar}|{CharEscape})?  {
        errorHandler.reportUnterminatedChar(yyline + 1, yycolumn + 1, yytext());
        return createToken(TokenType.ERROR, yytext());
    }
    
    /* 10. Single-character operators */
    "+"                      { return token(TokenType.PLUS); }
    "-"                      { return token(TokenType.MINUS); }
    "*"                      { return token(TokenType.MULTIPLY); }
    "/"                      { return token(TokenType.DIVIDE); }
    "%"                      { return token(TokenType.MODULO); }
    "="                      { return token(TokenType.ASSIGN); }
    "<"                      { return token(TokenType.LESS_THAN); }
    ">"                      { return token(TokenType.GREATER_THAN); }
    "!"                      { return token(TokenType.NOT); }
    
    /* 11. Punctuators */
    "("                      { return token(TokenType.LEFT_PAREN); }
    ")"                      { return token(TokenType.RIGHT_PAREN); }
    "{"                      { return token(TokenType.LEFT_BRACE); }
    "}"                      { return token(TokenType.RIGHT_BRACE); }
    "["                      { return token(TokenType.LEFT_BRACKET); }
    "]"                      { return token(TokenType.RIGHT_BRACKET); }
    ","                      { return token(TokenType.COMMA); }
    ";"                      { return token(TokenType.SEMICOLON); }
    ":"                      { return token(TokenType.COLON); }
    
    /* 12. Whitespace (skip) */
    {WhiteSpace}             { /* Skip whitespace */ }
    
    /* Invalid identifier starting with lowercase */
    {LowerLetter}({Letter}|{Digit}|{Underscore})* {
        errorHandler.reportInvalidIdentifier(yyline + 1, yycolumn + 1, yytext(),
            "Identifier must start with an uppercase letter");
        return createToken(TokenType.ERROR, yytext());
    }
    
    /* Invalid characters */
    .                        { 
        errorHandler.reportInvalidCharacter(yyline + 1, yycolumn + 1, yytext().charAt(0));
        return createToken(TokenType.ERROR, yytext());
    }
}

<MULTILINE_COMMENT> {
    {MultiLineCommentEnd}    { 
        commentBuffer.append(yytext());
        yybegin(YYINITIAL);
        /* Comment consumed, continue scanning */
    }
    
    [^*]+                    { commentBuffer.append(yytext()); }
    "*"                      { commentBuffer.append(yytext()); }
    
    <<EOF>>                  {
        errorHandler.reportUnclosedComment(commentStartLine, commentStartColumn, 
                                          commentBuffer.toString());
        return createToken(TokenType.ERROR, commentBuffer.toString());
    }
}

/* End of file */
<<EOF>>                      { return createToken(TokenType.EOF, ""); }
