#include "parser.h"
#include <stdexcept>
#include <iostream>

Parser::Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}

std::vector<std::unique_ptr<Stmt>> Parser::parse() {
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!isAtEnd()) {
        statements.push_back(declaration());
    }
    return statements;
}

std::unique_ptr<Stmt> Parser::declaration() {
    if (match({TokenType::FN})) {
        Token name = consume(TokenType::IDENTIFIER, "Expect function name.");
        consume(TokenType::LEFT_PAREN, "Expect '(' after function name.");
        
        std::vector<Token> parameters;
        if (!check(TokenType::RIGHT_PAREN)) {
            do {
                parameters.push_back(consume(TokenType::IDENTIFIER, "Expect parameter name."));
            } while (match({TokenType::COMMA}));
        }
        consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");
        consume(TokenType::LEFT_BRACE, "Expect '{' before function body.");
        
        std::vector<std::unique_ptr<Stmt>> body = block();
        return std::make_unique<FunctionDeclStmt>(name, parameters, std::move(body));
    }
    
    if (match({TokenType::LET})) {
        Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
        consume(TokenType::EQUAL, "Expect '=' after variable name.");
        std::unique_ptr<Expr> initializer = expression();
        consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
        return std::make_unique<VarDeclStmt>(name, std::move(initializer));
    }
    return statement(); 
}

std::unique_ptr<Stmt> Parser::statement() {
    if (match({TokenType::IF})) return ifStatement();
    if (match({TokenType::WHILE})) return whileStatement();
    if (match({TokenType::FOR})) return forStatement();
    if (match({TokenType::RETURN_KW})) return returnStatement();
    if (match({TokenType::PRINT})) return printStatement();
    if (match({TokenType::LEFT_BRACE})) return std::make_unique<BlockStmt>(block());
    return expressionStatement();
}

std::unique_ptr<Stmt> Parser::returnStatement() {
    Token keyword = previous();
    std::unique_ptr<Expr> value = nullptr;
    
    if (!check(TokenType::SEMICOLON)) {
        value = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after return value.");
    return std::make_unique<ReturnStmt>(keyword, std::move(value));
}

std::unique_ptr<Stmt> Parser::ifStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
    std::unique_ptr<Expr> condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");

    std::unique_ptr<Stmt> thenBranch = statement();
    std::unique_ptr<Stmt> elseBranch = nullptr;
    if (match({TokenType::ELSE})) {
        elseBranch = statement();
    }

    return std::make_unique<IfStmt>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

std::unique_ptr<Stmt> Parser::forStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'for'.");

    
    std::unique_ptr<Stmt> initializer;
    if (match({TokenType::SEMICOLON})) {
        initializer = nullptr;
    } else {
        initializer = declaration(); 
    }

    
    std::unique_ptr<Expr> condition = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        condition = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after loop condition.");

    
    std::unique_ptr<Expr> increment = nullptr;
    if (!check(TokenType::RIGHT_PAREN)) {
        increment = expression();
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after for clauses.");

    
    std::unique_ptr<Stmt> body = statement();

    

    
    if (increment != nullptr) {
        std::vector<std::unique_ptr<Stmt>> bodyStmts;
        bodyStmts.push_back(std::move(body));
        bodyStmts.push_back(std::make_unique<ExprStmt>(std::move(increment)));
        body = std::make_unique<BlockStmt>(std::move(bodyStmts));
    }

    
    if (condition == nullptr) {
        Token trueToken{TokenType::TRUE_KW, "true", previous().line};
        condition = std::make_unique<LiteralExpr>(trueToken);
    }
    body = std::make_unique<WhileStmt>(std::move(condition), std::move(body));

    
    if (initializer != nullptr) {
        std::vector<std::unique_ptr<Stmt>> initStmts;
        initStmts.push_back(std::move(initializer));
        initStmts.push_back(std::move(body));
        body = std::make_unique<BlockStmt>(std::move(initStmts));
    }

    return body;
}

std::unique_ptr<Stmt> Parser::whileStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
    std::unique_ptr<Expr> condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
    
    std::unique_ptr<Stmt> body = statement();
    return std::make_unique<WhileStmt>(std::move(condition), std::move(body));
}

std::vector<std::unique_ptr<Stmt>> Parser::block() {
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }
    consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}

std::unique_ptr<Stmt> Parser::printStatement() {
    std::unique_ptr<Expr> value = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");
    return std::make_unique<PrintStmt>(std::move(value));
}

std::unique_ptr<Stmt> Parser::expressionStatement() {
    std::unique_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression.");
    return std::make_unique<ExprStmt>(std::move(expr));
}

std::unique_ptr<Expr> Parser::expression() { return assignment(); }

std::unique_ptr<Expr> Parser::assignment() {
    std::unique_ptr<Expr> expr = logicalOr(); 

    if (match({TokenType::EQUAL})) {
        Token equals = previous();
        std::unique_ptr<Expr> value = assignment(); 

        if (auto* varExpr = dynamic_cast<VariableExpr*>(expr.get())) {
            Token name = varExpr->name;
            return std::make_unique<AssignExpr>(name, std::move(value));
        }
        else if (auto* getExpr = dynamic_cast<IndexGetExpr*>(expr.get())) {
            return std::make_unique<IndexSetExpr>(std::move(getExpr->target), std::move(getExpr->index), std::move(value));
        }
        throw std::runtime_error("Invalid assignment target.");
    }
    return expr;
}
std::unique_ptr<Expr> Parser::logicalOr() {
    std::unique_ptr<Expr> expr = logicalAnd();
    while (match({TokenType::OR_OR})) {
        Token op = previous();
        std::unique_ptr<Expr> right = logicalAnd();
        expr = std::make_unique<LogicalExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::logicalAnd() {
    std::unique_ptr<Expr> expr = bitwiseOr();
    while (match({TokenType::AND_AND})) {
        Token op = previous();
        std::unique_ptr<Expr> right = bitwiseOr();
        expr = std::make_unique<LogicalExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::bitwiseOr() {
    std::unique_ptr<Expr> expr = bitwiseXor();
    while (match({TokenType::PIPE})) {
        Token op = previous();
        std::unique_ptr<Expr> right = bitwiseXor();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::bitwiseXor() {
    std::unique_ptr<Expr> expr = bitwiseAnd();
    while (match({TokenType::CARET})) {
        Token op = previous();
        std::unique_ptr<Expr> right = bitwiseAnd();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::bitwiseAnd() {
    std::unique_ptr<Expr> expr = equality(); 
    while (match({TokenType::AMPERSAND})) {
        Token op = previous();
        std::unique_ptr<Expr> right = equality();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::postfix() {
    std::unique_ptr<Expr> expr = primary();

    while (match({TokenType::LEFT_BRACKET})) {
        std::unique_ptr<Expr> index = expression();
        consume(TokenType::RIGHT_BRACKET, "Expect ']' after index.");
        
        expr = std::make_unique<IndexGetExpr>(std::move(expr), std::move(index));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::equality() {
    std::unique_ptr<Expr> expr = comparison();
    while (match({TokenType::EQUAL_EQUAL})) {
        Token op = previous();
        std::unique_ptr<Expr> right = comparison();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::comparison() {
    std::unique_ptr<Expr> expr = term();
    
    while (match({TokenType::LESS, TokenType::GREATER})) {
        Token op = previous();
        std::unique_ptr<Expr> right = term();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::term() {
    std::unique_ptr<Expr> expr = factor();
    while (match({TokenType::MINUS, TokenType::PLUS})) {
        Token op = previous();
        std::unique_ptr<Expr> right = factor();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::factor() {
    std::unique_ptr<Expr> expr = postfix();
    while (match({TokenType::SLASH, TokenType::STAR})) {
        Token op = previous();
        std::unique_ptr<Expr> right = postfix();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::primary() {
    if (match({TokenType::NUMBER, TokenType::TRUE_KW, TokenType::FALSE_KW})) {
        return std::make_unique<LiteralExpr>(previous());
    }
    if (match({TokenType::IDENTIFIER})) {
        

        Token name = previous();
        
        
        if (match({TokenType::LEFT_PAREN})) {
            std::vector<std::unique_ptr<Expr>> arguments;
            if (!check(TokenType::RIGHT_PAREN)) {
                do {
                    arguments.push_back(expression());
                } while (match({TokenType::COMMA}));
            }
            consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");
            return std::make_unique<CallExpr>(name, std::move(arguments));
        }
        
        
        return std::make_unique<VariableExpr>(name);
    }
    if (match({TokenType::INPUT})) {
        return std::make_unique<InputExpr>();
    }
    if (match({TokenType::CLOCK_KW})) {
        return std::make_unique<ClockExpr>();
    }
    if (match({TokenType::LEFT_PAREN})) {
        std::unique_ptr<Expr> expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_unique<GroupingExpr>(std::move(expr));
    }
    if (match({TokenType::SQRT, TokenType::ABS, TokenType::LEN})) {
        Token funcName = previous();
        consume(TokenType::LEFT_PAREN, "Expect '(' after function name.");
        std::unique_ptr<Expr> arg = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after argument.");
        return std::make_unique<BuiltinUnaryExpr>(funcName, std::move(arg));
    }
    
    if (match({TokenType::MIN, TokenType::MAX, TokenType::POW})) {
        Token funcName = previous();
        consume(TokenType::LEFT_PAREN, "Expect '(' after function name.");
        std::unique_ptr<Expr> left = expression();
        consume(TokenType::COMMA, "Expect ',' between arguments.");
        std::unique_ptr<Expr> right = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");
        return std::make_unique<BuiltinBinaryExpr>(funcName, std::move(left), std::move(right));
    }
    if (match({TokenType::LEFT_BRACKET})) {
        std::vector<std::unique_ptr<Expr>> elements;
        if (!check(TokenType::RIGHT_BRACKET)) {
            do {
                elements.push_back(expression());
            } while (match({TokenType::COMMA}));
        }
        consume(TokenType::RIGHT_BRACKET, "Expect ']' after array elements.");
        return std::make_unique<ArrayExpr>(std::move(elements));
    }
    throw std::runtime_error("Expect expression.");
}


bool Parser::match(std::initializer_list<TokenType> types) {
    for (TokenType type : types) {
        if (check(type)) { advance(); return true; }
    }
    return false;
}
bool Parser::check(TokenType type) { return isAtEnd() ? false : peek().type == type; }
Token Parser::advance() { if (!isAtEnd()) current++; return previous(); }
bool Parser::isAtEnd() { return peek().type == TokenType::END_OF_FILE; }
Token Parser::peek() { return tokens[current]; }
Token Parser::previous() { return tokens[current - 1]; }
Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    throw std::runtime_error(message);
}