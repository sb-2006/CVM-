#pragma once
#include <vector>
#include <memory>
#include <cstdint>
#include <string>
#include <unordered_map>
#include "ast.h"
#include "opcode.h"


struct FunctionInfo {
    int ipStart;
    std::vector<std::string> parameters;
};

class Compiler {
public:
    Compiler();
    void compile(const std::vector<std::unique_ptr<Stmt>>& statements);
    
    std::vector<uint8_t> getBytecode() const;
    std::vector<int> getConstants() const;
    std::vector<std::string> getStringConstants() const;
    
    
    std::unordered_map<std::string, FunctionInfo> getFunctions() const;

private:
    std::vector<uint8_t> bytecode;
    std::vector<int> constants;
    std::vector<std::string> stringConstants;
    
    
    std::unordered_map<std::string, FunctionInfo> functions;

    void compileStatement(Stmt* stmt);
    void compileExpression(Expr* expr);
    
    void emitByte(uint8_t byte);
    int addConstant(int value);
    int addStringConstant(const std::string& name);

    int emitJump(uint8_t instruction);
    void patchJump(int offset);
    void emitLoop(int loopStart);
};