package symboltable;
import java.util.*;

public class SymbolTable {
    private static class SymbolInfo {
        String name;
        String type;
        int firstOccurrence;
        int frequency;
        
        SymbolInfo(String name, String type, int firstOccurrence) {
            this.name = name;
            this.type = type;
            this.firstOccurrence = firstOccurrence;
            this.frequency = 1;
        }
        
        void incrementFrequency() {
            frequency++;
        }
    }
    
    private Map<String, SymbolInfo> table;
    
    public SymbolTable() {
        table = new LinkedHashMap<>();
    }
    
    public void addIdentifier(String name, int line) {
        if (table.containsKey(name)) {
            table.get(name).incrementFrequency();
        } else {
            table.put(name, new SymbolInfo(name, "IDENTIFIER", line));
        }
    }
    
    public boolean contains(String name) {
        return table.containsKey(name);
    }
    
    public void display() {
        System.out.println("\n=== Symbol Table ===");
        System.out.println(String.format("%-30s %-15s %-20s %-10s", 
                                       "Identifier", "Type", "First Occurrence", "Frequency"));
        System.out.println("-".repeat(80));
        
        for (SymbolInfo info : table.values()) {
            System.out.println(String.format("%-30s %-15s %-20d %-10d", 
                                           info.name, info.type, 
                                           info.firstOccurrence, info.frequency));
        }
        System.out.println("-".repeat(80));
        System.out.println("Total Identifiers: " + table.size());
    }
    
    public int size() {
        return table.size();
    }
    
    public void clear() {
        table.clear();
    }
}