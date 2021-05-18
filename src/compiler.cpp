#include "compiler.h"
#include <unordered_set>
#include <cstring>

#define SPACE_AMOUNT 7

static std::unordered_set<std::string> variables_list;

void print_asm_expr(Ast *node, char *name);
void print_asm_ops(Ast *node);

char *next_temp_name() {
    static int tmpCount = 0;
    char *name = new char[20];
    snprintf(name, 20, "tmp%d", tmpCount);
    tmpCount += 1;
    return name;
}

char *next_label() {
    static int labelCount = 0;
    char *name = new char[20];
    snprintf(name, 20, "L%d", labelCount);
    labelCount += 1;
    return name;
}

char *get_node_type(Ast *node) {
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

void print_ast(Ast *node, int offset) {
    if (node == nullptr) return;

    offset += SPACE_AMOUNT;
    print_ast(node->right, offset);
    for (int i = SPACE_AMOUNT; i < offset; i++) {
        if (offset > SPACE_AMOUNT && i >= offset - SPACE_AMOUNT) {
            if (i == offset - SPACE_AMOUNT) {
                printf("+");
            } else {
                printf("-");
            }
        } else {
            printf(" ");
        }
    }
    char *type = get_node_type(node);
    if (type) {
        printf("[%s]", type);
    } else {
        printf("[Shit happened]");
    }
    if (node->value) {
        printf("(%s)\n", node->value);
    } else {
        printf("(NULL)\n");
    }
    print_ast(node->left, offset);
}

void print_asm_if(Ast *node) {
    if (!node) return;

    char *tmp_name = next_temp_name();
    char *not_label = next_label();
    char *cmp_block_end_label = next_label();
    print_asm_expr(node->left, tmp_name);
    printf("cmptrue %s %s\n", tmp_name, not_label);
    print_asm_ops(node->right->left);
    printf("goto %s\n", cmp_block_end_label);
    printf("%s:\n", not_label);
    if (node->right->right) {
        print_asm_ops(node->right->right);
    }
    printf("%s:\n", cmp_block_end_label);
}

void print_asm_expr(Ast *node, char *name) {
    if (!node) return;
    Ast *left = node->left;
    Ast *right = node->right;

    char *tempName = nullptr;
    if (left) {
        switch (left->node_type) {
            case ASTNodeType::Expression:
                tempName = next_temp_name();
                print_asm_expr(left, tempName);
                break;
            case ASTNodeType::Const:
            case ASTNodeType::Identifier:
                tempName = left->value;
                break;
            default:
                printf("Asm expr error!\n");
                tempName = "Error";
                break;
        }
    }

    char *tempName2 = nullptr;

    switch (right->node_type) {
        case ASTNodeType::Expression:
            tempName2 = next_temp_name();
            print_asm_expr(right, tempName2);
            break;
        case ASTNodeType::Const:
        case ASTNodeType::Identifier:
            tempName2 = right->value;
            break;
        default:
            printf("Asm expr error!\n");
            tempName2 = "Error";
            break;
    }

    if (strcmp(node->value, "-") == 0
        || strcmp(node->value, "+") == 0
        || strcmp(node->value, "*") == 0
        || strcmp(node->value, "/") == 0
        || strcmp(node->value, ">") == 0
        || strcmp(node->value, "<") == 0
        || strcmp(node->value, "====") == 0
            ) {
        printf("%s = %s %s %s\n", name, tempName, node->value, tempName2);
    } else if (strcmp(node->value, "U") == 0) {
        printf("%s = -%s\n", name, tempName2);
    } else {
        printf("Asm expr error!\n");
    }
}

void print_asm_asgn(Ast *node) {
    if (!node) return;
    Ast *variable = node->left;
    Ast *expression = node->right;

    switch (expression->node_type) {
        case ASTNodeType::Const:
        case ASTNodeType::Identifier:
            printf("%s %s %s\n", variable->value, node->value, expression->value);
            break;
        case ASTNodeType::Expression:
            char *tmp_name = next_temp_name();
            print_asm_expr(expression, tmp_name);
            printf("%s %s %s\n", variable->value, node->value, tmp_name);
            break;
    }
}

void print_asm_ops(Ast *node) {
    if (!node) return;

    switch (node->node_type) {
        case ASTNodeType::Operators:
            print_asm_ops(node->left);
            print_asm_ops(node->right);
            break;
        case ASTNodeType::Operator:
            print_asm_asgn(node);
            break;
        case ASTNodeType::FlowIf:
        case ASTNodeType::FlowIfElse:
            print_asm_if(node);
            break;
        default:
            printf("Error\n");
            break;
    }
}

void print_asm(Ast *root) {
    if (!root) return;

    if (!root->right) {
        printf("Empty program\n");
        return;
    }
    printf("\n");
    print_asm_ops(root->right->left);
}

Ast *ast_node(ASTNodeType type, char *value, Ast *left, Ast *right) {
    Ast *node = new Ast(type, value);
    node->left = left;
    node->right = right;

    if (type == ASTNodeType::Root) {
        print_ast(node, 0);
        print_asm(node);
    }
    return node;
}

Ast *add_variable(char *value, Ast *variables) {
    Ast *node = new Ast(ASTNodeType::VarList, value);
    node->right = variables;
    variables_list.insert(std::string(value));
    return node;
}

Ast *add_assignment(char *name, Ast *expression) {
    Ast *node = new Ast(ASTNodeType::Operator, "=");
    node->left = get_variable(name);
    node->right = expression;
    return node;
}

Ast *add_flow(ASTNodeType type, Ast *cond, Ast *then_op, Ast *else_op) {
    auto *flow = new Ast(type, nullptr);
    flow->left = cond;
    if (else_op) {
        flow->right = ast_node(ASTNodeType::FlowOp, nullptr, then_op, else_op);
    } else {
        flow->right = ast_node(ASTNodeType::FlowOp, nullptr, then_op, nullptr);
    }
    return flow;
}

Ast *get_variable(char *name) {
    if (variables_list.find(std::string(name)) == variables_list.end()) {
        fprintf(stderr, "Variable %s wasn't declared!\n", name);
        exit(EXIT_FAILURE);
    }
    Ast *tree = new Ast(ASTNodeType::Identifier, name);
    return tree;
}

Ast *get_constant(char *name) {
    Ast *tree = new Ast(ASTNodeType::Const, name);
    return tree;
}
