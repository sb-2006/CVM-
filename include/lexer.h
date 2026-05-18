#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "token.h"

class Lexer {
public:
    Lexer(std::string source);
    std::vector<Token> scanTokens();

private:
    std::string source;
    std::vector<Token> tokens;
    
    int start = 0;
    int current = 0;
    int line = 1;
    
    std::unordered_map<std::string, TokenType> keywords;

    bool isAtEnd();
    void scanToken();
    char advance();
    bool match(char expected);
    char peek();
    void addToken(TokenType type);
    void number();
    void identifier();
};