#pragma once
#include <string>

enum class TokenType {
    
    LEFT_PAREN, RIGHT_PAREN, 
    LEFT_BRACE, RIGHT_BRACE,
    LEFT_BRACKET, RIGHT_BRACKET,
    PLUS, MINUS, STAR, SLASH, 
    SEMICOLON, EQUAL, LESS, GREATER, COMMA,
    AMPERSAND, PIPE, CARET, 
    AND_AND, OR_OR,         

    
    EQUAL_EQUAL,

    
    IDENTIFIER, NUMBER,

    
    LET, IF, ELSE, WHILE, FOR,
    PRINT, INPUT, TRUE_KW, FALSE_KW,CLOCK_KW,
    SQRT, ABS, MIN, MAX, POW, LEN,
    FN, RETURN_KW,

    
    END_OF_FILE
};


struct Token {
    TokenType type;
    std::string lexeme; 
    int line;
};