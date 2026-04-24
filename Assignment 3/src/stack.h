// Author: Ruhab Ahmad (23I-0559)
// Assignment 03: Bottom-Up Parser Design & Implementation
#ifndef STACK_H
#define STACK_H

#include <vector>
#include <string>

class ParserStack {
private:
    std::vector<int> states;
    std::vector<std::string> symbols;

public:
    void pushState(int state);
    void pushSymbol(const std::string& symbol);
    
    int popState();
    std::string popSymbol();
    
    int peekState() const;
    std::string peekSymbol() const;
    
    void print() const;
};

#endif