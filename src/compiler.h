#pragma once

#include <unordered_set>
#include <string>
#include <utility>

enum class ASTNodeType {
    Undefined,
    Operator,
    Expression,
    Identifier,
    Const,
    FlowIf,
    FlowIfElse,
    FlowOp,
    UMinus,
    Root,
    ProgramBody,
    VarDecl,
    VarList,
    Operators,
};

struct Ast {
    ASTNodeType node_type;
    char *value;
    Ast *left;
    Ast *right;

    Ast() : node_type(ASTNodeType::Undefined), value(nullptr), left(nullptr), right(nullptr) {}

    Ast(ASTNodeType _node_type, char *_value) : node_type(_node_type), value(_value), left(nullptr), right(nullptr) {}
};

#ifdef __cplusplus
extern "C" {
#endif

Ast *add_variable(char *name, Ast *variables);
Ast *ast_node(ASTNodeType type, char *value, Ast *left, Ast *right);
Ast *add_assignment(char *name, Ast *expression);
Ast *add_flow(ASTNodeType type, Ast *cond, Ast *then_op, Ast *else_op);
Ast *get_variable(char *name);
Ast *get_constant(char *name);

#ifdef __cplusplus
}
#endif