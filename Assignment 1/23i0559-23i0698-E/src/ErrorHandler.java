package errorhandler;
/**
 * ErrorHandler Class
 * Handles detection, reporting, and recovery from lexical errors
 * 
 * @author Ruhab (23i-0559), Hasan (23i-0698)
 * @course CS4031 - Compiler Construction
 * @assignment Assignment 1 - Lexical Analyzer
 */

import java.util.*;

public class ErrorHandler {
    
    /**
     * Inner class to represent a lexical error
     */
    public static class LexicalError {
        String errorType;
        int lineNumber;
        int columnNumber;
        String lexeme;
        String reason;
        
        public LexicalError(String errorType, int lineNumber, int columnNumber, 
                          String lexeme, String reason) {
            this.errorType = errorType;
            this.lineNumber = lineNumber;
            this.columnNumber = columnNumber;
            this.lexeme = lexeme;
            this.reason = reason;
        }
        
        @Override
        public String toString() {
            return String.format("[%s] Line %d, Col %d: '%s' - %s",
                errorType, lineNumber, columnNumber, lexeme, reason);
        }
    }
    
    private List<LexicalError> errors;
    private boolean hasErrors;
    
    /**
     * Constructor
     */
    public ErrorHandler() {
        errors = new ArrayList<>();
        hasErrors = false;
    }
    
    /**
     * Report an invalid character error
     */
    public void reportInvalidCharacter(char ch, int line, int column) {
        String lexeme = String.valueOf(ch);
        String reason = "Invalid character '" + ch + "' (ASCII: " + (int)ch + ")";
        errors.add(new LexicalError("INVALID_CHARACTER", line, column, lexeme, reason));
        hasErrors = true;
    }
    
    /**
     * Report a malformed integer literal error
     */
    public void reportMalformedInteger(String lexeme, int line, int column, String reason) {
        errors.add(new LexicalError("MALFORMED_INTEGER", line, column, lexeme, reason));
        hasErrors = true;
    }
    
    /**
     * Report a malformed float literal error
     */
    public void reportMalformedFloat(String lexeme, int line, int column, String reason) {
        errors.add(new LexicalError("MALFORMED_FLOAT", line, column, lexeme, reason));
        hasErrors = true;
    }
    
    /**
     * Report an unterminated string literal error
     */
    public void reportUnterminatedString(String lexeme, int line, int column) {
        String reason = "String literal not terminated before end of line";
        errors.add(new LexicalError("UNTERMINATED_STRING", line, column, lexeme, reason));
        hasErrors = true;
    }
    
    /**
     * Report an unterminated character literal error
     */
    public void reportUnterminatedChar(String lexeme, int line, int column) {
        String reason = "Character literal not properly terminated";
        errors.add(new LexicalError("UNTERMINATED_CHAR", line, column, lexeme, reason));
        hasErrors = true;
    }
    
    /**
     * Report an invalid character literal error
     */
    public void reportInvalidCharLiteral(String lexeme, int line, int column, String reason) {
        errors.add(new LexicalError("INVALID_CHAR_LITERAL", line, column, lexeme, reason));
        hasErrors = true;
    }
    
    /**
     * Report an invalid identifier error
     */
    public void reportInvalidIdentifier(String lexeme, int line, int column, String reason) {
        errors.add(new LexicalError("INVALID_IDENTIFIER", line, column, lexeme, reason));
        hasErrors = true;
    }
    
    /**
     * Report an unclosed multi-line comment error
     */
    public void reportUnclosedComment(int line, int column) {
        String reason = "Multi-line comment not closed before end of file";
        errors.add(new LexicalError("UNCLOSED_COMMENT", line, column, "#*", reason));
        hasErrors = true;
    }
    
    /**
     * Report an invalid escape sequence error
     */
    public void reportInvalidEscape(String lexeme, int line, int column, char escapeChar) {
        String reason = "Invalid escape sequence '\\" + escapeChar + "'";
        errors.add(new LexicalError("INVALID_ESCAPE", line, column, lexeme, reason));
        hasErrors = true;
    }
    
    /**
     * Report a generic lexical error
     */
    public void reportError(String errorType, String lexeme, int line, int column, String reason) {
        errors.add(new LexicalError(errorType, line, column, lexeme, reason));
        hasErrors = true;
    }
    
    /**
     * Check if there are any errors
     */
    public boolean hasErrors() {
        return hasErrors;
    }
    
    /**
     * Get total number of errors
     */
    public int getErrorCount() {
        return errors.size();
    }
    
    /**
     * Get all errors
     */
    public List<LexicalError> getErrors() {
        return new ArrayList<>(errors);
    }
    
    /**
     * Display all errors in a formatted way
     */
    public void displayErrors() {
        if (!hasErrors) {
            System.out.println("\n✓ No lexical errors found!");
            return;
        }
        
        System.out.println("\n╔══════════════════════════════════════════════════════════════════════════════╗");
        System.out.println("║                            LEXICAL ERRORS                                    ║");
        System.out.println("╠══════════════════════════════════════════════════════════════════════════════╣");
        
        for (int i = 0; i < errors.size(); i++) {
            System.out.printf("║ %-77s║\n", (i + 1) + ". " + errors.get(i).toString());
        }
        
        System.out.println("╠══════════════════════════════════════════════════════════════════════════════╣");
        System.out.printf("║ Total Errors: %-66d ║\n", errors.size());
        System.out.println("╚══════════════════════════════════════════════════════════════════════════════╝");
    }
    
    /**
     * Clear all errors
     */
    public void clear() {
        errors.clear();
        hasErrors = false;
    }
}