#ifndef ITEMS_H
#define ITEMS_H

#include "grammar.h"

// Represents an LR(0) or LR(1) item [cite: 52, 131]
struct Item {
    int prod_idx;       // Index in Grammar::productions
    int dot_pos;        // Position of the dot
    std::string lookahead; // Used only for LR(1), empty for LR(0)

    bool operator<(const Item& other) const {
        if (prod_idx != other.prod_idx) return prod_idx < other.prod_idx;
        if (dot_pos != other.dot_pos) return dot_pos < other.dot_pos;
        return lookahead < other.lookahead;
    }
    
    bool operator==(const Item& other) const {
        return prod_idx == other.prod_idx && dot_pos == other.dot_pos && lookahead == other.lookahead;
    }
};

typedef std::set<Item> ItemSet;

class ItemCollection {
public:
    std::vector<ItemSet> states;
    std::map<std::pair<int, std::string>, int> transitions; // GOTO table [cite: 93]

    // SLR(1) Item Operations [cite: 54, 60, 65]
    ItemSet closureLR0(const ItemSet& I, const Grammar& g);
    ItemSet gotoLR0(const ItemSet& I, const std::string& X, const Grammar& g);
    void buildCanonicalLR0(const Grammar& g);

    // LR(1) Item Operations [cite: 134, 145, 150]
    ItemSet closureLR1(const ItemSet& I, const Grammar& g);
    ItemSet gotoLR1(const ItemSet& I, const std::string& X, const Grammar& g);
    void buildCanonicalLR1(const Grammar& g);
};

#endif