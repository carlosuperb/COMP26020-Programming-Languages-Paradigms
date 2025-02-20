# Register Allocation using Graph Coloring

## How to Run the Code

1. **Prerequisites:**
   - Python 3.12.0

2. **Run the Program:**
    python3 register.py input.txt output.txt

3. **Example input and output**
    - Example input:
    ```
    1,2,3,4
    2,4,1
    3,1
    4,1,2
    ```
    - Example output:
    ```
    1A
    2B
    3B
    4C
    ```

## Error Handling
- **Invalid Input Format:**  
Displays `Contains invalid input format.` if the input file has errors.

- **Exceeded Color Limit:**  
Displays `Error: registers are used out!` if more than 26 colors are needed.

## Author
- Name: Cong Wan
- ID: p55115cw