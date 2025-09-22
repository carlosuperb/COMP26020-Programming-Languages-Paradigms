# COMP26020 Part 3 – Quadtrees in Haskell

## Overview
This project implements a **quadtree data structure** in Haskell to represent black-and-white bitmap images.  
The work corresponds to Exercises 1 and 2 of the COMP26020 Haskell lab.  

Main goals:
- Define an **Algebraic Data Type (ADT)** for quadtrees.
- Implement constructors for black/white images and quadrant composition.
- Implement a crude **blurring** operation based on neighbour majority.

---

## Files
- `Quadtrees.hs` – main implementation file.
- `ex1.hs` – test script for Exercise 1 (basic constructors).
- `ex2.hs` – test script for Exercise 2 (blur function and striped cases).

---

## Functions Implemented
### Exercise 1
- `allBlack :: Int -> Quadtree`  
- `allWhite :: Int -> Quadtree`  
- `clockwise :: Quadtree -> Quadtree -> Quadtree -> Quadtree -> Quadtree`  
- `anticlockwise :: Quadtree -> Quadtree -> Quadtree -> Quadtree -> Quadtree`  

### Exercise 2
- `blur :: Quadtree -> Quadtree`  
  - Flips the colour of a cell if more than half of its neighbours are of the opposite colour.  
  - Handles special cases for striped quadtrees.  

---

## How to Run
1. Load your implementation together with the tests:  
   ```bash
   ghci Quadtrees.hs ex1.hs
   mainS
   ```
   or
   ```bash
   ghci Quadtrees.hs ex2.hs
   main
   ```
2. Expected output: messages like
    ```makefile
    Joe: Simple check for ex 1 passed!
    Joe: Inequality check for ex 1 passed!
    Joe: Larger check for ex 1 passed!
    ...
    Joe: big striped check for ex2 passed!
    ...
    ```

---

## Notes
- The `size` argument for `allBlack` and `allWhite` is not used for logic, but included for consistency.
- `deriving (Eq, Show)` is added to ADTs so values can be compared and printed.
- The quadtree representation is not unique; rotations or reflections are acceptable if consistent.
