// Author: Ruhab Ahmad (23I-0559)
// Assignment 03: Bottom-Up Parser Design & Implementation
#include "parsing_table.h"
#include <iostream>
#include <fstream>
#include <iomanip>

void ParsingTableFormatter::printTable(
    const std::map<std::pair<int, std::string>, Action>& action_table,
    const std::map<std::pair<int, std::string>, int>& goto_table,
    const std::vector<std::string>& terminals,
    const std::vector<std::string>& non_terminals,
    int num_states,
    const std::string& outputFile) 
{
    std::ofstream out(outputFile);
    if (!out.is_open()) {
        std::cerr << "Failed to open " << outputFile << " for writing.\n";
        return;
    }

    out << "State\t| ACTION";
    for (size_t i = 0; i < terminals.size() * 8; ++i) out << " ";
    out << "| GOTO\n";
    
    // Print headers
    out << "     \t| ";
    for (const auto& t : terminals) out << std::setw(8) << t;
    out << " | ";
    for (const auto& nt : non_terminals) out << std::setw(8) << nt;
    out << "\n";
    
    out << std::string(100, '-') << "\n";

    // Print rows
    for (int i = 0; i < num_states; ++i) {
        out << std::setw(5) << i << "\t| ";
        
        for (const auto& t : terminals) {
            auto it = action_table.find({i, t});
            if (it != action_table.end()) {
                if (it->second.type == ActionType::SHIFT) out << std::setw(7) << "S" + std::to_string(it->second.target) << " ";
                else if (it->second.type == ActionType::REDUCE) out << std::setw(7) << "R" + std::to_string(it->second.target) << " ";
                else if (it->second.type == ActionType::ACCEPT) out << std::setw(8) << "ACC ";
            } else {
                out << std::setw(8) << " ";
            }
        }
        
        out << " | ";
        for (const auto& nt : non_terminals) {
            auto it = goto_table.find({i, nt});
            if (it != goto_table.end()) {
                out << std::setw(8) << it->second;
            } else {
                out << std::setw(8) << " ";
            }
        }
        out << "\n";
    }
    
    out.close();
    std::cout << "Table written to " << outputFile << "\n";
}