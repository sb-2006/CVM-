#pragma once
#include <cstdint>

enum class OpCode : uint8_t {
    
    OP_CONSTANT,      
    OP_TRUE,          
    OP_FALSE,         
    OP_POP,           

    
    OP_ADD,           
    OP_SUBTRACT,      
    OP_MULTIPLY,      
    OP_DIVIDE,        
    OP_EQUAL,         
    OP_LESS,          
    OP_GREATER,

    
    OP_DEFINE_GLOBAL, 
    OP_GET_GLOBAL,    
    OP_SET_GLOBAL,    

    
    OP_JUMP,          
    OP_JUMP_IF_FALSE, 
    OP_JUMP_IF_TRUE,
    OP_LOOP,          

    
    OP_PRINT,         
    OP_INPUT,        
    OP_CLOCK, 
    OP_CALL,

    OP_BIT_AND,       
    OP_BIT_OR,        
    OP_BIT_XOR,

    OP_SQRT, 
    OP_ABS, 
    OP_MIN, 
    OP_MAX, 
    OP_POW,

    OP_BUILD_ARRAY,
    OP_INDEX_GET,
    OP_INDEX_SET,
    OP_LEN,

    
    OP_RETURN         
};