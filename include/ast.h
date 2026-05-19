#pragma once
#include "ast.h"
#include <iostream>
#include <string>

class ASTPrinter {
public:
    void print(const std::vector<std::unique_ptr<Stmt>>& statements) {
        std::cout << "\n=== ABSTRACT SYNTAX TREE ===\n";
        for (const auto& stmt : statements) {
            printStmt(stmt.get(), 0);
        }
        std::cout << "============================\n\n";
    }

private:
    std::string indent(int depth) {
        return std::string(depth * 2, ' ');
    }

    void printStmt(Stmt* stmt, int depth) {
        if (auto* s = dynamic_cast<PrintStmt*>(stmt)) {
            std::cout << indent(depth) << "[PrintStmt]\n";
            printExpr(s->expression.get(), depth + 1);

        } else if (auto* s = dynamic_cast<VarDeclStmt*>(stmt)) {
            std::cout << indent(depth) << "[VarDecl] name=" << s->name.lexeme << "\n";
            printExpr(s->initializer.get(), depth + 1);

        } else if (auto* s = dynamic_cast<ExprStmt*>(stmt)) {
            std::cout << indent(depth) << "[ExprStmt]\n";
            printExpr(s->expression.get(), depth + 1);

        } else if (auto* s = dynamic_cast<BlockStmt*>(stmt)) {
            std::cout << indent(depth) << "[Block]\n";
            for (const auto& inner : s->statements)
                printStmt(inner.get(), depth + 1);

        } else if (auto* s = dynamic_cast<IfStmt*>(stmt)) {
            std::cout << indent(depth) << "[If]\n";
            std::cout << indent(depth + 1) << "condition:\n";
            printExpr(s->condition.get(), depth + 2);
            std::cout << indent(depth + 1) << "then:\n";
            printStmt(s->thenBranch.get(), depth + 2);
            if (s->elseBranch) {
                std::cout << indent(depth + 1) << "else:\n";
                printStmt(s->elseBranch.get(), depth + 2);
            }

        } else if (auto* s = dynamic_cast<WhileStmt*>(stmt)) {
            std::cout << indent(depth) << "[While]\n";
            std::cout << indent(depth + 1) << "condition:\n";
            printExpr(s->condition.get(), depth + 2);
            std::cout << indent(depth + 1) << "body:\n";
            printStmt(s->body.get(), depth + 2);

        } else if (auto* s = dynamic_cast<FunctionDeclStmt*>(stmt)) {
            std::cout << indent(depth) << "[FunctionDecl] name=" << s->name.lexeme << " params=(";
            for (size_t i = 0; i < s->parameters.size(); i++) {
                if (i > 0) std::cout << ", ";
                std::cout << s->parameters[i].lexeme;
            }
            std::cout << ")\n";
            for (const auto& bodyStmt : s->body)
                printStmt(bodyStmt.get(), depth + 1);

        } else if (auto* s = dynamic_cast<ReturnStmt*>(stmt)) {
            std::cout << indent(depth) << "[Return]\n";
            if (s->value) printExpr(s->value.get(), depth + 1);

        } else {
            std::cout << indent(depth) << "[UnknownStmt]\n";
        }
    }

    void printExpr(Expr* expr, int depth) {
        if (auto* e = dynamic_cast<LiteralExpr*>(expr)) {
            std::cout << indent(depth) << "[Literal] " << e->value.lexeme << "\n";

        } else if (auto* e = dynamic_cast<VariableExpr*>(expr)) {
            std::cout << indent(depth) << "[Variable] " << e->name.lexeme << "\n";

        } else if (auto* e = dynamic_cast<AssignExpr*>(expr)) {
            std::cout << indent(depth) << "[Assign] " << e->name.lexeme << " =\n";
            printExpr(e->value.get(), depth + 1);

        } else if (auto* e = dynamic_cast<BinaryExpr*>(expr)) {
            std::cout << indent(depth) << "[Binary] op=" << e->op.lexeme << "\n";
            printExpr(e->left.get(),  depth + 1);
            printExpr(e->right.get(), depth + 1);

        } else if (auto* e = dynamic_cast<LogicalExpr*>(expr)) {
            std::cout << indent(depth) << "[Logical] op=" << e->op.lexeme << "\n";
            printExpr(e->left.get(),  depth + 1);
            printExpr(e->right.get(), depth + 1);

        } else if (auto* e = dynamic_cast<GroupingExpr*>(expr)) {
            std::cout << indent(depth) << "[Grouping]\n";
            printExpr(e->expression.get(), depth + 1);

        } else if (dynamic_cast<InputExpr*>(expr)) {
            std::cout << indent(depth) << "[Input]\n";

        } else if (dynamic_cast<ClockExpr*>(expr)) {
            std::cout << indent(depth) << "[Clock]\n";

        } else if (auto* e = dynamic_cast<BuiltinUnaryExpr*>(expr)) {
            std::cout << indent(depth) << "[BuiltinUnary] fn=" << e->functionName.lexeme << "\n";
            printExpr(e->argument.get(), depth + 1);

        } else if (auto* e = dynamic_cast<BuiltinBinaryExpr*>(expr)) {
            std::cout << indent(depth) << "[BuiltinBinary] fn=" << e->functionName.lexeme << "\n";
            printExpr(e->left.get(),  depth + 1);
            printExpr(e->right.get(), depth + 1);

        } else if (auto* e = dynamic_cast<CallExpr*>(expr)) {
            std::cout << indent(depth) << "[Call] fn=" << e->calleeName.lexeme << " args=" << e->arguments.size() << "\n";
            for (const auto& arg : e->arguments)
                printExpr(arg.get(), depth + 1);

        } else if (auto* e = dynamic_cast<ArrayExpr*>(expr)) {
            std::cout << indent(depth) << "[Array] size=" << e->elements.size() << "\n";
            for (const auto& el : e->elements)
                printExpr(el.get(), depth + 1);

        } else if (auto* e = dynamic_cast<IndexGetExpr*>(expr)) {
            std::cout << indent(depth) << "[IndexGet]\n";
            printExpr(e->target.get(), depth + 1);
            printExpr(e->index.get(),  depth + 1);

        } else if (auto* e = dynamic_cast<IndexSetExpr*>(expr)) {
            std::cout << indent(depth) << "[IndexSet]\n";
            printExpr(e->target.get(), depth + 1);
            printExpr(e->index.get(),  depth + 1);
            printExpr(e->value.get(),  depth + 1);

        } else {
            std::cout << indent(depth) << "[UnknownExpr]\n";
        }
    }
};
