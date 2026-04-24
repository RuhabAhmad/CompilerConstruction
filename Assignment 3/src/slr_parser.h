#ifndef SLR_PARSER_H
#define SLR_PARSER_H

#include "items.h"

enum class ActionType { SHIFT, REDUCE, ACCEPT, ERROR };

struct Action {
    ActionType type;
    int target; // state for SHIFT, production index for REDUCE
};

class SLRParser {
private:
    Grammar g;
    ItemCollection items;
    std::map<std::pair<int, std::string>, Action> action_table; // [cite: 100]
    std::map<std::pair<int, std::string>, int> goto_table;      // [cite: 100]

public:
    SLRParser(const Grammar& grammar);
    void buildTable(); // Task 1.4: Implement table construction [cite: 74, 75]
    bool parse(const std::vector<std::string>& inputTokens); // Task 1.5: Stack parsing [cite: 103, 104, 105]
    void reportConflicts(); // Checks for Shift/Reduce or Reduce/Reduce [cite: 94, 95, 96]
};

#endif