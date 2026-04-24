#include "items.h"
#include <queue>
#include <algorithm>

// Task 1.3: LR(0) CLOSURE Operation [cite: 54, 55, 56, 57, 58, 59]
ItemSet ItemCollection::closureLR0(const ItemSet& I, const Grammar& g) {
    ItemSet closureSet = I;
    std::queue<Item> worklist;
    for (const auto& item : I) worklist.push(item);

    while (!worklist.empty()) {
        Item current = worklist.front();
        worklist.pop();

        const Production& prod = g.productions[current.prod_idx];
        
        // Check if dot is before a non-terminal
        if (current.dot_pos < (int)prod.rhs.size()) {
            std::string symbolAfterDot = prod.rhs[current.dot_pos];
            
            if (g.non_terminals.count(symbolAfterDot)) {
                // Add B -> . gamma for all productions B -> gamma
                for (size_t i = 0; i < g.productions.size(); ++i) {
                    if (g.productions[i].lhs == symbolAfterDot) {
                        Item newItem = {(int)i, 0, ""};
                        if (closureSet.insert(newItem).second) {
                            worklist.push(newItem);
                        }
                    }
                }
            }
        }
    }
    return closureSet;
}

// Task 1.3: LR(0) GOTO Operation [cite: 60, 61, 62, 63, 64]
ItemSet ItemCollection::gotoLR0(const ItemSet& I, const std::string& X, const Grammar& g) {
    ItemSet movedItems;
    for (const auto& item : I) {
        const Production& prod = g.productions[item.prod_idx];
        if (item.dot_pos < (int)prod.rhs.size() && prod.rhs[item.dot_pos] == X) {
            Item movedItem = item;
            movedItem.dot_pos++;
            movedItems.insert(movedItem);
        }
    }
    return closureLR0(movedItems, g);
}

// Task 2.1: LR(1) CLOSURE Operation [cite: 134, 135, 136, 137, 138, 139, 140, 141, 142, 143]
ItemSet ItemCollection::closureLR1(const ItemSet& I, const Grammar& g) {
    ItemSet closureSet = I;
    std::queue<Item> worklist;
    for (const auto& item : I) worklist.push(item);

    while (!worklist.empty()) {
        Item current = worklist.front();
        worklist.pop();

        const Production& prod = g.productions[current.prod_idx];
        
        if (current.dot_pos < (int)prod.rhs.size()) {
            std::string B = prod.rhs[current.dot_pos];
            
            if (g.non_terminals.count(B)) {
                // Compute FIRST(beta a)
                std::vector<std::string> beta_a;
                for (size_t i = current.dot_pos + 1; i < prod.rhs.size(); ++i) {
                    beta_a.push_back(prod.rhs[i]);
                }
                beta_a.push_back(current.lookahead);
                
                std::set<std::string> firstSets = g.computeFirst(beta_a);
                
                for (size_t i = 0; i < g.productions.size(); ++i) {
                    if (g.productions[i].lhs == B) {
                        for (const auto& b : firstSets) {
                            Item newItem = {(int)i, 0, b};
                            if (closureSet.insert(newItem).second) {
                                worklist.push(newItem);
                            }
                        }
                    }
                }
            }
        }
    }
    return closureSet;
}

// Task 2.1: LR(1) GOTO Operation [cite: 145, 146, 147, 148, 149]
ItemSet ItemCollection::gotoLR1(const ItemSet& I, const std::string& X, const Grammar& g) {
    ItemSet movedItems;
    for (const auto& item : I) {
        const Production& prod = g.productions[item.prod_idx];
        // Move the dot past X [cite: 148]
        if (item.dot_pos < (int)prod.rhs.size() && prod.rhs[item.dot_pos] == X) {
            Item movedItem = item;
            movedItem.dot_pos++;
            movedItems.insert(movedItem);
        }
    }
    // Return CLOSURE of these items [cite: 149]
    return closureLR1(movedItems, g);
}

// Task 1.3: Canonical Collection Algorithm for LR(0)
void ItemCollection::buildCanonicalLR0(const Grammar& g) {
    states.clear();
    transitions.clear();

    // 1. Initialize: C = {CLOSURE({S' -> . S})}
    // Assuming S' -> S is always the first production (index 0) after augmentation
    Item startItem = {0, 0, ""}; 
    ItemSet initialSet;
    initialSet.insert(startItem);
    
    ItemSet state0 = closureLR0(initialSet, g);
    states.push_back(state0);

    // Combine all grammar symbols (terminals and non-terminals) for the GOTO loops
    std::vector<std::string> all_symbols;
    all_symbols.insert(all_symbols.end(), g.terminals.begin(), g.terminals.end());
    all_symbols.insert(all_symbols.end(), g.non_terminals.begin(), g.non_terminals.end());

    // 2. Compute GOTO for each state and symbol
    size_t head = 0; // Using a while loop since states.size() will grow dynamically
    while (head < states.size()) {
        ItemSet current_state = states[head];
        
        for (const auto& X : all_symbols) {
            ItemSet next_state = gotoLR0(current_state, X, g);
            
            if (!next_state.empty()) {
                // Check if next_state is already in our collection (C)
                auto it = std::find(states.begin(), states.end(), next_state);
                int target_state_idx;
                
                if (it == states.end()) {
                    // Not found, add it to C
                    states.push_back(next_state);
                    target_state_idx = states.size() - 1;
                } else {
                    // Found, get its index
                    target_state_idx = std::distance(states.begin(), it);
                }
                
                // Record the GOTO transition
                transitions[{head, X}] = target_state_idx;
            }
        }
        head++;
    }
}

// Task 2.1: Canonical Collection Algorithm for LR(1)
void ItemCollection::buildCanonicalLR1(const Grammar& g) {
    states.clear();
    transitions.clear();

    // 1. Initialize: C = {CLOSURE({[S' -> . S, $]})}
    Item startItem = {0, 0, "$"}; // Lookahead is $
    ItemSet initialSet;
    initialSet.insert(startItem);
    
    ItemSet state0 = closureLR1(initialSet, g);
    states.push_back(state0);

    std::vector<std::string> all_symbols;
    all_symbols.insert(all_symbols.end(), g.terminals.begin(), g.terminals.end());
    all_symbols.insert(all_symbols.end(), g.non_terminals.begin(), g.non_terminals.end());

    size_t head = 0;
    while (head < states.size()) {
        ItemSet current_state = states[head];
        
        for (const auto& X : all_symbols) {
            ItemSet next_state = gotoLR1(current_state, X, g);
            
            if (!next_state.empty()) {
                auto it = std::find(states.begin(), states.end(), next_state);
                int target_state_idx;
                
                if (it == states.end()) {
                    states.push_back(next_state);
                    target_state_idx = states.size() - 1;
                } else {
                    target_state_idx = std::distance(states.begin(), it);
                }
                
                transitions[{head, X}] = target_state_idx;
            }
        }
        head++;
    }
}