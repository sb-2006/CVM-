#pragma once
#include "opcode.h"
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

class Disassembler {
public:
    void print(const std::vector<uint8_t>& bytecode,
               const std::vector<int>& constants,
               const std::vector<std::string>& stringConstants)
    {
        std::cout << "\n=== BYTECODE DISASSEMBLY ===\n";
        size_t i = 0;
        while (i < bytecode.size()) {
            // Print address
            std::cout << std::setw(4) << std::setfill('0') << i << "  ";

            OpCode op = static_cast<OpCode>(bytecode[i]);
            switch (op) {

            case OpCode::OP_CONSTANT: {
                uint8_t idx = bytecode[++i];
                int val = (idx < constants.size()) ? constants[idx] : -1;
                std::cout << "OP_CONSTANT      #" << (int)idx << "  (" << val << ")\n";
                break;
            }
            case OpCode::OP_TRUE:
                std::cout << "OP_TRUE\n"; break;
            case OpCode::OP_FALSE:
                std::cout << "OP_FALSE\n"; break;
            case OpCode::OP_POP:
                std::cout << "OP_POP\n"; break;

            case OpCode::OP_ADD:
                std::cout << "OP_ADD\n"; break;
            case OpCode::OP_SUBTRACT:
                std::cout << "OP_SUBTRACT\n"; break;
            case OpCode::OP_MULTIPLY:
                std::cout << "OP_MULTIPLY\n"; break;
            case OpCode::OP_DIVIDE:
                std::cout << "OP_DIVIDE\n"; break;
            case OpCode::OP_EQUAL:
                std::cout << "OP_EQUAL\n"; break;
            case OpCode::OP_LESS:
                std::cout << "OP_LESS\n"; break;
            case OpCode::OP_GREATER:
                std::cout << "OP_GREATER\n"; break;

            case OpCode::OP_BIT_AND:
                std::cout << "OP_BIT_AND\n"; break;
            case OpCode::OP_BIT_OR:
                std::cout << "OP_BIT_OR\n"; break;
            case OpCode::OP_BIT_XOR:
                std::cout << "OP_BIT_XOR\n"; break;

            case OpCode::OP_DEFINE_GLOBAL: {
                uint8_t idx = bytecode[++i];
                std::string name = (idx < stringConstants.size()) ? stringConstants[idx] : "?";
                std::cout << "OP_DEFINE_GLOBAL #" << (int)idx << "  (\"" << name << "\")\n";
                break;
            }
            case OpCode::OP_GET_GLOBAL: {
                uint8_t idx = bytecode[++i];
                std::string name = (idx < stringConstants.size()) ? stringConstants[idx] : "?";
                std::cout << "OP_GET_GLOBAL    #" << (int)idx << "  (\"" << name << "\")\n";
                break;
            }
            case OpCode::OP_SET_GLOBAL: {
                uint8_t idx = bytecode[++i];
                std::string name = (idx < stringConstants.size()) ? stringConstants[idx] : "?";
                std::cout << "OP_SET_GLOBAL    #" << (int)idx << "  (\"" << name << "\")\n";
                break;
            }

            case OpCode::OP_JUMP: {
                uint8_t offset = bytecode[++i];
                std::cout << "OP_JUMP          +" << (int)offset << "  -> " << (i + offset) << "\n";
                break;
            }
            case OpCode::OP_JUMP_IF_FALSE: {
                uint8_t offset = bytecode[++i];
                std::cout << "OP_JUMP_IF_FALSE +" << (int)offset << "  -> " << (i + offset) << "\n";
                break;
            }
            case OpCode::OP_JUMP_IF_TRUE: {
                uint8_t offset = bytecode[++i];
                std::cout << "OP_JUMP_IF_TRUE  +" << (int)offset << "  -> " << (i + offset) << "\n";
                break;
            }
            case OpCode::OP_LOOP: {
                uint8_t offset = bytecode[++i];
                std::cout << "OP_LOOP          -" << (int)offset << "  -> " << (i - offset) << "\n";
                break;
            }

            case OpCode::OP_PRINT:
                std::cout << "OP_PRINT\n"; break;
            case OpCode::OP_INPUT:
                std::cout << "OP_INPUT\n"; break;
            case OpCode::OP_CLOCK:
                std::cout << "OP_CLOCK\n"; break;

            case OpCode::OP_CALL: {
                uint8_t idx = bytecode[++i];
                std::string name = (idx < stringConstants.size()) ? stringConstants[idx] : "?";
                std::cout << "OP_CALL          #" << (int)idx << "  (\"" << name << "\")\n";
                break;
            }
            case OpCode::OP_RETURN:
                std::cout << "OP_RETURN\n"; break;

            case OpCode::OP_SQRT:
                std::cout << "OP_SQRT\n"; break;
            case OpCode::OP_ABS:
                std::cout << "OP_ABS\n"; break;
            case OpCode::OP_MIN:
                std::cout << "OP_MIN\n"; break;
            case OpCode::OP_MAX:
                std::cout << "OP_MAX\n"; break;
            case OpCode::OP_POW:
                std::cout << "OP_POW\n"; break;

            case OpCode::OP_BUILD_ARRAY: {
                uint8_t count = bytecode[++i];
                std::cout << "OP_BUILD_ARRAY   count=" << (int)count << "\n";
                break;
            }
            case OpCode::OP_INDEX_GET:
                std::cout << "OP_INDEX_GET\n"; break;
            case OpCode::OP_INDEX_SET:
                std::cout << "OP_INDEX_SET\n"; break;
            case OpCode::OP_LEN:
                std::cout << "OP_LEN\n"; break;

            default:
                std::cout << "UNKNOWN(" << (int)bytecode[i] << ")\n"; break;
            }
            i++;
        }
        std::cout << "============================\n\n";
    }
};
