#include "vm.h"
#include <iostream>
#include <chrono>
#include <cmath>
#include <algorithm>

VM::VM(const std::vector<uint8_t>& bytecode, 
       const std::vector<int>& constants,
       const std::vector<std::string>& 
       stringConstants,
       const std::unordered_map<std::string, FunctionInfo>& functions)
       
    : bytecode(bytecode), constants(constants), stringConstants(stringConstants), functions(functions) {
        environments.push_back({});
}

void VM::push(int value) { stack.push_back(value); }
int VM::pop() { int value = stack.back(); stack.pop_back(); return value; }
uint8_t VM::readByte() { return bytecode[ip++]; }

void VM::run() {
    while (ip < bytecode.size()) {
        uint8_t instruction = readByte();

        switch (static_cast<OpCode>(instruction)) {
            
            case OpCode::OP_CONSTANT: { push(constants[readByte()]); break; }
            
            
            case OpCode::OP_TRUE:  { push(1); break; }
            case OpCode::OP_FALSE: { push(0); break; }

            case OpCode::OP_DEFINE_GLOBAL: {
                std::string varName = stringConstants[readByte()];
                environments.back()[varName] = pop(); 
                break;
            }
            case OpCode::OP_GET_GLOBAL: {
                std::string varName = stringConstants[readByte()];
                
                if (environments.back().count(varName)) {
                    push(environments.back()[varName]);
                } 
                
                else if (environments.front().count(varName)) {
                    push(environments.front()[varName]);
                } else { 
                    std::cerr << "Undefined variable '" << varName << "'\n"; return; 
                }
                break;
            }
            case OpCode::OP_SET_GLOBAL: {
                std::string varName = stringConstants[readByte()];
                int val = stack.back();
                if (environments.back().count(varName)) environments.back()[varName] = val;
                else if (environments.front().count(varName)) environments.front()[varName] = val;
                else { std::cerr << "Undefined variable '" << varName << "'\n"; return; }
                break;
            }
            case OpCode::OP_CALL: {
                std::string funcName = stringConstants[readByte()];
                if (functions.find(funcName) == functions.end()) {
                    std::cerr << "Unknown function '" << funcName << "'\n"; return;
                }
                FunctionInfo func = functions[funcName];
                
                
                std::unordered_map<std::string, int> localEnv;
                for (int i = func.parameters.size() - 1; i >= 0; --i) {
                    localEnv[func.parameters[i]] = pop();
                }
                
                
                callStack.push_back(ip);          
                environments.push_back(localEnv); 
                ip = func.ipStart;                
                break;
            }

            case OpCode::OP_INPUT: {
                int value; std::cout << ">> "; std::cin >> value; push(value); break;
            }
            case OpCode::OP_CLOCK: {
                
                auto now = std::chrono::system_clock::now().time_since_epoch();
                int ms = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
                push(ms);
                break;
            }
            case OpCode::OP_SQRT: {
                int a = pop();
                push(std::round(std::sqrt(a))); 
                break;
            }
            case OpCode::OP_ABS: {
                int a = pop();
                push(std::abs(a));
                break;
            }
            case OpCode::OP_MIN: {
                int b = pop(); int a = pop();
                push(std::min(a, b));
                break;
            }
            case OpCode::OP_MAX: {
                int b = pop(); int a = pop();
                push(std::max(a, b));
                break;
            }
            case OpCode::OP_POW: {
                int b = pop(); int a = pop();
                push(std::round(std::pow(a, b)));
                break;
            }
            case OpCode::OP_POP: { pop(); break; }

            case OpCode::OP_ADD: { int b = pop(); int a = pop(); push(a + b); break; }
            case OpCode::OP_SUBTRACT: { int b = pop(); int a = pop(); push(a - b); break; }
            case OpCode::OP_MULTIPLY: { int b = pop(); int a = pop(); push(a * b); break; }
            case OpCode::OP_DIVIDE: { int b = pop(); int a = pop(); push(a / b); break; }
            case OpCode::OP_BIT_AND: { int b = pop(); int a = pop(); push(a & b); break; }
            case OpCode::OP_BIT_OR:  { int b = pop(); int a = pop(); push(a | b); break; }
            case OpCode::OP_BIT_XOR: { int b = pop(); int a = pop(); push(a ^ b); break; }
            
            
            case OpCode::OP_EQUAL: { int b = pop(); int a = pop(); push(a == b ? 1 : 0); break; }
            case OpCode::OP_LESS:  { int b = pop(); int a = pop(); push(a < b ? 1 : 0); break; }
            case OpCode::OP_GREATER: { int b = pop(); int a = pop(); push(a > b ? 1 : 0); break; }
            
            case OpCode::OP_JUMP: {
                uint8_t offset = readByte();
                ip += offset; 
                break;
            }
            case OpCode::OP_JUMP_IF_FALSE: {
                uint8_t offset = readByte();
                if (stack.back() == 0) ip += offset; 
                break;
            }
            case OpCode::OP_JUMP_IF_TRUE: {
                uint8_t offset = readByte();
                if (stack.back() != 0) ip += offset; 
                break;
            }
            case OpCode::OP_LOOP: {
                uint8_t offset = readByte();
                ip -= offset; 
                break;
            }

            case OpCode::OP_BUILD_ARRAY: {
                uint8_t count = readByte();
                std::vector<int> arr(count);
                
                
                for (int i = count - 1; i >= 0; i--) {
                    arr[i] = pop();
                }
                
                heap.push_back(arr); 
                push(heap.size() - 1); 
                break;
            }
            case OpCode::OP_INDEX_GET: {
                int index = pop();
                int arrayPtr = pop();
                push(heap[arrayPtr][index]); 
                break;
            }
            case OpCode::OP_INDEX_SET: {
                int value = pop();
                int index = pop();
                int arrayPtr = pop();
                heap[arrayPtr][index] = value; 
                push(value); 
                break;
            }
            case OpCode::OP_LEN: {
                int arrayPtr = pop();
                push(heap[arrayPtr].size());
                break;
            }

            case OpCode::OP_PRINT: { std::cout << pop() << std::endl; break; }
          case OpCode::OP_RETURN: {
                if (callStack.empty()) return; 
                
                int retVal = pop();       
                environments.pop_back();  
                ip = callStack.back();    
                callStack.pop_back();     
                push(retVal);             
                break;
            }
            default: std::cerr << "Unknown opcode.\n"; return;


        }
    }
}