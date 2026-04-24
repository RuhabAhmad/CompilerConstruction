#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>

// Represents a single production rule, e.g., A -> alpha
struct Production {
    std::string lhs;
    std::vector<std::string> rhs;
};

class Grammar {
public:
    std::vector<Production> productions;
    std::set<std::string> non_terminals;
    std::set<std::string> terminals;
    std::string start_symbol;

    void loadFromFile(const std::string& filename);
    void augmentGrammar(); // Adds S' -> S [cite: 47]
    std::set<std::string> computeFirst(const std::vector<std::string>& symbols) const;
    std::set<std::string> computeFollow(const std::string& non_terminal) const;
    
    void printAugmentedGrammar() const; // Output display requirement [cite: 49]
};

#endif