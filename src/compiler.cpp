#include "compiler.h"
#include <string>

Compiler::Compiler() {}

void Compiler::compile(const std::vector<std::unique_ptr<Stmt>> &statements)
{
  for (const auto &stmt : statements)
  {
    compileStatement(stmt.get());
  }
  emitByte(static_cast<uint8_t>(OpCode::OP_RETURN));
}

void Compiler::compileStatement(Stmt *stmt)
{
  if (auto *printStmt = dynamic_cast<PrintStmt *>(stmt))
  {
    compileExpression(printStmt->expression.get());
    emitByte(static_cast<uint8_t>(OpCode::OP_PRINT));
  }
  else if (auto *varDecl = dynamic_cast<VarDeclStmt *>(stmt))
  {
    compileExpression(varDecl->initializer.get());
    int nameIndex = addStringConstant(varDecl->name.lexeme);
    emitByte(static_cast<uint8_t>(OpCode::OP_DEFINE_GLOBAL));
    emitByte(static_cast<uint8_t>(nameIndex));
  }
  else if (auto *exprStmt = dynamic_cast<ExprStmt *>(stmt))
  {
    compileExpression(exprStmt->expression.get());
    emitByte(static_cast<uint8_t>(OpCode::OP_POP));
  }
  
  else if (auto *blockStmt = dynamic_cast<BlockStmt *>(stmt))
  {
    for (const auto &s : blockStmt->statements)
    {
      compileStatement(s.get());
    }
  }
  else if (auto *ifStmt = dynamic_cast<IfStmt *>(stmt))
  {
    compileExpression(ifStmt->condition.get());

    int thenJump = emitJump(static_cast<uint8_t>(OpCode::OP_JUMP_IF_FALSE));
    emitByte(static_cast<uint8_t>(OpCode::OP_POP)); 

    compileStatement(ifStmt->thenBranch.get());

    int elseJump = emitJump(static_cast<uint8_t>(OpCode::OP_JUMP));
    patchJump(thenJump);
    emitByte(static_cast<uint8_t>(OpCode::OP_POP)); 

    if (ifStmt->elseBranch != nullptr)
    {
      compileStatement(ifStmt->elseBranch.get());
    }
    patchJump(elseJump);
  }
  else if (auto *funcDecl = dynamic_cast<FunctionDeclStmt *>(stmt))
  {
    
    int skipJump = emitJump(static_cast<uint8_t>(OpCode::OP_JUMP));

    
    FunctionInfo info;
    for (const Token &param : funcDecl->parameters)
    {
      info.parameters.push_back(param.lexeme);
    }
    info.ipStart = bytecode.size();
    functions[funcDecl->name.lexeme] = info;

    
    for (const auto &bodyStmt : funcDecl->body)
    {
      compileStatement(bodyStmt.get());
    }

    
    emitByte(static_cast<uint8_t>(OpCode::OP_CONSTANT));
    emitByte(static_cast<uint8_t>(addConstant(0)));
    emitByte(static_cast<uint8_t>(OpCode::OP_RETURN));

    
    patchJump(skipJump);
  }
  else if (auto *retStmt = dynamic_cast<ReturnStmt *>(stmt))
  {
    if (retStmt->value != nullptr)
    {
      compileExpression(retStmt->value.get());
    }
    else
    {
      emitByte(static_cast<uint8_t>(OpCode::OP_CONSTANT));
      emitByte(static_cast<uint8_t>(addConstant(0)));
    }
    emitByte(static_cast<uint8_t>(OpCode::OP_RETURN));
  }
  else if (auto *whileStmt = dynamic_cast<WhileStmt *>(stmt))
  {
    int loopStart = bytecode.size();

    compileExpression(whileStmt->condition.get());
    int exitJump = emitJump(static_cast<uint8_t>(OpCode::OP_JUMP_IF_FALSE));
    emitByte(static_cast<uint8_t>(OpCode::OP_POP)); 

    compileStatement(whileStmt->body.get());
    emitLoop(loopStart);

    patchJump(exitJump);
    emitByte(static_cast<uint8_t>(OpCode::OP_POP)); 
  }
}

void Compiler::compileExpression(Expr *expr)
{
  if (auto *literal = dynamic_cast<LiteralExpr *>(expr))
  {
    if (literal->value.type == TokenType::NUMBER)
    {
      int val = std::stoi(literal->value.lexeme);
      int index = addConstant(val);
      emitByte(static_cast<uint8_t>(OpCode::OP_CONSTANT));
      emitByte(static_cast<uint8_t>(index));
    }
    else if (literal->value.type == TokenType::TRUE_KW)
    {
      emitByte(static_cast<uint8_t>(OpCode::OP_TRUE));
    }
    else if (literal->value.type == TokenType::FALSE_KW)
    {
      emitByte(static_cast<uint8_t>(OpCode::OP_FALSE));
    }
  }
  else if (auto *grouping = dynamic_cast<GroupingExpr *>(expr))
  {
    compileExpression(grouping->expression.get());
  }
  else if (auto *binary = dynamic_cast<BinaryExpr *>(expr))
  {
    compileExpression(binary->left.get());
    compileExpression(binary->right.get());

    switch (binary->op.type)
    {
    case TokenType::PLUS:
      emitByte(static_cast<uint8_t>(OpCode::OP_ADD));
      break;
    case TokenType::MINUS:
      emitByte(static_cast<uint8_t>(OpCode::OP_SUBTRACT));
      break;
    case TokenType::STAR:
      emitByte(static_cast<uint8_t>(OpCode::OP_MULTIPLY));
      break;
    case TokenType::SLASH:
      emitByte(static_cast<uint8_t>(OpCode::OP_DIVIDE));
      break;
    
    case TokenType::EQUAL_EQUAL:
      emitByte(static_cast<uint8_t>(OpCode::OP_EQUAL));
      break;
    case TokenType::LESS:
      emitByte(static_cast<uint8_t>(OpCode::OP_LESS));
      break;
    case TokenType::GREATER:
      emitByte(static_cast<uint8_t>(OpCode::OP_GREATER));
      break;
    case TokenType::AMPERSAND:
      emitByte(static_cast<uint8_t>(OpCode::OP_BIT_AND));
      break;
    case TokenType::PIPE:
      emitByte(static_cast<uint8_t>(OpCode::OP_BIT_OR));
      break;
    case TokenType::CARET:
      emitByte(static_cast<uint8_t>(OpCode::OP_BIT_XOR));
      break;
    default:
      break;
    }
  }
  else if (auto *varExpr = dynamic_cast<VariableExpr *>(expr))
  {
    int nameIndex = addStringConstant(varExpr->name.lexeme);
    emitByte(static_cast<uint8_t>(OpCode::OP_GET_GLOBAL));
    emitByte(static_cast<uint8_t>(nameIndex));
  }
  else if (auto *assignExpr = dynamic_cast<AssignExpr *>(expr))
  {
    compileExpression(assignExpr->value.get());
    int nameIndex = addStringConstant(assignExpr->name.lexeme);
    emitByte(static_cast<uint8_t>(OpCode::OP_SET_GLOBAL));
    emitByte(static_cast<uint8_t>(nameIndex));
  }
  else if (auto *inputExpr = dynamic_cast<InputExpr *>(expr))
  {
    emitByte(static_cast<uint8_t>(OpCode::OP_INPUT));
  }
  else if (auto *clockExpr = dynamic_cast<ClockExpr *>(expr))
  {
    emitByte(static_cast<uint8_t>(OpCode::OP_CLOCK));
  }
  else if (auto *unary = dynamic_cast<BuiltinUnaryExpr *>(expr))
  {
    compileExpression(unary->argument.get()); 
    switch (unary->functionName.type)
    {
    case TokenType::SQRT:
      emitByte(static_cast<uint8_t>(OpCode::OP_SQRT));
      break;
    case TokenType::ABS:
      emitByte(static_cast<uint8_t>(OpCode::OP_ABS));
      break;
    default:
      break;
    }
  }
  else if (auto *binary = dynamic_cast<BuiltinBinaryExpr *>(expr))
  {
    compileExpression(binary->left.get());  
    compileExpression(binary->right.get()); 
    switch (binary->functionName.type)
    {
    case TokenType::MIN:
      emitByte(static_cast<uint8_t>(OpCode::OP_MIN));
      break;
    case TokenType::MAX:
      emitByte(static_cast<uint8_t>(OpCode::OP_MAX));
      break;
    case TokenType::POW:
      emitByte(static_cast<uint8_t>(OpCode::OP_POW));
      break;
    default:
      break;
    }
  }
  else if (auto *logical = dynamic_cast<LogicalExpr *>(expr))
  {
    compileExpression(logical->left.get());

    if (logical->op.type == TokenType::AND_AND)
    {
      
      int endJump = emitJump(static_cast<uint8_t>(OpCode::OP_JUMP_IF_FALSE));
      emitByte(static_cast<uint8_t>(
          OpCode::OP_POP)); 
      compileExpression(logical->right.get());
      patchJump(endJump);
    }
    else if (logical->op.type == TokenType::OR_OR)
    {
      
      int endJump = emitJump(static_cast<uint8_t>(OpCode::OP_JUMP_IF_TRUE));
      emitByte(static_cast<uint8_t>(
          OpCode::OP_POP)); 
      compileExpression(logical->right.get());
      patchJump(endJump);
    }
  }

  else if (auto *arrayExpr = dynamic_cast<ArrayExpr *>(expr))
  {
    for (const auto &element : arrayExpr->elements)
    {
      compileExpression(element.get());
    }
    emitByte(static_cast<uint8_t>(OpCode::OP_BUILD_ARRAY));
    emitByte(static_cast<uint8_t>(
        arrayExpr->elements.size())); 
  }
  else if (auto *getExpr = dynamic_cast<IndexGetExpr *>(expr))
  {
    compileExpression(getExpr->target.get());
    compileExpression(getExpr->index.get());
    emitByte(static_cast<uint8_t>(OpCode::OP_INDEX_GET));
  }
  else if (auto *callExpr = dynamic_cast<CallExpr *>(expr))
  {
    
    for (const auto &arg : callExpr->arguments)
    {
      compileExpression(arg.get());
    }
    
    int nameIndex = addStringConstant(callExpr->calleeName.lexeme);
    emitByte(static_cast<uint8_t>(OpCode::OP_CALL));
    emitByte(static_cast<uint8_t>(nameIndex));
  }
  else if (auto *setExpr = dynamic_cast<IndexSetExpr *>(expr))
  {
    compileExpression(setExpr->target.get()); 
    compileExpression(setExpr->index.get());  
    compileExpression(setExpr->value.get());  
    emitByte(static_cast<uint8_t>(OpCode::OP_INDEX_SET));
  }
}

void Compiler::emitByte(uint8_t byte) { bytecode.push_back(byte); }
int Compiler::addConstant(int value)
{
  constants.push_back(value);
  return constants.size() - 1;
}
int Compiler::addStringConstant(const std::string &name)
{
  stringConstants.push_back(name);
  return stringConstants.size() - 1;
}


int Compiler::emitJump(uint8_t instruction)
{
  emitByte(instruction);
  emitByte(0xff); 
  return bytecode.size() - 1;
}

void Compiler::patchJump(int offset)
{
  int jump = bytecode.size() - 1 - offset;
  bytecode[offset] = static_cast<uint8_t>(jump);
}

void Compiler::emitLoop(int loopStart)
{
  emitByte(static_cast<uint8_t>(OpCode::OP_LOOP));
  int offset = bytecode.size() - loopStart + 1;
  emitByte(static_cast<uint8_t>(offset));
}

std::vector<uint8_t> Compiler::getBytecode() const { return bytecode; }
std::vector<int> Compiler::getConstants() const { return constants; }
std::vector<std::string> Compiler::getStringConstants() const
{
  return stringConstants;
}
std::unordered_map<std::string, FunctionInfo> Compiler::getFunctions() const
{
  return functions;
}