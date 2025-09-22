# COMP26020 Part 1 – Matrix Processing Library in C

## Overview
This project implements a **matrix processing library** in C, providing functions for matrix allocation, initialization, operations, and file I/O.  
The assignment is part of **COMP26020 Programming Languages & Paradigms**.  

The library is defined by the provided `matrix.h` header and fully implemented in `matrix.c`.  
A test suite using the Unity framework (`basic-testsuite.c`) validates correctness and robustness.  

---

## Features
The library supports 13 core functions grouped into three categories:

1. **Allocation / Initialisation / Destruction**
   - `matrix_allocate` / `matrix_free`  
   - `matrix_init_rand`, `matrix_init_n`, `matrix_init_zeros`, `matrix_init_identity`

2. **Matrix Operations**
   - `matrix_equal`  
   - `matrix_sum`  
   - `matrix_scalar_product`  
   - `matrix_transposition`  
   - `matrix_product`

3. **File I/O**
   - `matrix_dump_file`  
   - `matrix_allocate_and_init_file`

Extra helper functions (e.g. `print_matrix`) may also be included for debugging.

---

## File Format
Matrices are stored in **text-based files**, where:
- Each line represents a row.  
- Numbers are separated by spaces.  
- Extra spaces or empty lines are tolerated.  

Example (3×3 identity matrix):
```text
1 0 0
0 1 0
0 0 1
```

## Compilation & Running Tests
1. **Compile the library and test suite**:
    ```bash
    gcc -Wall -Werror -std=c11 -o basic-testsuite \
       matrix.c basic-testsuite.c unity.c
    ```

2. **Run tests**:
    ```bash
    ./basic-testsuite
    ```
    You should see Unity’s test output, indicating passed/failed test cases.

## Error Handling & Robustness
- The implementation checks for:
- Invalid dimensions (negative or zero sizes).
- Null pointers when freeing or accessing matrices.
- Malformed input files (non-integers, inconsistent row sizes).
- In these cases, functions return error codes or NULL instead of crashing.