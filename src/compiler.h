#pragma once
#include <unordered_set>
#include <string>
#include <utility>

struct Ast {
    std::string node_type;
    std::string name;
    Ast* left;
    Ast* right;

    Ast(): node_type(), left(nullptr), right(nullptr) {}
    Ast(std::string _node_type): node_type(std::move(_node_type)), left(nullptr), right(nullptr) {}
};

struct AstFlow {
    std::string node_type;
    Ast* cond;
    Ast* then_op;
    Ast* else_op;
};

#ifdef __cplusplus
extern "C" {
#endif

Ast* add_variable(char* name, Ast* variables);
Ast* ast_node(char* name, Ast* left, Ast* right);
Ast* add_assignment(char* name, Ast* expression);
Ast* add_flow(char* name, Ast* cond, Ast* then_op, Ast* else_op);
Ast* get_variable(char* name);
Ast* get_constant(char* name);

#ifdef __cplusplus
}
#endif