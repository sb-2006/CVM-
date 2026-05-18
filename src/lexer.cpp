#include "lexer.h"
#include <cctype>

Lexer::Lexer(std::string source) : source(std::move(source)) {
    
    keywords["let"] = TokenType::LET;
    keywords["if"] = TokenType::IF;
    keywords["else"] = TokenType::ELSE;
    keywords["while"] = TokenType::WHILE;
    keywords["for"] = TokenType::FOR;
    keywords["print"] = TokenType::PRINT;
    keywords["input"] = TokenType::INPUT;
    keywords["true"] = TokenType::TRUE_KW;
    keywords["false"] = TokenType::FALSE_KW;
    keywords["clock"] = TokenType::CLOCK_KW;
    keywords["sqrt"] = TokenType::SQRT;
    keywords["abs"] = TokenType::ABS;
    keywords["min"] = TokenType::MIN;
    keywords["max"] = TokenType::MAX;
    keywords["pow"] = TokenType::POW;
    keywords["len"] = TokenType::LEN;
    keywords["fn"] = TokenType::FN;
    keywords["return"] = TokenType::RETURN_KW;
}

std::vector<Token> Lexer::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }
    tokens.push_back({TokenType::END_OF_FILE, "", line});
    return tokens;
}

void Lexer::scanToken() {
    char c = advance();
    switch (c) {
        
        case '(': addToken(TokenType::LEFT_PAREN); break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case '{': addToken(TokenType::LEFT_BRACE); break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        case '-': addToken(TokenType::MINUS); break;
        case '+': addToken(TokenType::PLUS); break;
        case '*': addToken(TokenType::STAR); break;
        case '[': addToken(TokenType::LEFT_BRACKET); break;
        case ']': addToken(TokenType::RIGHT_BRACKET); break;
       case '/':
            if (match('/')) {
                
                while (peek() != '\n' && !isAtEnd()) advance();
            } else {
                
                addToken(TokenType::SLASH);
            }
            break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case '&': 
            addToken(match('&') ? TokenType::AND_AND : TokenType::AMPERSAND); 
            break;
        case '|': 
            addToken(match('|') ? TokenType::OR_OR : TokenType::PIPE); 
            break;
        case '^': 
            addToken(TokenType::CARET); 
            break;
        case '<': addToken(TokenType::LESS); break;
        case '>': addToken(TokenType::GREATER); break;
        case ',': addToken(TokenType::COMMA); break;
        
        
        case '=':
            addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;

        
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            line++;
            break;

        default:
            if (std::isdigit(c)) {
                number();
            } else if (std::isalpha(c) || c == '_') {
                identifier();
            } else {
                
            }
            break;
    }
}

void Lexer::identifier() {
    while (std::isalnum(peek()) || peek() == '_') advance();

    std::string text = source.substr(start, current - start);
    TokenType type = TokenType::IDENTIFIER;
    
    if (keywords.find(text) != keywords.end()) {
        type = keywords[text];
    }
    
    addToken(type);
}

void Lexer::number() {
    while (std::isdigit(peek())) advance();
    addToken(TokenType::NUMBER);
}

bool Lexer::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;
    current++;
    return true;
}

char Lexer::peek() {
    if (isAtEnd()) return '\0';
    return source[current];
}

char Lexer::advance() {
    return source[current++];
}

bool Lexer::isAtEnd() {
    return current >= source.length();
}

void Lexer::addToken(TokenType type) {
    std::string text = source.substr(start, current - start);
    tokens.push_back({type, text, line});
}