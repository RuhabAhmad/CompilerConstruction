// Author: Ruhab Ahmad (23I-0559)
// Assignment 03: Bottom-Up Parser Design & Implementation
#include "slr_parser.h"
#include "stack.h"
#include <iostream>
#include <iomanip>

SLRParser::SLRParser(const Grammar& grammar) : g(grammar) {}

void SLRParser::buildTable() {
    items.buildCanonicalLR0(g);
    
    // 1. Shift Actions and 4. GOTO Actions
    for (const auto& transition : items.transitions) {
        int from_state = transition.first.first;
        std::string symbol = transition.first.second;
        int to_state = transition.second;

        if (g.terminals.count(symbol)) {
            action_table[{from_state, symbol}] = {ActionType::SHIFT, to_state}; // [cite: 78, 79, 80]
        } else if (g.non_terminals.count(symbol)) {
            goto_table[{from_state, symbol}] = to_state; // [cite: 91, 92, 93]
        }
    }

    // 2. Reduce Actions and 3. Accept Action
    for (size_t i = 0; i < items.states.size(); ++i) {
        for (const auto& item : items.states[i]) {
            const Production& prod = g.productions[item.prod_idx];
            
            // Check if dot is at the end (A -> alpha .)
            if (item.dot_pos == (int)prod.rhs.size() || (prod.rhs.size() == 1 && (prod.rhs[0] == "epsilon" || prod.rhs[0] == "@"))) {
                
                if (prod.lhs == g.start_symbol) {
                    action_table[{i, "$"}] = {ActionType::ACCEPT, 0}; // [cite: 87, 88, 89]
                } else {
                    std::set<std::string> followA = g.computeFollow(prod.lhs); // [cite: 84]
                    for (const auto& a : followA) {
                        
                        // Conflict Detection
                        if (action_table.find({(int)i, a}) != action_table.end()) {
                            Action existing = action_table[{(int)i, a}];
                            if (existing.type == ActionType::SHIFT) {
                                std::cout << "Shift/Reduce Conflict in State " << i << " on terminal '" << a << "'\n"; // [cite: 95]
                            } else if (existing.type == ActionType::REDUCE) {
                                std::cout << "Reduce/Reduce Conflict in State " << i << " on terminal '" << a << "'\n"; // [cite: 96]
                            }
                        }
                        
                        action_table[{(int)i, a}] = {ActionType::REDUCE, item.prod_idx}; // [cite: 85]
                    }
                }
            }
        }
    }
}

void SLRParser::reportConflicts() {
    std::cout << "Conflict analysis complete.\n"; // [cite: 101]
}

bool SLRParser::parse(const std::vector<std::string>& inputTokens) {
    ParserStack stack;
    stack.pushState(0); // [cite: 105]
    
    size_t inputPtr = 0;
    int step = 1;
    
    std::cout << std::left << std::setw(6) << "Step" 
              << std::setw(30) << "Stack" 
              << std::setw(30) << "Input" 
              << "Action\n"; // [cite: 123, 124, 125, 126, 127]
    std::cout << std::string(80, '-') << "\n";
    
    while (true) { // [cite: 107]
        int s = stack.peekState(); // [cite: 106]
        std::string a = inputTokens[inputPtr]; // [cite: 106]
        
        // Print Step
        std::cout << std::left << std::setw(6) << step++;
        
        // Print Stack
        stack.print();
        
        // Print Input
        std::string remainingInput = "";
        for (size_t i = inputPtr; i < inputTokens.size(); ++i) {
            remainingInput += inputTokens[i] + " ";
        }
        std::cout << std::left << std::setw(30) << remainingInput;
        
        auto actionIt = action_table.find({s, a});
        if (actionIt == action_table.end()) {
            std::cout << "Error (No valid action)\n"; // [cite: 121, 122]
            return false;
        }
        
        Action action = actionIt->second;
        
        if (action.type == ActionType::SHIFT) {
            std::cout << "Shift " << action.target << "\n"; // [cite: 108, 109]
            stack.pushSymbol(a); // [cite: 110]
            stack.pushState(action.target); // [cite: 110]
            inputPtr++; // [cite: 111]
        } 
        else if (action.type == ActionType::REDUCE) {
            Production p = g.productions[action.target];
            
            std::cout << "Reduce " << p.lhs << " -> "; // [cite: 112, 113]
            bool isEpsilon = (p.rhs.size() == 1 && (p.rhs[0] == "epsilon" || p.rhs[0] == "@"));
            
            if (isEpsilon) {
                 std::cout << "epsilon\n";
            } else {
                 for (const auto& sym : p.rhs) std::cout << sym << " ";
                 std::cout << "\n";
            }
            
            // Pop symbols from stack
            int popCount = isEpsilon ? 0 : p.rhs.size();
            for (int i = 0; i < popCount; ++i) {
                stack.popState();
                stack.popSymbol();
            } // [cite: 114]
            
            int t = stack.peekState(); // [cite: 115]
            stack.pushSymbol(p.lhs); // [cite: 116]
            
            auto gotoIt = goto_table.find({t, p.lhs});
            if (gotoIt != goto_table.end()) {
                stack.pushState(gotoIt->second); // [cite: 116]
            } else {
                std::cout << "GOTO Error\n";
                return false;
            }
        } 
        else if (action.type == ActionType::ACCEPT) {
            std::cout << "Accept\n"; // [cite: 117, 118, 119]
            return true;
        }
    }
}