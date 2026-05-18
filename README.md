# CVM — Custom Virtual Machine
A lightweight, custom virtual machine and scripting runtime written in C++. CVM interprets `.cvm` scripts through its own instruction set, providing a minimal and self-contained execution environment.

---

## Table of Contents
- [Overview](#overview)
- [Project Structure](#project-structure)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Building from Source](#building-from-source)
  - [Running the VM](#running-the-vm)
- [Usage](#usage)
- [Writing CVM Scripts](#writing-cvm-scripts)
- [Language Reference](#language-reference)
  - [Variables and Core I/O](#1-variables-and-core-io)
  - [Operators and Logic](#2-operators-and-logic)
  - [Control Flow](#3-control-flow)
  - [Functions and Scoping](#4-functions-and-scoping)
  - [Standard Library](#5-the-standard-library)
- [Contributing](#contributing)
- [License](#license)

---

## Overview
CVM is a custom virtual machine built in C++ that executes programs written in its own `.cvm` scripting format. The project is designed to explore low-level VM internals, including instruction dispatch, memory management, and runtime execution — all within a clean, portable C++ codebase.

CVM++ features dynamic typing handled natively under the hood, along with a fully custom bytecode instruction set architecture.

---

## Project Structure
```
CVM/
├── include/          # Header files and public API declarations
├── src/              # Core VM source files (lexer, parser, executor)
├── test.cvm          # Sample script demonstrating the CVM language
├── cvm.exe           # Pre-built Windows executable
└── README.md
```

---

## Getting Started

### Prerequisites
- A C++17-compatible compiler (GCC, Clang, or MSVC)
- Make or any standard C++ build system

### Building from Source
Clone the repository:
```bash
git clone https://github.com/Tanishq96sage/CVM-.git
cd CVM-
```

Compile with g++:
```bash
g++ -std=c++17 -Iinclude src/*.cpp -o cvm
```

Or with MSVC (Windows):
```bash
cl /std:c++17 /I include src\*.cpp /Fe:cvm.exe
```

### Running the VM
On Linux or macOS (after building):
```bash
./cvm test.cvm
```

On Windows using the pre-built binary:
```cmd
cvm.exe test.cvm
```

---

## Usage
```
cvm <script.cvm>
```

| Argument     | Description                       |
|--------------|-----------------------------------|
| `script.cvm` | Path to the CVM script to execute |

---

## Writing CVM Scripts
CVM scripts use the `.cvm` file extension. Refer to `test.cvm` in the root of the repository for a working example of the script syntax and supported operations.

```cvm
// Declare a variable
let score = 100;

// Print to terminal
print score;

// Read from terminal
let user_val = input;
```

---

## Language Reference

### 1. Variables and Core I/O

| Feature              | CVM++ Syntax                  | C++ Equivalent                          |
|----------------------|-------------------------------|-----------------------------------------|
| Variable Declaration | `let score = 100;`            | `int score = 100;`                      |
| Reassignment         | `score = 50;`                 | `score = 50;`                           |
| Standard Output      | `print score;`                | `std::cout << score << "\n";`           |
| Terminal Input       | `let user_val = input;`       | `int user_val; std::cin >> user_val;`   |
| Line Comments        | `// This is ignored`          | `// This is ignored`                    |

---

### 2. Operators and Logic

| Feature                  | CVM++ Syntax      | C++ Equivalent    |
|--------------------------|-------------------|-------------------|
| Basic Math               | `+, -, *, /`      | `+, -, *, /`      |
| Booleans                 | `true, false`     | `true, false`     |
| Comparisons              | `==, <, >`        | `==, <, >`        |
| Logical (Short-Circuit)  | `&&, \|\|`        | `&&, \|\|`        |
| Bitwise Operations       | `&, \|, ^`        | `&, \|, ^`        |

---

### 3. Control Flow

**If / Else**
```cvm
if (x < 5) {
    print 1;
} else {
    print 0;
}
```

**While Loop**
```cvm
while (x > 0) {
    x = x - 1;
}
```

**For Loop** *(desugared)*
```cvm
for (let i = 0; i < 5; i = i + 1) {
    print i;
}
```

| Feature           | CVM++ Syntax                              | C++ Equivalent                            |
|-------------------|-------------------------------------------|-------------------------------------------|
| If / Else         | `if (cond) { ... } else { ... }`          | `if (cond) { ... } else { ... }`          |
| While Loop        | `while (cond) { ... }`                    | `while (cond) { ... }`                    |
| For Loop          | `for (let i = 0; i < n; i = i + 1) { }` | `for (int i = 0; i < n; i++) { }`        |

---

### 4. Functions and Scoping

**Declaration and Call**
```cvm
fn add(a, b) {
    return a + b;
}

let result = add(10, 20);
print result;
```

| Feature       | CVM++ Syntax                        | C++ Equivalent                              |
|---------------|-------------------------------------|---------------------------------------------|
| Declaration   | `fn add(a, b) { return a + b; }`    | `int add(int a, int b) { return a + b; }`   |
| Call          | `let result = add(10, 20);`         | `int result = add(10, 20);`                 |
| Empty Return  | `return;`                           | `return;`                                   |

---

### 5. The Standard Library

| Feature          | CVM++ Syntax          | C++ Equivalent                          |
|------------------|-----------------------|-----------------------------------------|
| System Timing    | `let start = clock;`  | `auto start = std::chrono::...`         |
| Square Root      | `sqrt(100)`           | `std::sqrt(100)`                        |
| Absolute Value   | `abs(0 - 50)`         | `std::abs(0 - 50)`                      |
| Minimum          | `min(10, 20)`         | `std::min(10, 20)`                      |
| Maximum          | `max(10, 20)`         | `std::max(10, 20)`                      |
| Power/Exponents  | `pow(2, 8)`           | `std::pow(2, 8)`                        |

> **Note:** Because CVM++ currently uses integers on the stack, all math functions automatically round to the nearest whole number.

---

## Contributing
Contributions are welcome. To contribute:

1. Fork the repository.
2. Create a new branch: `git checkout -b feature/your-feature-name`
3. Commit your changes: `git commit -m "Add your feature"`
4. Push to the branch: `git push origin feature/your-feature-name`
5. Open a pull request.

Please ensure your code follows the existing style and compiles without warnings.

---

## License
This project does not currently specify a license. All rights are reserved by the author unless otherwise stated. Contact the repository owner for usage permissions.

---

*Built with C++ by [Tanishq96sage](https://github.com/Tanishq96sage)*
