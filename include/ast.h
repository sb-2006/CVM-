#pragma once
#include "token.h"
#include <memory>
#include <vector>


struct Expr { virtual ~Expr() = default; };

struct LiteralExpr : public Expr {
    Token value;
    LiteralExpr(Token value) : value(value) {}
};

struct VariableExpr : public Expr {
    Token name;
    VariableExpr(Token name) : name(name) {}
};

struct AssignExpr : public Expr {
    Token name;
    std::unique_ptr<Expr> value;
    AssignExpr(Token name, std::unique_ptr<Expr> value) : name(name), value(std::move(value)) {}
};

struct BinaryExpr : public Expr {
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;
    BinaryExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}
};

struct GroupingExpr : public Expr {
    std::unique_ptr<Expr> expression;
    GroupingExpr(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {}
};

struct InputExpr : public Expr { InputExpr() = default; };

struct ClockExpr : public Expr { ClockExpr() = default; };

struct BuiltinUnaryExpr : public Expr {
    Token functionName;
    std::unique_ptr<Expr> argument;
    BuiltinUnaryExpr(Token functionName, std::unique_ptr<Expr> argument)
        : functionName(functionName), argument(std::move(argument)) {}
};

struct CallExpr : public Expr {
    Token calleeName;
    std::vector<std::unique_ptr<Expr>> arguments;
    CallExpr(Token calleeName, std::vector<std::unique_ptr<Expr>> arguments)
        : calleeName(calleeName), arguments(std::move(arguments)) {}
};


struct BuiltinBinaryExpr : public Expr {
    Token functionName;
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
    BuiltinBinaryExpr(Token functionName, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right)
        : functionName(functionName), left(std::move(left)), right(std::move(right)) {}
};

struct ArrayExpr : public Expr {
    std::vector<std::unique_ptr<Expr>> elements;
    ArrayExpr(std::vector<std::unique_ptr<Expr>> elements) : elements(std::move(elements)) {}
};


struct IndexGetExpr : public Expr {
    std::unique_ptr<Expr> target;
    std::unique_ptr<Expr> index;
    IndexGetExpr(std::unique_ptr<Expr> target, std::unique_ptr<Expr> index)
        : target(std::move(target)), index(std::move(index)) {}
};


struct IndexSetExpr : public Expr {
    std::unique_ptr<Expr> target;
    std::unique_ptr<Expr> index;
    std::unique_ptr<Expr> value;
    IndexSetExpr(std::unique_ptr<Expr> target, std::unique_ptr<Expr> index, std::unique_ptr<Expr> value)
        : target(std::move(target)), index(std::move(index)), value(std::move(value)) {}
};


struct Stmt { virtual ~Stmt() = default; };

struct PrintStmt : public Stmt {
    std::unique_ptr<Expr> expression;
    PrintStmt(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {}
};

struct FunctionDeclStmt : public Stmt {
    Token name;
    std::vector<Token> parameters;
    std::vector<std::unique_ptr<Stmt>> body;
    FunctionDeclStmt(Token name, std::vector<Token> parameters, std::vector<std::unique_ptr<Stmt>> body)
        : name(name), parameters(parameters), body(std::move(body)) {}
};

struct ReturnStmt : public Stmt {
    Token keyword;
    std::unique_ptr<Expr> value; 
    ReturnStmt(Token keyword, std::unique_ptr<Expr> value)
        : keyword(keyword), value(std::move(value)) {}
};

struct ExprStmt : public Stmt {
    std::unique_ptr<Expr> expression;
    ExprStmt(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {}
};

struct VarDeclStmt : public Stmt {
    Token name;
    std::unique_ptr<Expr> initializer;
    VarDeclStmt(Token name, std::unique_ptr<Expr> initializer)
        : name(name), initializer(std::move(initializer)) {}
};


struct BlockStmt : public Stmt {
    std::vector<std::unique_ptr<Stmt>> statements;
    BlockStmt(std::vector<std::unique_ptr<Stmt>> statements) : statements(std::move(statements)) {}
};


struct IfStmt : public Stmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch; 
    IfStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch)
        : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}
};


struct WhileStmt : public Stmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;
    WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body)
        : condition(std::move(condition)), body(std::move(body)) {}
};

struct LogicalExpr : public Expr {
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;
    LogicalExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}
};