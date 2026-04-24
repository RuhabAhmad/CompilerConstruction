// Author: Ruhab Ahmad (23I-0559)
// Assignment 03: Bottom-Up Parser Design & Implementation
#include "lr1_parser.h"
#include "stack.h"
#include <iostream>
#include <iomanip>

LR1Parser::LR1Parser(const Grammar& grammar) : g(grammar) {}

void LR1Parser::buildTable() {
    items.buildCanonicalLR1(g);
    
    for (const auto& transition : items.transitions) {
        int from_state = transition.first.first;
        std::string symbol = transition.first.second;
        int to_state = transition.second;

        if (g.terminals.count(symbol)) {
            action_table[{from_state, symbol}] = {ActionType::SHIFT, to_state};
        } else if (g.non_terminals.count(symbol)) {
            goto_table[{from_state, symbol}] = to_state;
        }
    }

    for (size_t i = 0; i < items.states.size(); ++i) {
        for (const auto& item : items.states[i]) {
            const Production& prod = g.productions[item.prod_idx];
            
            if (item.dot_pos == (int)prod.rhs.size() || (prod.rhs.size() == 1 && (prod.rhs[0] == "epsilon" || prod.rhs[0] == "@"))) {
                
                if (prod.lhs == g.start_symbol && item.lookahead == "$") {
                    action_table[{i, "$"}] = {ActionType::ACCEPT, 0};
                } else if (prod.lhs != g.start_symbol) { 
                    std::string a = item.lookahead;
                    
                    if (action_table.find({(int)i, a}) != action_table.end()) {
                        Action existing = action_table[{(int)i, a}];
                        if (existing.type == ActionType::SHIFT) {
                            std::cout << "LR(1) Shift/Reduce Conflict in State " << i << " on terminal '" << a << "'\n";
                        } else if (existing.type == ActionType::REDUCE && existing.target != item.prod_idx) {
                            std::cout << "LR(1) Reduce/Reduce Conflict in State " << i << " on terminal '" << a << "'\n";
                        }
                    }
                    
                    action_table[{(int)i, a}] = {ActionType::REDUCE, item.prod_idx};
                }
            }
        }
    }
}

void LR1Parser::reportConflicts() {
    std::cout << "LR(1) Conflict analysis complete.\n";
}

bool LR1Parser::parse(const std::vector<std::string>& inputTokens) {
    ParserStack stack;
    stack.pushState(0);
    
    size_t inputPtr = 0;
    int step = 1;
    
    std::cout << std::left << std::setw(6) << "Step" 
              << std::setw(30) << "Stack" 
              << std::setw(30) << "Input" 
              << "Action\n";
    std::cout << std::string(80, '-') << "\n";
    
    while (true) {
        int s = stack.peekState();
        std::string a = inputTokens[inputPtr];
        
        std::cout << std::left << std::setw(6) << step++;
        
        stack.print();
        
        std::string remainingInput = "";
        for (size_t i = inputPtr; i < inputTokens.size(); ++i) {
            remainingInput += inputTokens[i] + " ";
        }
        std::cout << std::left << std::setw(30) << remainingInput;
        
        auto actionIt = action_table.find({s, a});
        if (actionIt == action_table.end()) {
            std::cout << "Error (No valid action)\n";
            return false;
        }
        
        Action action = actionIt->second;
        
        if (action.type == ActionType::SHIFT) {
            std::cout << "Shift " << action.target << "\n";
            stack.pushSymbol(a);
            stack.pushState(action.target);
            inputPtr++;
        } 
        else if (action.type == ActionType::REDUCE) {
            Production p = g.productions[action.target];
            
            std::cout << "Reduce " << p.lhs << " -> ";
            bool isEpsilon = (p.rhs.size() == 1 && (p.rhs[0] == "epsilon" || p.rhs[0] == "@"));
            
            if (isEpsilon) {
                 std::cout << "epsilon\n";
            } else {
                 for (const auto& sym : p.rhs) std::cout << sym << " ";
                 std::cout << "\n";
            }
            
            int popCount = isEpsilon ? 0 : p.rhs.size();
            for (int i = 0; i < popCount; ++i) {
                stack.popState();
                stack.popSymbol();
            }
            
            int t = stack.peekState();
            stack.pushSymbol(p.lhs);
            
            auto gotoIt = goto_table.find({t, p.lhs});
            if (gotoIt != goto_table.end()) {
                stack.pushState(gotoIt->second);
            } else {
                std::cout << "GOTO Error\n";
                return false;
            }
        } 
        else if (action.type == ActionType::ACCEPT) {
            std::cout << "Accept\n";
            return true;
        }
    }
}