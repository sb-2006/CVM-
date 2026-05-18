#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "lexer.h"
#include "parser.h"
#include "compiler.h"
#include "vm.h"


void run(const std::string& sourceCode) {
    Lexer lexer(sourceCode);
    std::vector<Token> tokens = lexer.scanTokens();

    Parser parser(tokens);
    std::vector<std::unique_ptr<Stmt>> ast;
    try {
        ast = parser.parse();
    } catch (const std::exception& e) {
        std::cerr << "Parser Error: " << e.what() << std::endl;
        return;
    }

    Compiler compiler;
    compiler.compile(ast);
    
    VM vm(compiler.getBytecode(), compiler.getConstants(), compiler.getStringConstants(), compiler.getFunctions());
    vm.run();
}


void runFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << path << "'\n";
        exit(1);
    }

    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string sourceCode = buffer.str();

    
    run(sourceCode);
}


int main(int argc, char* argv[]) {
    
    if (argc < 2) {
        std::cout << "Usage: ./cvm <path_to_script.cvm>\n";
        return 1;
    } 
    
    else {
        runFile(argv[1]);
    }

    return 0;
}