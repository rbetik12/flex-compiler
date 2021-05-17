#include "symtab.h"
#include <vector>
#include <unordered_map>
#include <string>

static std::unordered_map<std::string, std::vector<int>> symbol_table;

void insert(char* _var_name, int _lineno) {
    std::string var_name = std::string(_var_name);

    if (symbol_table.find(var_name) == symbol_table.end()) {
        symbol_table[var_name] = std::vector<int> {
            _lineno
        };
    }
    else {
        symbol_table[var_name].push_back(_lineno);
    }
}

void dump_table(FILE* file) {
    std::string output_string;
    for (const auto& el: symbol_table) {
        output_string.clear();
        output_string.append(el.first);
        output_string += ": ";
        for (const auto& lineno: el.second) {
            output_string.append(std::to_string(lineno));
            output_string += " ";
        }
        output_string += "\n";
        fwrite(output_string.data(), output_string.size(), 1, file);
    }
}