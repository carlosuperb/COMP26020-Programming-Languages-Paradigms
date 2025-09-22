# COMP26020 Part 4 – Register Allocation using Graph Colouring

## Overview
This project implements **register allocation using graph colouring** as part of COMP26020 – Part III (Compilers).  
The program applies a **top-down colouring heuristic** to assign registers (colours A–Z) to nodes in an interference graph.  

- Input: An interference graph described in a text file.  
- Output: A colouring assignment for each node, where each colour corresponds to a register.  
- Algorithm:  
  1. Parse the input into an undirected graph.  
  2. Rank nodes by degree (number of neighbours), breaking ties by node ID.  
  3. Assign the first available colour (A–Z) not used by neighbours.  
  4. Exit with an error if more than 26 colours are required.  

---

## Files
- `graph_coloring.py` – main implementation (parsing, colouring, writing output).  
- `README.md` – documentation.  

---

## Requirements
- Python **3.7+**  
- Standard libraries only (`sys`, `re`, `string`, `collections`).  

---

## Usage
Run the program from the command line:

```bash
python3 graph_coloring.py <input_file> <output_file>
```

Example:
```bash
python3 graph_coloring.py input.txt output.txt
```

---

## Input File Format
- Each line represents one node and its neighbours, separated by commas.
- Nodes are consecutive integers starting at 1.
- Example:
    ```makefile
    1,2,3,4
    2,4,1
    3,1
    4,1,2
    ```
- This means:
    - Node 1 interferes with nodes 2, 3, 4
    - Node 2 interferes with nodes 1, 4
    - Node 3 interferes with node 1
    - Node 4 interferes with nodes 1, 2
Invalid formats (non-integers, stray characters) will be rejected with an error message.

---

## Output File Format
- Each line contains a node number and its assigned colour.
- Example output for the input above:
```mathematica
1A
2B
3B
4C
```

---

## Error Handling
- Prints an error and exits if:
    - The input file is missing or invalid.
    - The graph requires more than 26 colours (registers).