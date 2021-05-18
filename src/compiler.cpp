#include "compiler.h"
#include <iostream>
#include <cstring>
#include <unordered_set>

#define SPACE_AMOUNT 7

static std::unordered_set<std::string> variables_list;

void print_ast(Ast* node, int offset) {
    if (node == nullptr) return;

    offset += SPACE_AMOUNT;
    print_ast(node->right, offset);
    for (int i = SPACE_AMOUNT; i < offset; i++) {
        if (offset > SPACE_AMOUNT && i >= offset - SPACE_AMOUNT) {
            if (i == offset - SPACE_AMOUNT) {
                printf("+");
            }
            else {
                printf("-");
            }
        }
        else {
            printf(" ");
        }
    }
    printf("(%s)\n", node->node_type.c_str());
    print_ast(node->left, offset);
}

Ast *ast_node(char *name, Ast *left, Ast *right) {
    Ast* node = new Ast(name);
    node->left = left;
    node->right = right;

    if (strcmp(name, "root") == 0) {
        print_ast(node, 0);
    }
    return node;
}

Ast *add_variable(char *name, Ast *variables) {
    Ast* node = new Ast("Var list");
    node->left = new Ast(name);
    node->right = variables;
    variables_list.insert(std::string(name));
    return node;
}

Ast *add_assignment(char *name, Ast *expression) {
    Ast* node = new Ast("=");
    node->left = new Ast(name);
    node->right = expression;
    return node;
}

Ast *add_flow(char *name, Ast *cond, Ast *then_op, Ast *else_op) {
    auto* flow = new Ast();
    flow->node_type = name;
    flow->left = cond;
    if (else_op) {
        flow->right = ast_node("ThenElse", then_op, else_op);
    }
    else {
        flow->right = ast_node("Then", then_op, nullptr);
    }
    return flow;
}

Ast *get_variable(char *name) {
    if (variables_list.find(std::string(name)) == variables_list.end()) {
        fprintf(stderr, "Variable %s wasn't declared!\n", name);
        exit(EXIT_FAILURE);
    }
    Ast* tree = new Ast(name);
    return tree;
}

Ast *get_constant(char *name) {
    Ast* tree = new Ast(name);
    return tree;
}
