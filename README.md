# COMP26020 Part 2 – Processor Emulator in C++

## Overview
This project implements a **processor emulator** in modern C++.  
The emulator simulates a simple processor with:
- One 8-bit general purpose register  
- One 8-bit Program Counter  
- 256-byte address space  
- Eight supported instruction types  

The assignment focuses on rewriting unsafe C-style code into modern C++ using:
- **RAII** and the “Rule of Zero”  
- Standard library containers and algorithms instead of raw arrays/pointers  
- Type safety improvements  
- Elimination of manual memory management (`new`/`delete`)  
- Clean, maintainable code that passes structural and functional tests  

---

## Project Structure
- `instructions.cpp/.h` – class hierarchy for processor instructions, their creation, string representation, and execution logic.  
- `emulator.cpp/.h` – core emulator, processor state management, instruction execution, breakpoints.  
- `common.h` – shared definitions for emulator and instructions.  

**Testing files (provided):**
- `catch.cpp`, `catch.hpp` – Catch2 testing framework.  
- `structural-tests.cpp` – verifies public interface and type correctness.  
- `functional-tests.cpp` – validates correct emulator behaviour.  

---

## Build & Run
The project uses **CMake** for building and testing.  
From the project root:

1. **Configure the build:**
   ```bash
   cmake -B build -DCMAKE_CXX_COMPILER=g++-12
   ```

2. **Build and run tests:**
    ```bash
    cmake --build build/ --target tests
    ```
    Expected outputs:
    - Structural tests: all assertions passed.
    - Functional tests: all assertions passed.

3. **Re-run tests after changes:**
    ```bash
    cmake --build build/ --target tests
    ```

---

## Additional Tools
- Clang-Tidy:
    ```bash
    cmake --build build/ --target tidy
    ```
- Address Sanitizer (ASan):
    ```bash
    cmake --build build/ --target sanitized
    ```
    Detects memory errors after running functional tests.

---

## Notes
- Only the following files should be modified:
`emulator.cpp/.h`, `instructions.cpp/.h`, `common.h`
- Do not remove or change existing public member functions unless explicitly allowed.
- The public interface must remain compatible with the structural and functional tests.
- Focus on eliminating unsafe C-style code while keeping behaviour unchanged.

---

## Learning Outcomes
This assignment develops skills in:
- Modern C++ programming practices
- Memory safety and resource management (RAII)
- Designing class hierarchies and clean APIs
- Using CMake, unit testing (Catch2), and static analysis tools