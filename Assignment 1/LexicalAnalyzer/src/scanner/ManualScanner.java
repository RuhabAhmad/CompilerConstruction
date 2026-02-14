package scanner;
import java.io.*;
import java.util.*;
import java.util.regex.*;

import tokens.Token;
import tokens.TokenType;
import symboltable.SymbolTable;
import errorhandler.ErrorHandler;

/**
 * ManualScanner Class - DFA-based Lexical Analyzer for Nexus Language
 * CS4031 - Compiler Construction Assignment 01
 * 
 * This scanner implements a DFA-based approach to tokenize Nexus source code.
 * It recognizes all token types specified in the assignment.
 */
public class ManualScanner {
    
    private String input;
    private int position;
    private int line;
    private int column;
    private int lineStart;  // Position where current line starts
    
    private List<Token> tokens;
    private SymbolTable symbolTable;
    private ErrorHandler errorHandler;
    
    // Statistics
    private Map<TokenType, Integer> tokenCounts;
    private int totalTokens;
    private int linesProcessed;
    private int commentsRemoved;
    
    // Keywords set for quick lookup
    private static final Set<String> KEYWORDS = new HashSet<>(Arrays.asList(
        "start", "finish", "loop", "condition", "declare", "output", 
        "input", "function", "return", "break", "continue", "else"
    ));
    
    /**
     * Constructor
     */
    public ManualScanner() {
        this.tokens = new ArrayList<>();
        this.symbolTable = new SymbolTable();
        this.errorHandler = new ErrorHandler();
        this.tokenCounts = new HashMap<>();
        this.totalTokens = 0;
        this.commentsRemoved = 0;
    }
    
    /**
     * Scan a file and tokenize it
     * @param filename Path to the source file
     * @return List of tokens
     */
    public List<Token> scanFile(String filename) throws IOException {
        // Read file content
        StringBuilder content = new StringBuilder();
        try (BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            String line;
            while ((line = reader.readLine()) != null) {
                content.append(line).append('\n');
            }
        }
        
        return scan(content.toString());
    }
    
    /**
     * Scan a string and tokenize it
     * @param source Source code string
     * @return List of tokens
     */
    public List<Token> scan(String source) {
        this.input = source;
        this.position = 0;
        this.line = 1;
        this.column = 1;
        this.lineStart = 0;
        this.tokens.clear();
        this.tokenCounts.clear();
        this.totalTokens = 0;
        this.linesProcessed = 0;
        this.commentsRemoved = 0;
        
        while (position < input.length()) {
            scanToken();
        }
        
        // Add EOF token
        tokens.add(new Token(TokenType.EOF, "", line, column));
        
        // Update statistics
        linesProcessed = line;
        
        return tokens;
    }
    
    /**
     * Scan next token using DFA-based approach
     */
    private void scanToken() {
        char current = peek();
        
        // Check patterns in priority order as specified
        
        // 1. Multi-line comments: #*(. . .)*#
        if (tryMatchMultiLineComment()) return;
        
        // 2. Single-line comments: ##...
        if (tryMatchSingleLineComment()) return;
        
        // 3. Multi-character operators (must come before single-char operators)
        if (tryMatchMultiCharOperators()) return;
        
        // 4. Keywords
        if (Character.isLetter(current) && Character.isUpperCase(current)) {
            if (tryMatchKeywordOrIdentifier()) return;
        }
        
        // 5. Boolean literals (true/false) - handled in identifier matching
        
        // 6. Identifiers - already handled above
        
        // 7. Floating-point literals (must come before integer)
        if (Character.isDigit(current) || current == '+' || current == '-') {
            if (tryMatchNumber()) return;
        }
        
        // 8. String literals
        if (current == '"') {
            if (tryMatchString()) return;
        }
        
        // 9. Character literals
        if (current == '\'') {
            if (tryMatchChar()) return;
        }
        
        // 10. Single-character operators
        if (tryMatchSingleCharOperator()) return;
        
        // 11. Punctuators
        if (tryMatchPunctuator()) return;
        
        // 12. Whitespace
        if (Character.isWhitespace(current)) {
            skipWhitespace();
            return;
        }
        
        // Invalid character - error recovery
        errorHandler.reportInvalidCharacter(line, column, current);
        advance();
    }
    
    /**
     * Try to match a multi-line comment: #*...*#
     */
    private boolean tryMatchMultiLineComment() {
        if (peek() == '#' && peekAhead(1) == '*') {
            int startLine = line;
            int startCol = column;
            StringBuilder comment = new StringBuilder();
            
            advance(); // consume #
            advance(); // consume *
            comment.append("#*");
            
            while (position < input.length()) {
                char c = peek();
                
                if (c == '*' && peekAhead(1) == '#') {
                    comment.append("*#");
                    advance(); // consume *
                    advance(); // consume #
                    commentsRemoved++;
                    return true;
                }
                
                comment.append(c);
                advance();
            }
            
            // Unclosed comment
            errorHandler.reportUnclosedComment(startLine, startCol, comment.toString());
            return true;
        }
        return false;
    }
    
    /**
     * Try to match a single-line comment: ##...
     */
    private boolean tryMatchSingleLineComment() {
        if (peek() == '#' && peekAhead(1) == '#') {
            int startCol = column;
            StringBuilder comment = new StringBuilder();
            
            while (position < input.length() && peek() != '\n') {
                comment.append(peek());
                advance();
            }
            
            commentsRemoved++;
            return true;
        }
        return false;
    }
    
    /**
     * Try to match multi-character operators
     * Priority: **, ==, !=, <=, >=, &&, ||, ++, --, +=, -=, *=, /=
     */
    private boolean tryMatchMultiCharOperators() {
        char c1 = peek();
        char c2 = peekAhead(1);
        
        int startCol = column;
        TokenType type = null;
        String lexeme = null;
        
        // Two-character operators
        if (c1 == '*' && c2 == '*') { type = TokenType.OP_EXPONENT; lexeme = "**"; }
        else if (c1 == '=' && c2 == '=') { type = TokenType.OP_EQUAL; lexeme = "=="; }
        else if (c1 == '!' && c2 == '=') { type = TokenType.OP_NOT_EQUAL; lexeme = "!="; }
        else if (c1 == '<' && c2 == '=') { type = TokenType.OP_LESS_EQUAL; lexeme = "<="; }
        else if (c1 == '>' && c2 == '=') { type = TokenType.OP_GREATER_EQUAL; lexeme = ">="; }
        else if (c1 == '&' && c2 == '&') { type = TokenType.OP_AND; lexeme = "&&"; }
        else if (c1 == '|' && c2 == '|') { type = TokenType.OP_OR; lexeme = "||"; }
        else if (c1 == '+' && c2 == '+') { type = TokenType.OP_INCREMENT; lexeme = "++"; }
        else if (c1 == '-' && c2 == '-') { type = TokenType.OP_DECREMENT; lexeme = "--"; }
        else if (c1 == '+' && c2 == '=') { type = TokenType.OP_PLUS_ASSIGN; lexeme = "+="; }
        else if (c1 == '-' && c2 == '=') { type = TokenType.OP_MINUS_ASSIGN; lexeme = "-="; }
        else if (c1 == '*' && c2 == '=') { type = TokenType.OP_MULT_ASSIGN; lexeme = "*="; }
        else if (c1 == '/' && c2 == '=') { type = TokenType.OP_DIV_ASSIGN; lexeme = "/="; }
        
        if (type != null) {
            advance();
            advance();
            addToken(type, lexeme, line, startCol);
            return true;
        }
        
        return false;
    }
    
    /**
     * Try to match keyword or identifier
     * Keywords: start, finish, loop, condition, declare, output, input, function, return, break, continue, else
     * Identifiers: [A-Z][a-z0-9_]{0,30}
     * Booleans: true, false
     */
    private boolean tryMatchKeywordOrIdentifier() {
        int startCol = column;
        int startPos = position;
        StringBuilder lexeme = new StringBuilder();
        
        char first = peek();
        if (!Character.isUpperCase(first)) {
            // Check for boolean literals (lowercase)
            if (tryMatchBoolean()) return true;
            return false;
        }
        
        lexeme.append(first);
        advance();
        
        // Continue matching [a-z0-9_]
        while (position < input.length()) {
            char c = peek();
            if (Character.isLowerCase(c) || Character.isDigit(c) || c == '_') {
                lexeme.append(c);
                advance();
            } else {
                break;
            }
        }
        
        String text = lexeme.toString();
        
        // Check length constraint (max 31 characters)
        if (text.length() > 31) {
            errorHandler.reportInvalidIdentifier(line, startCol, text, 
                "Identifier exceeds maximum length of 31 characters");
            return true;
        }
        
        // Check if it's a keyword
        if (KEYWORDS.contains(text)) {
            TokenType keywordType = getKeywordType(text);
            addToken(keywordType, text, line, startCol);
            return true;
        }
        
        // It's an identifier
        addToken(TokenType.IDENTIFIER, text, line, startCol);
        symbolTable.addIdentifier(text, line);
        return true;
    }
    
    /**
     * Try to match boolean literal: true or false
     */
    private boolean tryMatchBoolean() {
        if (matchKeyword("true")) {
            addToken(TokenType.BOOLEAN_LITERAL, "true", line, column - 4);
            return true;
        }
        if (matchKeyword("false")) {
            addToken(TokenType.BOOLEAN_LITERAL, "false", line, column - 5);
            return true;
        }
        return false;
    }
    
    /**
     * Match a specific keyword at current position
     */
    private boolean matchKeyword(String keyword) {
        int startPos = position;
        int startCol = column;
        
        for (int i = 0; i < keyword.length(); i++) {
            if (position >= input.length() || peek() != keyword.charAt(i)) {
                // Rewind
                position = startPos;
                column = startCol;
                return false;
            }
            advance();
        }
        
        // Make sure next character is not a letter/digit (word boundary)
        if (position < input.length()) {
            char next = peek();
            if (Character.isLetterOrDigit(next) || next == '_') {
                // Rewind
                position = startPos;
                column = startCol;
                return false;
            }
        }
        
        return true;
    }
    
    /**
     * Get TokenType for a keyword
     */
    private TokenType getKeywordType(String keyword) {
        switch (keyword) {
            case "start": return TokenType.KEYWORD_START;
            case "finish": return TokenType.KEYWORD_FINISH;
            case "loop": return TokenType.KEYWORD_LOOP;
            case "condition": return TokenType.KEYWORD_CONDITION;
            case "declare": return TokenType.KEYWORD_DECLARE;
            case "output": return TokenType.KEYWORD_OUTPUT;
            case "input": return TokenType.KEYWORD_INPUT;
            case "function": return TokenType.KEYWORD_FUNCTION;
            case "return": return TokenType.KEYWORD_RETURN;
            case "break": return TokenType.KEYWORD_BREAK;
            case "continue": return TokenType.KEYWORD_CONTINUE;
            case "else": return TokenType.KEYWORD_ELSE;
            default: return TokenType.IDENTIFIER;
        }
    }
    
    /**
     * Try to match integer or floating-point literal
     * Integer: [+-]?[0-9]+
     * Float: [+-]?[0-9]+\.[0-9]{1,6}([eE][+-]?[0-9]+)?
     */
    private boolean tryMatchNumber() {
        int startCol = column;
        StringBuilder lexeme = new StringBuilder();
        
        // Optional sign
        if (peek() == '+' || peek() == '-') {
            lexeme.append(peek());
            advance();
        }
        
        // Must have at least one digit
        if (position >= input.length() || !Character.isDigit(peek())) {
            // Just a sign, not a number - backtrack
            for (int i = 0; i < lexeme.length(); i++) {
                position--;
                column--;
            }
            return false;
        }
        
        // Integer part
        while (position < input.length() && Character.isDigit(peek())) {
            lexeme.append(peek());
            advance();
        }
        
        // Check for decimal point (float)
        if (position < input.length() && peek() == '.') {
            // Look ahead to ensure there's a digit after the decimal
            if (position + 1 < input.length() && Character.isDigit(peekAhead(1))) {
                lexeme.append(peek());
                advance();
                
                // Fractional part
                int fracDigits = 0;
                while (position < input.length() && Character.isDigit(peek())) {
                    lexeme.append(peek());
                    advance();
                    fracDigits++;
                }
                
                // Check fractional part length (1-6 digits)
                if (fracDigits > 6) {
                    errorHandler.reportMalformedFloat(line, startCol, lexeme.toString(), 
                        "Fractional part exceeds 6 digits");
                    return true;
                }
                
                // Optional exponent
                if (position < input.length() && (peek() == 'e' || peek() == 'E')) {
                    lexeme.append(peek());
                    advance();
                    
                    // Optional sign in exponent
                    if (position < input.length() && (peek() == '+' || peek() == '-')) {
                        lexeme.append(peek());
                        advance();
                    }
                    
                    // Exponent digits
                    int expStart = lexeme.length();
                    while (position < input.length() && Character.isDigit(peek())) {
                        lexeme.append(peek());
                        advance();
                    }
                    
                    // Check if exponent has digits
                    if (lexeme.length() == expStart) {
                        errorHandler.reportMalformedFloat(line, startCol, lexeme.toString(), 
                            "Exponent has no digits");
                        return true;
                    }
                }
                
                addToken(TokenType.FLOAT_LITERAL, lexeme.toString(), line, startCol);
                return true;
            }
        }
        
        // It's an integer
        addToken(TokenType.INTEGER_LITERAL, lexeme.toString(), line, startCol);
        return true;
    }
    
    /**
     * Try to match string literal: "..."
     * Supports escape sequences: \", \\, \n, \t, \r
     */
    private boolean tryMatchString() {
        if (peek() != '"') return false;
        
        int startLine = line;
        int startCol = column;
        StringBuilder lexeme = new StringBuilder();
        
        lexeme.append('"');
        advance();
        
        while (position < input.length()) {
            char c = peek();
            
            if (c == '"') {
                lexeme.append(c);
                advance();
                addToken(TokenType.STRING_LITERAL, lexeme.toString(), startLine, startCol);
                return true;
            }
            
            if (c == '\n') {
                // Unterminated string
                errorHandler.reportUnterminatedString(startLine, startCol, lexeme.toString());
                return true;
            }
            
            if (c == '\\') {
                lexeme.append(c);
                advance();
                
                if (position < input.length()) {
                    char escaped = peek();
                    // Valid escape sequences: \", \\, \n, \t, \r
                    if (escaped == '"' || escaped == '\\' || escaped == 'n' || 
                        escaped == 't' || escaped == 'r') {
                        lexeme.append(escaped);
                        advance();
                    } else {
                        errorHandler.reportInvalidEscape(line, column, lexeme.toString(), 
                            "\\" + escaped);
                        advance();
                    }
                }
            } else {
                lexeme.append(c);
                advance();
            }
        }
        
        // End of file reached without closing quote
        errorHandler.reportUnterminatedString(startLine, startCol, lexeme.toString());
        return true;
    }
    
    /**
     * Try to match character literal: '.'
     * Supports escape sequences: \', \\, \n, \t, \r
     */
    private boolean tryMatchChar() {
        if (peek() != '\'') return false;
        
        int startLine = line;
        int startCol = column;
        StringBuilder lexeme = new StringBuilder();
        
        lexeme.append('\'');
        advance();
        
        if (position >= input.length()) {
            errorHandler.reportUnterminatedChar(startLine, startCol, lexeme.toString());
            return true;
        }
        
        char c = peek();
        
        if (c == '\\') {
            lexeme.append(c);
            advance();
            
            if (position < input.length()) {
                char escaped = peek();
                // Valid escape sequences: \', \\, \n, \t, \r
                if (escaped == '\'' || escaped == '\\' || escaped == 'n' || 
                    escaped == 't' || escaped == 'r') {
                    lexeme.append(escaped);
                    advance();
                } else {
                    errorHandler.reportInvalidEscape(line, column, lexeme.toString(), 
                        "\\" + escaped);
                    advance();
                }
            }
        } else if (c != '\n' && c != '\'') {
            lexeme.append(c);
            advance();
        } else if (c == '\n') {
            errorHandler.reportUnterminatedChar(startLine, startCol, lexeme.toString());
            return true;
        }
        
        // Closing quote
        if (position < input.length() && peek() == '\'') {
            lexeme.append('\'');
            advance();
            addToken(TokenType.CHAR_LITERAL, lexeme.toString(), startLine, startCol);
            return true;
        }
        
        errorHandler.reportUnterminatedChar(startLine, startCol, lexeme.toString());
        return true;
    }
    
    /**
     * Try to match single-character operators
     */
    private boolean tryMatchSingleCharOperator() {
        char c = peek();
        int startCol = column;
        TokenType type = null;
        
        switch (c) {
            case '+': type = TokenType.OP_PLUS; break;
            case '-': type = TokenType.OP_MINUS; break;
            case '*': type = TokenType.OP_MULTIPLY; break;
            case '/': type = TokenType.OP_DIVIDE; break;
            case '%': type = TokenType.OP_MODULO; break;
            case '<': type = TokenType.OP_LESS; break;
            case '>': type = TokenType.OP_GREATER; break;
            case '=': type = TokenType.OP_ASSIGN; break;
            case '!': type = TokenType.OP_NOT; break;
            default: return false;
        }
        
        advance();
        addToken(type, String.valueOf(c), line, startCol);
        return true;
    }
    
    /**
     * Try to match punctuators: ( ) { } [ ] , ; :
     */
    private boolean tryMatchPunctuator() {
        char c = peek();
        int startCol = column;
        TokenType type = null;
        
        switch (c) {
            case '(': type = TokenType.PUNCT_LPAREN; break;
            case ')': type = TokenType.PUNCT_RPAREN; break;
            case '{': type = TokenType.PUNCT_LBRACE; break;
            case '}': type = TokenType.PUNCT_RBRACE; break;
            case '[': type = TokenType.PUNCT_LBRACKET; break;
            case ']': type = TokenType.PUNCT_RBRACKET; break;
            case ',': type = TokenType.PUNCT_COMMA; break;
            case ';': type = TokenType.PUNCT_SEMICOLON; break;
            case ':': type = TokenType.PUNCT_COLON; break;
            default: return false;
        }
        
        advance();
        addToken(type, String.valueOf(c), line, startCol);
        return true;
    }
    
    /**
     * Skip whitespace and track line numbers
     */
    private void skipWhitespace() {
        while (position < input.length() && Character.isWhitespace(peek())) {
            advance();
        }
    }
    
    /**
     * Peek at current character without advancing
     */
    private char peek() {
        if (position >= input.length()) return '\0';
        return input.charAt(position);
    }
    
    /**
     * Peek ahead n characters
     */
    private char peekAhead(int n) {
        if (position + n >= input.length()) return '\0';
        return input.charAt(position + n);
    }
    
    /**
     * Advance to next character and update position tracking
     */
    private void advance() {
        if (position < input.length()) {
            char c = input.charAt(position);
            position++;
            
            if (c == '\n') {
                line++;
                column = 1;
                lineStart = position;
            } else {
                column++;
            }
        }
    }
    
    /**
     * Add a token to the list
     */
    private void addToken(TokenType type, String lexeme, int line, int column) {
        tokens.add(new Token(type, lexeme, line, column));
        totalTokens++;
        tokenCounts.put(type, tokenCounts.getOrDefault(type, 0) + 1);
    }
    
    /**
     * Print all tokens
     */
    public void printTokens() {
        System.out.println("\n╔════════════════════════════════════════════════════════════════════════╗");
        System.out.println("║                            TOKEN LIST                                  ║");
        System.out.println("╚════════════════════════════════════════════════════════════════════════╝\n");
        
        for (Token token : tokens) {
            if (token.getType() != TokenType.EOF) {
                System.out.println(token.toString());
            }
        }
        
        System.out.println();
    }
    
    /**
     * Print statistics
     */
    public void printStatistics() {
        System.out.println("\n╔════════════════════════════════════════════════════════════════════════╗");
        System.out.println("║                          SCAN STATISTICS                               ║");
        System.out.println("╠════════════════════════════════════════════════════════════════════════╣");
        System.out.printf("║ Total Tokens:            %-47d ║%n", totalTokens);
        System.out.printf("║ Lines Processed:         %-47d ║%n", linesProcessed);
        System.out.printf("║ Comments Removed:        %-47d ║%n", commentsRemoved);
        System.out.printf("║ Unique Identifiers:      %-47d ║%n", symbolTable.size());
        System.out.printf("║ Errors Detected:         %-47d ║%n", errorHandler.getErrorCount());
        System.out.println("╠════════════════════════════════════════════════════════════════════════╣");
        System.out.println("║ Token Type Distribution:                                               ║");
        System.out.println("╠════════════════════════════════════════════════════════════════════════╣");
        
        // Sort token types by count (descending)
        List<Map.Entry<TokenType, Integer>> sorted = new ArrayList<>(tokenCounts.entrySet());
        sorted.sort((a, b) -> b.getValue().compareTo(a.getValue()));
        
        for (Map.Entry<TokenType, Integer> entry : sorted) {
            System.out.printf("║ %-35s : %-32d ║%n", 
                            entry.getKey(), entry.getValue());
        }
        
        System.out.println("╚════════════════════════════════════════════════════════════════════════╝\n");
    }
    
    // Getters
    public List<Token> getTokens() { return tokens; }
    public SymbolTable getSymbolTable() { return symbolTable; }
    public ErrorHandler getErrorHandler() { return errorHandler; }
    public int getTotalTokens() { return totalTokens; }
    public int getLinesProcessed() { return linesProcessed; }
    public int getCommentsRemoved() { return commentsRemoved; }
    
    /**
     * Main method for testing
     */
    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Usage: java ManualScanner <source-file.nex>");
            System.exit(1);
        }
        
        ManualScanner scanner = new ManualScanner();
        
        try {
            System.out.println("╔════════════════════════════════════════════════════════════════════════╗");
            System.out.println("║              NEXUS LANGUAGE LEXICAL ANALYZER                           ║");
            System.out.println("║                  Manual Scanner Implementation                         ║");
            System.out.println("╚════════════════════════════════════════════════════════════════════════╝");
            System.out.println("\nScanning file: " + args[0]);
            
            scanner.scanFile(args[0]);
            
            // Print results
            scanner.printTokens();
            scanner.printStatistics();
            scanner.getSymbolTable().print();
            scanner.getErrorHandler().printErrors();
            
        } catch (IOException e) {
            System.err.println("Error reading file: " + e.getMessage());
            System.exit(1);
        }
    }
}
