#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "opcode.h"
#include "compiler.h" 

class VM {
public:
    VM(const std::vector<uint8_t>& bytecode, 
       const std::vector<int>& constants,
       const std::vector<std::string>& stringConstants,
       const std::unordered_map<std::string, FunctionInfo>& functions); 
    void run();

private:
    std::vector<uint8_t> bytecode;
    std::vector<int> constants;
    std::vector<std::string> stringConstants;
    std::unordered_map<std::string, FunctionInfo> functions; 
    
    std::vector<int> stack;
    
    
    size_t ip = 0;
    std::vector<size_t> callStack; 
    std::vector<std::unordered_map<std::string, int>> environments; 
    

    std::vector<std::vector<int>> heap;

    void push(int value);
    int pop();
    uint8_t readByte();
};