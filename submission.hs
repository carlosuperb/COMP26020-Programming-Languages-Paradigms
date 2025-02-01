-- Exercise 1
-- | Represents the two possible colors(Black or White) of a cell.
data Color = Black | White deriving (Eq, Show)

-- | Represents a cell in the quadtree.
-- A cell can be either:
--   * A 'Unit' which stores a color and its size.
--   * A 'Block' which contains four smaller cells.
data Cell = Unit Color Int | Block Cell Cell Cell Cell deriving (Eq, Show)

-- | Computes the size of a cell.
--   * If the cell is a 'Unit', it returns its stored size.
--   * If the cell is a 'Block', its size is twice the size of any of its sub-cells.
size :: Cell -> Int
size (Unit _ n) = n
size (Block a b c d) = (size a) * 2

-- | Creates a 'Unit' cell filled with black of a given size.
allBlack :: Int -> Cell
allBlack n = Unit Black n

-- | Creates a 'Unit' cell filled with white of a given size.
allWhite :: Int -> Cell
allWhite n = Unit White n

-- | Combines four cells into a 'Block' in clockwise order.
clockwise :: Cell -> Cell -> Cell -> Cell -> Cell
clockwise a b c d = Block a b c d

-- | Combines four cells into a 'Block' in anticlockwise order.
anticlockwise :: Cell -> Cell -> Cell -> Cell -> Cell
anticlockwise a b c d = Block a d c b


-- Exercise 2
-- | Takes a quadtree as input and returns a blurred quadtree as output.
--   The structure of the quadtree remains unchanged, but colors may change.
blur :: Cell -> Cell
blur (Unit color size) = Unit color size  -- If the cell is a unit, its color remains unchanged.
blur block = recursiveBlur block block 0 0 (size block)  -- Otherwise, recursively blur the quadtree.

-- | Recursively applies the operation to a given quadtree.
--   The function modifies the color of each cell based on its neighbors.
--
--   * 'quadtree' - The original quadtree, used to find neighbors.
--   * 'currentCell' - The current cell being processed.
--   * 'targetX' and 'targetY' - The position of the current cell in the original structure.
--   * 'targetSize' - The size of the current cell.
recursiveBlur :: Cell -> Cell -> Int -> Int -> Int -> Cell
-- Base case: If the cell is a unit, determine its new color based on its neighbors.
recursiveBlur quadtree (Unit color size) targetX targetY targetSize
    | color == Black && (black * 2 < total) = Unit White size  -- If more than half neighbors are white, change to white.
    | color == White && (black * 2 > total) = Unit Black size  -- If more than half neighbors are black, change to black.
    | otherwise = Unit color size  -- Otherwise, keep the color unchanged.
  where
    (black, total) = countNeighbors quadtree targetX targetY targetSize startX startY
    startX = 0
    startY = 0
-- Recursive case: If the cell is a block, apply recursiveBlur to each sub-block.
recursiveBlur quadtree (Block tl tr br bl) targetX targetY targetSize
    | subBlockSize * 2 /= targetSize = error "Invalid quadtree structure"
    | otherwise = clockwise blurredTL blurredTR blurredBR blurredBL
  where
    subBlockSize = size tl
    blurredTL = recursiveBlur quadtree tl targetX targetY subBlockSize
    blurredTR = recursiveBlur quadtree tr (targetX + subBlockSize) targetY subBlockSize
    blurredBR = recursiveBlur quadtree br (targetX + subBlockSize) (targetY + subBlockSize) subBlockSize
    blurredBL = recursiveBlur quadtree bl targetX (targetY + subBlockSize) subBlockSize

-- | Counts the number of neighboring black and white cells for a given cell.
--
--   * 'quadtree' - The original quadtree being processed.
--   * 'targetX' and 'targetY' - The position of the target cell.
--   * 'targetSize' - The size of the target cell.
--   * 'cellX' and 'cellY' - The position of the current cell in the quadtree.
--   Returns a tuple (number of black neighbors, total number of neighbors).
countNeighbors :: Cell -> Int -> Int -> Int -> Int -> Int -> (Int, Int)
-- If the cell is a black unit, check if it is a neighbor.
countNeighbors (Unit Black cellSize) targetX targetY targetSize cellX cellY
    | isNeighbor targetX targetY targetSize cellX cellY cellSize = (1, 1)
    | otherwise = (0, 0)
-- If the cell is a white unit, check if it is a neighbor.
countNeighbors (Unit White cellSize) targetX targetY targetSize cellX cellY
    | isNeighbor targetX targetY targetSize cellX cellY cellSize = (0, 1)
    | otherwise = (0, 0)
-- If the cell is a block, recursively count neighbors in each sub-block.
countNeighbors (Block topLeft topRight bottomRight bottomLeft) targetX targetY targetSize cellX cellY
    | otherwise =
        let subBlockSize = size topLeft
            (tlBlack, tlTotal) = countNeighbors topLeft targetX targetY targetSize cellX cellY
            (trBlack, trTotal) = countNeighbors topRight targetX targetY targetSize (cellX + subBlockSize) cellY
            (brBlack, brTotal) = countNeighbors bottomRight targetX targetY targetSize (cellX + subBlockSize) (cellY + subBlockSize)
            (blBlack, blTotal) = countNeighbors bottomLeft targetX targetY targetSize cellX (cellY + subBlockSize)
        in (tlBlack + trBlack + brBlack + blBlack, tlTotal + trTotal + brTotal + blTotal)

-- | Determines whether two cells are neighbors based on their positions and sizes.
--
--   * Cells are considered neighbors if they share an edge (but not just a corner).
--   * This function checks whether two cells touch along the X or Y axis.
isNeighbor :: Int -> Int -> Int -> Int -> Int -> Int -> Bool
isNeighbor cell1X cell1Y cell1Size cell2X cell2Y cell2Size
    | cell1X == cell2X && cell1Y == cell2Y && cell1Size == cell2Size = False  -- Identical cells are not neighbors.
    | (cell1X == cell2Right || cell2X == cell1Right) && (cell2Y < cell1Bottom && cell2Bottom > cell1Y) = True  -- Horizontally adjacent.
    | (cell1Y == cell2Bottom || cell2Y == cell1Bottom) && (cell2X < cell1Right && cell2Right > cell1X) = True  -- Vertically adjacent.
    | otherwise = False
  where
    cell1Right = cell1X + cell1Size
    cell1Bottom = cell1Y + cell1Size
    cell2Right = cell2X + cell2Size
    cell2Bottom = cell2Y + cell2Size