package errorhandler;
import java.util.*;

public class ErrorHandler {
    private List<LexicalError> errors;
    
    public static class LexicalError {
        String errorType;
        int line;
        int column;
        String lexeme;
        String reason;
        
        public LexicalError(String errorType, int line, int column, 
                          String lexeme, String reason) {
            this.errorType = errorType;
            this.line = line;
            this.column = column;
            this.lexeme = lexeme;
            this.reason = reason;
        }
        
        @Override
        public String toString() {
            return String.format("Error [%s] at Line %d, Col %d: '%s' - %s", 
                               errorType, line, column, lexeme, reason);
        }
    }
    
    public ErrorHandler() {
        errors = new ArrayList<>();
    }
    
    public void reportError(String errorType, int line, int column, 
                          String lexeme, String reason) {
        errors.add(new LexicalError(errorType, line, column, lexeme, reason));
    }
    
    public void invalidCharacter(int line, int column, char ch) {
        reportError("INVALID_CHARACTER", line, column, 
                   String.valueOf(ch), 
                   "Character '" + ch + "' is not allowed in this language");
    }
    
    public void malformedFloat(int line, int column, String lexeme) {
        reportError("MALFORMED_FLOAT", line, column, lexeme, 
                   "Invalid floating-point literal format");
    }
    
    public void unterminatedString(int line, int column, String lexeme) {
        reportError("UNTERMINATED_STRING", line, column, lexeme, 
                   "String literal not properly closed");
    }
    
    public void unterminatedChar(int line, int column, String lexeme) {
        reportError("UNTERMINATED_CHAR", line, column, lexeme, 
                   "Character literal not properly closed");
    }
    
    public void invalidIdentifier(int line, int column, String lexeme, String reason) {
        reportError("INVALID_IDENTIFIER", line, column, lexeme, reason);
    }
    
    public void unclosedComment(int line, int column) {
        reportError("UNCLOSED_COMMENT", line, column, "#*", 
                   "Multi-line comment not properly closed");
    }
    
    public void invalidEscape(int line, int column, String lexeme) {
        reportError("INVALID_ESCAPE", line, column, lexeme, 
                   "Invalid escape sequence in string/character literal");
    }
    
    public boolean hasErrors() {
        return !errors.isEmpty();
    }
    
    public int getErrorCount() {
        return errors.size();
    }
    
    public void displayErrors() {
        if (errors.isEmpty()) {
            System.out.println("\n=== No Lexical Errors Found ===");
            return;
        }
        
        System.out.println("\n=== Lexical Errors (" + errors.size() + ") ===");
        for (LexicalError error : errors) {
            System.out.println(error);
        }
    }
    
    public void clear() {
        errors.clear();
    }
}