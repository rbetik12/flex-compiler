#include "compiler.h"
#include <unordered_set>

#define SPACE_AMOUNT 7

static std::unordered_set<std::string> variables_list;

char* get_node_type(Ast* node) {
    if (node == nullptr) return nullptr;
    switch (node->node_type) {
        case ASTNodeType::Undefined:
            return "Undefined";
        case ASTNodeType::Operator:
            return "Operator";
        case ASTNodeType::Expression:
            return "Expression";
        case ASTNodeType::Identifier:
            return "Identifier";
        case ASTNodeType::Const:
            return "Const";
        case ASTNodeType::FlowIf:
            return "FlowIf";
        case ASTNodeType::FlowIfElse:
            return "FlowIfElse";
        case ASTNodeType::FlowOp:
            return "FlowOp";
        case ASTNodeType::UMinus:
            return "UMinus";
        case ASTNodeType::Root:
            return "Root";
        case ASTNodeType::ProgramBody:
            return "Program body";
        case ASTNodeType::VarDecl:
            return "VarDecl";
        case ASTNodeType::VarList:
            return "VarList";
        case ASTNodeType::Operators:
            return "Operators";
        default:
            return nullptr;
    }
}

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
    char* type = get_node_type(node);
    if (type) {
        printf("[%s]", type);
    }
    else {
        printf("[Shit happened]");
    }
    if (node->value) {
        printf("(%s)\n", node->value);
    }
    else {
        printf("(NULL)\n");
    }
    print_ast(node->left, offset);
}

Ast *ast_node(ASTNodeType type, char *value, Ast *left, Ast *right) {
    Ast* node = new Ast(type, value);
    node->left = left;
    node->right = right;

    if (type == ASTNodeType::Root) {
        print_ast(node, 0);
    }
    return node;
}

Ast *add_variable(char *value, Ast *variables) {
    Ast* node = new Ast(ASTNodeType::VarList, value);
    node->right = variables;
    variables_list.insert(std::string(value));
    return node;
}

Ast *add_assignment(char *name, Ast *expression) {
    Ast* node = new Ast(ASTNodeType::Operator, "=");
    node->left = get_variable(name);
    node->right = expression;
    return node;
}

Ast *add_flow(ASTNodeType type, Ast *cond, Ast *then_op, Ast *else_op) {
    auto* flow = new Ast(type, nullptr);
    flow->left = cond;
    if (else_op) {
        flow->right = ast_node(ASTNodeType::FlowOp, nullptr, then_op, else_op);
    }
    else {
        flow->right = ast_node(ASTNodeType::FlowOp, nullptr, then_op, nullptr);
    }
    return flow;
}

Ast *get_variable(char *name) {
    if (variables_list.find(std::string(name)) == variables_list.end()) {
        fprintf(stderr, "Variable %s wasn't declared!\n", name);
        exit(EXIT_FAILURE);
    }
    Ast* tree = new Ast(ASTNodeType::Identifier, name);
    return tree;
}

Ast *get_constant(char *name) {
    Ast* tree = new Ast(ASTNodeType::Const, name);
    return tree;
}
