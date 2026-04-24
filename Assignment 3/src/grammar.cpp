// Author: Ruhab Ahmad (23I-0559)
// Assignment 03: Bottom-Up Parser Design & Implementation
#include "grammar.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>

// Helper function to trim whitespace from strings
static std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (std::string::npos == first) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

void Grammar::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening grammar file: " << filename << "\n";
        return;
    }

    std::string line;
    bool firstLine = true;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        // Find the '->' separator [cite: 37]
        size_t arrowPos = line.find("->");
        if (arrowPos == std::string::npos) continue;

        std::string lhs = trim(line.substr(0, arrowPos));
        std::string rhs_str = trim(line.substr(arrowPos + 2));

        non_terminals.insert(lhs);
        
        if (firstLine) {
            // Set the original start symbol
            start_symbol = lhs;
            firstLine = false;
        }

        // Split alternatives by '|' [cite: 38]
        std::stringstream ss(rhs_str);
        std::string alt;
        while (std::getline(ss, alt, '|')) {
            std::stringstream alt_ss(trim(alt));
            std::string sym;
            Production p;
            p.lhs = lhs;
            
            while (alt_ss >> sym) {
                p.rhs.push_back(sym);
            }
            
            if (!p.rhs.empty()) {
                productions.push_back(p);
            }
        }
    }

    // Populate terminals: any symbol in RHS that is not a non-terminal [cite: 39]
    for (const auto& prod : productions) {
        for (const auto& sym : prod.rhs) {
            // Check if symbol is not a non-terminal and not epsilon [cite: 41]
            if (non_terminals.find(sym) == non_terminals.end() && sym != "epsilon" && sym != "@") {
                terminals.insert(sym);
            }
        }
    }
}

void Grammar::augmentGrammar() {
    std::string newStart = start_symbol + "Prime";
    Production augmentedProd;
    augmentedProd.lhs = newStart;
    augmentedProd.rhs.push_back(start_symbol);
    
    // Insert S' -> S at the beginning
    productions.insert(productions.begin(), augmentedProd);
    non_terminals.insert(newStart);
    
    // Update the start symbol to S'
    start_symbol = newStart;
    
    // CRITICAL FIX: Explicitly add $ so LR(1) FIRST sets compute properly
    terminals.insert("$"); 
}

void Grammar::printAugmentedGrammar() const {
    std::cout << "Augmented Grammar:\n";
    for (size_t i = 0; i < productions.size(); ++i) {
        std::cout << i << ": " << productions[i].lhs << " -> ";
        for (const auto& symbol : productions[i].rhs) {
            std::cout << symbol << " ";
        }
        std::cout << "\n";
    }
}

// Note: Ensure your grammar.h has the `const` keyword at the end of this signature
std::set<std::string> Grammar::computeFirst(const std::vector<std::string>& symbols) const {
    std::set<std::string> firstSet;
    if (symbols.empty()) return firstSet;

    // Helper map to iteratively calculate FIRST sets for all symbols in the grammar
    std::map<std::string, std::set<std::string>> first_map;
    
    // Base case: FIRST(terminal) = {terminal}
    for (const auto& t : terminals) first_map[t].insert(t);
    first_map["epsilon"].insert("epsilon");
    first_map["@"].insert("@");

    bool changed = true;
    while (changed) {
        changed = false;
        for (const auto& prod : productions) {
            size_t original_size = first_map[prod.lhs].size();
            bool all_derive_eps = true;
            
            for (const auto& sym : prod.rhs) {
                // Add everything from FIRST(sym) to FIRST(LHS) except epsilon
                for (const auto& f : first_map[sym]) {
                    if (f != "epsilon" && f != "@") {
                        first_map[prod.lhs].insert(f);
                    }
                }
                // If sym doesn't derive epsilon, we stop checking the sequence
                if (first_map[sym].find("epsilon") == first_map[sym].end() && 
                    first_map[sym].find("@") == first_map[sym].end()) {
                    all_derive_eps = false;
                    break;
                }
            }
            // If all symbols in RHS derive epsilon, then LHS derives epsilon
            if (all_derive_eps) {
                first_map[prod.lhs].insert("@");
            }
            
            // If the size changed, we need another pass
            if (first_map[prod.lhs].size() > original_size) {
                changed = true;
            }
        }
    }

    // Now evaluate the specific sequence of symbols passed into the function
    bool all_derive_eps = true;
    for (const auto& sym : symbols) {
        for (const auto& f : first_map[sym]) {
            if (f != "epsilon" && f != "@") {
                firstSet.insert(f);
            }
        }
        if (first_map[sym].find("epsilon") == first_map[sym].end() && 
            first_map[sym].find("@") == first_map[sym].end()) {
            all_derive_eps = false;
            break;
        }
    }
    
    // If the entire sequence can derive epsilon
    if (all_derive_eps) {
        firstSet.insert("@");
    }

    return firstSet;
}

// Note: Ensure your grammar.h has the `const` keyword at the end of this signature
std::set<std::string> Grammar::computeFollow(const std::string& non_terminal) const {
    std::map<std::string, std::set<std::string>> follow_map;
    
    // Rule 1: FOLLOW of start symbol contains $
    follow_map[start_symbol].insert("$");

    bool changed = true;
    while (changed) {
        changed = false;
        for (const auto& prod : productions) {
            for (size_t i = 0; i < prod.rhs.size(); ++i) {
                std::string B = prod.rhs[i];
                
                // We only calculate FOLLOW sets for non-terminals
                if (non_terminals.find(B) != non_terminals.end()) {
                    size_t original_size = follow_map[B].size();
                    
                    // Compute FIRST of beta (everything after B in the production)
                    std::vector<std::string> beta;
                    for (size_t j = i + 1; j < prod.rhs.size(); ++j) {
                        beta.push_back(prod.rhs[j]);
                    }
                    
                    std::set<std::string> first_beta = computeFirst(beta);
                    
                    bool beta_derives_eps = true;
                    if (!beta.empty()) {
                        beta_derives_eps = false;
                        for (const auto& f : first_beta) {
                            if (f != "epsilon" && f != "@") {
                                follow_map[B].insert(f);
                            } else {
                                beta_derives_eps = true;
                            }
                        }
                    }
                    
                    // Rule 2 & 3: If B is at the end (beta is empty), OR beta derives epsilon
                    // Then everything in FOLLOW(A) is added to FOLLOW(B)
                    if (beta_derives_eps) {
                        for (const auto& f : follow_map[prod.lhs]) {
                            follow_map[B].insert(f);
                        }
                    }
                    
                    if (follow_map[B].size() > original_size) {
                        changed = true;
                    }
                }
            }
        }
    }

    return follow_map[non_terminal];
}