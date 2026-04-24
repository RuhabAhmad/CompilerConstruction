// Author: Ruhab Ahmad (23I-0559)
// Assignment 03: Bottom-Up Parser Design & Implementation
#include "stack.h"
#include <iostream>

void ParserStack::pushState(int state) { states.push_back(state); }
void ParserStack::pushSymbol(const std::string& symbol) { symbols.push_back(symbol); }

int ParserStack::popState() {
    int s = states.back();
    states.pop_back();
    return s;
}

std::string ParserStack::popSymbol() {
    std::string sym = symbols.back();
    symbols.pop_back();
    return sym;
}

int ParserStack::peekState() const { return states.back(); }
std::string ParserStack::peekSymbol() const { return symbols.back(); }

void ParserStack::print() const {
    for (size_t i = 0; i < states.size(); ++i) {
        if (i < symbols.size()) {
            std::cout << symbols[i] << " ";
        }
        std::cout << states[i] << " ";
    }
}