# CVM++ — Custom Virtual Machine & Scripting Language

> A lightweight compiler + stack-based virtual machine written in C++.  
> Source code compiles to proprietary bytecode, which is then executed by the CVM++ runtime.

---

## Table of Contents

1. [Project Overview](#project-overview)
2. [Architecture: How It Works](#architecture-how-it-works)
3. [Project Structure](#project-structure)
4. [Prerequisites](#prerequisites)
5. [Building from Source](#building-from-source)
6. [Running the VM](#running-the-vm)
7. [Debug / Inspection Flags](#debug--inspection-flags)
8. [Language Reference](#language-reference)
9. [Sample Programs](#sample-programs)
10. [Demo Video Script](#demo-video-script)
11. [Contributors](#contributors)

---

## Project Overview

Most developers use high-level languages like Python, JavaScript, or Java without deeply understanding how raw text is translated into instructions a computer can actually execute.

**CVM++** demystifies this process by building a full compiler pipeline from scratch:

```
.cvm source code
      │
      ▼
  [ LEXER ]        → tokenises raw text into NUMBER, PLUS, IDENTIFIER, etc.
      │
      ▼
  [ PARSER ]       → arranges tokens into an Abstract Syntax Tree (AST)
      │
      ▼
  [ COMPILER ]     → flattens the AST into a Bytecode instruction array (Opcodes)
      │
      ▼
  [ VIRTUAL MACHINE ] → executes bytecode via a stack-based dispatch loop
      │
      ▼
   OUTPUT / RESULT
```

Everything — the language grammar, the instruction set architecture (ISA), and the runtime — is built entirely in C++.

---

## Architecture: How It Works

### 1. Lexer (Tokenisation)
The Lexer reads raw `.cvm` source text character-by-character and produces a flat list of **Tokens** such as:

| Token Type  | Example Source | Token Value |
|-------------|---------------|-------------|
| `NUMBER`    | `42`          | `42`        |
| `IDENTIFIER`| `score`       | `score`     |
| `PLUS`      | `+`           | `+`         |
| `LET`       | `let`         | `let`       |
| `IF`        | `if`          | `if`        |

### 2. Parser → AST
The Parser applies a **recursive descent** strategy to arrange tokens into a tree structure. For example, `let x = 3 + 4;` produces:

```
AssignNode
├── Identifier: x
└── BinaryOpNode (+)
    ├── NumberNode: 3
    └── NumberNode: 4
```

### 3. Compiler → Bytecode
The Compiler does a depth-first walk of the AST and emits **Opcodes** — raw integer instructions stored in a `std::vector<uint8_t>`. For example:

```
PUSH 3
PUSH 4
ADD
STORE x
```

### 4. Virtual Machine
The VM is a **stack-based execution engine**. It maintains:
- An **operand stack** (`std::vector`) for intermediate values
- A **variable table** (symbol map) for storing named values
- An **instruction pointer** that advances through the bytecode array

---

## Project Structure

```
CVM++/
├── include/
│   ├── lexer.h          # Token types and Lexer class
│   ├── parser.h         # AST node types and Parser class
│   ├── compiler.h       # Bytecode compiler
│   └── vm.h             # Virtual Machine and Opcode definitions
├── src/
│   ├── lexer.cpp
│   ├── parser.cpp
│   ├── compiler.cpp
│   ├── vm.cpp
│   └── main.cpp         # Entry point — wires Lexer → Parser → Compiler → VM
├── samples/
│   ├── calculator.cvm   # Interactive calculator demo
│   ├── truth_machine.cvm
│   ├── fibonacci.cvm
│   └── test.cvm
├── CVM++_Project_Report.pdf
├── cvm.exe              # Pre-built Windows binary
└── README.md
```

---

## Prerequisites

| Requirement | Version |
|-------------|---------|
| C++ Compiler (GCC / Clang / MSVC) | C++17 or later |
| Make (optional) | Any version |
| Windows (pre-built binary) | Windows 10/11 x64 |

No external libraries are required. CVM++ uses only the C++17 standard library.

---

## Building from Source

### Linux / macOS

```bash
# Clone the repository
git clone https://github.com/sb-2006/CVM-.git
cd CVM-

# Compile all source files
g++ -std=c++17 -Iinclude src/*.cpp -o cvm

# Verify the build
./cvm --version
```

### Windows (MSVC)

```cmd
cl /std:c++17 /I include src\*.cpp /Fe:cvm.exe
```

### Windows (MinGW / Git Bash)

```bash
g++ -std=c++17 -Iinclude src/*.cpp -o cvm.exe
```

> **Quick start (Windows):** A pre-built `cvm.exe` is included in the repo root. Skip compilation and jump straight to [Running the VM](#running-the-vm).

---

## Running the VM

The `cvm` executable takes a `.cvm` script file as its argument and runs it end-to-end through the full pipeline.

```bash
# Basic usage
./cvm <script.cvm>

# Example
./cvm samples/calculator.cvm
```

### What happens internally when you run `./cvm script.cvm`:

```
Step 1:  Lexer     reads script.cvm → produces Token list
Step 2:  Parser    reads Tokens     → produces AST
Step 3:  Compiler  walks AST        → produces Bytecode (uint8_t array)
Step 4:  VM        executes Bytecode → prints output to terminal
```

---

## Debug / Inspection Flags

CVM++ supports optional flags to inspect intermediate stages of compilation — exactly as the PS requires.

```bash
# Show the generated AST after parsing
./cvm script.cvm --ast

# Show compiled bytecode (opcode listing) before execution
./cvm script.cvm --bytecode

# Enable both AST + bytecode output, then run
./cvm script.cvm --ast --bytecode
```

### Example output — `./cvm samples/calculator.cvm --bytecode`

```
=== BYTECODE DISASSEMBLY ===
0000  PUSH    0
0002  STORE   result
0004  READ            ; input keyword → reads from stdin
0005  STORE   a
0007  READ
0008  STORE   b
0010  LOAD    a
0012  LOAD    b
0014  ADD
0015  STORE   result
0017  PRINT   result
0019  HALT
============================
```

---

## Language Reference

### Variables and I/O

```cvm
let x = 10;
let name = input;   // reads from terminal
print x;
print name;
```

### Arithmetic Operators

```cvm
let sum = a + b;
let diff = a - b;
let prod = a * b;
let quot = a / b;
```

### Comparison & Logic

```cvm
if (x == 10) { print x; }
if (x < 5)   { print 0; }
let flag = true;
let result = flag && false;
```

### Control Flow

```cvm
// if / else
if (x < 5) {
    print 1;
} else {
    print 0;
}

// while loop
while (x > 0) {
    x = x - 1;
    print x;
}

// for loop
for (let i = 0; i < 5; i = i + 1) {
    print i;
}
```

### Functions

```cvm
fn add(a, b) {
    return a + b;
}

let result = add(10, 20);
print result;
```

### Standard Library

| Function   | Usage          | Equivalent         |
|------------|----------------|--------------------|
| `sqrt(x)`  | `sqrt(144)`    | `std::sqrt(144)`   |
| `pow(x,y)` | `pow(2, 8)`    | `std::pow(2, 8)`   |
| `abs(x)`   | `abs(0 - 50)`  | `std::abs(-50)`    |
| `min(x,y)` | `min(3, 7)`    | `std::min(3, 7)`   |
| `max(x,y)` | `max(3, 7)`    | `std::max(3, 7)`   |
| `clock`    | `let t = clock;` | `std::chrono::...` |

---

## Sample Programs

### 1. Calculator (`samples/calculator.cvm`)

```cvm
// Interactive 4-operation calculator
print 0;   // prompt: enter first number
let a = input;

print 0;   // prompt: enter second number
let b = input;

let sum  = a + b;
let diff = a - b;
let prod = a * b;
let quot = a / b;

print sum;
print diff;
print prod;
print quot;
```

**Run it:**
```bash
./cvm samples/calculator.cvm
```

---

### 2. Truth Machine (`samples/truth_machine.cvm`)

> A classic CS demo: reads 0 or 1 — if 0, prints 0 once and halts; if 1, prints 1 forever.

```cvm
let x = input;

if (x == 0) {
    print 0;
}

while (x == 1) {
    print 1;
}
```

**Run it:**
```bash
./cvm samples/truth_machine.cvm
```

---

### 3. Fibonacci (`samples/fibonacci.cvm`)

```cvm
let n  = input;   // how many terms to print
let a  = 0;
let b  = 1;
let i  = 0;

while (i < n) {
    print a;
    let temp = a + b;
    a = b;
    b = temp;
    i = i + 1;
}
```

**Run it:**
```bash
./cvm samples/fibonacci.cvm
# Input: 10
# Output: 0 1 1 2 3 5 8 13 21 34
```

---

## Demo Video Script

> **What the mentor asked for:** compile a sample program to bytecode, run it on the VM, and record the demo.

Suggested sequence for the demo video:

```
1. Open terminal in CVM++ project directory.

2. Show the source file:
   cat samples/calculator.cvm

3. Run with --ast flag to show the generated AST:
   ./cvm samples/calculator.cvm --ast

4. Run with --bytecode flag to show compiled bytecode:
   ./cvm samples/calculator.cvm --bytecode

5. Run normally — enter two numbers and show the output:
   ./cvm samples/calculator.cvm
   > Input: 15
   > Input: 7
   > Output: 22  (sum)
   > Output: 8   (diff)
   > Output: 105 (product)
   > Output: 2   (quotient)

6. Repeat for truth_machine.cvm to show looping behaviour.
```

---

## Contributors

| Name | GitHub | Role |
|------|--------|------|
| Sneha Bindal | [@sb-2006](https://github.com/sb-2006)
