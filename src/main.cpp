#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "lexer.h"
#include "parser.h"
#include "compiler.h"
#include "vm.h"
#include "ast_printer.h"
#include "disassembler.h"

void run(const std::string& sourceCode, bool showAST, bool showBytecode) {
    // --- Stage 1: Lex ---
    Lexer lexer(sourceCode);
    std::vector<Token> tokens = lexer.scanTokens();

    // --- Stage 2: Parse -> AST ---
    Parser parser(tokens);
    std::vector<std::unique_ptr<Stmt>> ast;
    try {
        ast = parser.parse();
    } catch (const std::exception& e) {
        std::cerr << "Parser Error: " << e.what() << std::endl;
        return;
    }

    // --- Optional: Print AST ---
    if (showAST) {
        ASTPrinter printer;
        printer.print(ast);
    }

    // --- Stage 3: Compile -> Bytecode ---
    Compiler compiler;
    compiler.compile(ast);

    // --- Optional: Print Bytecode ---
    if (showBytecode) {
        Disassembler disasm;
        disasm.print(
            compiler.getBytecode(),
            compiler.getConstants(),
            compiler.getStringConstants()
        );
    }

    // --- Stage 4: Execute ---
    VM vm(
        compiler.getBytecode(),
        compiler.getConstants(),
        compiler.getStringConstants(),
        compiler.getFunctions()
    );
    vm.run();
}

void runFile(const std::string& path, bool showAST, bool showBytecode) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << path << "'\n";
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    run(buffer.str(), showAST, showBytecode);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: cvm <script.cvm> [--ast] [--bytecode]\n";
        std::cout << "  --ast       Print the Abstract Syntax Tree after parsing\n";
        std::cout << "  --bytecode  Print the compiled bytecode before execution\n";
        return 1;
    }

    std::string filePath = argv[1];
    bool showAST      = false;
    bool showBytecode = false;

    // Parse any extra flags (order doesn't matter)
    for (int i = 2; i < argc; i++) {
        std::string flag = argv[i];
        if (flag == "--ast")      showAST      = true;
        if (flag == "--bytecode") showBytecode = true;
    }

    runFile(filePath, showAST, showBytecode);
    return 0;
}
