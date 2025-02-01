-- | Data type representing a quadtree structure.
-- A quadtree can be either completely black, completely white, or a node containing four sub-quadtree.
data Quadtree = AllBlack | AllWhite | Node Quadtree Quadtree Quadtree Quadtree deriving (Eq, Show)

-- | Returns a quadtree that is completely black.
allBlack :: Quadtree
allBlack = AllBlack

-- | Returns a quadtree that is completely white.
allWhite :: Quadtree
allWhite = AllWhite

-- | Combines four quadtree into one node in clockwise order starting from the top-left.
clockwise :: Quadtree -> Quadtree -> Quadtree -> Quadtree -> Quadtree
clockwise a b c d = Node a b c d

-- | Combines four quadtree into one node in anticlockwise order starting from the top-left.
anticlockwise :: Quadtree -> Quadtree -> Quadtree -> Quadtree -> Quadtree
anticlockwise a b c d = Node a d c b

-- | Computes the n-th level coarse approximation of a quadtree.
-- At level 0, the quadtree is entirely white.
-- At each subsequent level, the function recursively approximates each sub-quadtree.
coarseWork :: Int -> Quadtree -> Quadtree
coarseWork 0 _ = AllWhite
coarseWork n AllWhite = AllWhite
coarseWork n AllBlack = AllBlack
coarseWork n (Node a b c d) =
    clockwise (coarseWork (n-1) a) (coarseWork (n-1) b) (coarseWork (n-1) c) (coarseWork (n-1) d)

-- | A function that returns True if the quadtree is mostly white.
fairExercise :: Quadtree -> Bool
fairExercise allWhite = True
fairExercise allBlack = False
fairExercise (Node a b c d) =
    let whites = length $ filter (== True) (map fairExercise [a, b, c, d])  -- Count the number of white sub-quadtree.
    in whites >= 3  -- Return True if at least 3 sub-quadtree are white.

-- | Finds a quadtree that satisfies the provided condition function.
-- Starts from an all-white quadtree and expands it recursively until the condition is met
-- or a maximum depth is reached. If the condition is not met within the depth, returns an all-black quadtree.
-- Example usage: result = my_solution fairExercise
my_solution :: (Quadtree -> Bool) -> Quadtree
my_solution f = search allWhite 0
  where
    maxDepth = 100  -- Set a maximum depth to prevent infinite recursion.
    search qt depth
      | f qt = qt  -- When the condition is met, return the current quadtree.
      | depth >= maxDepth = allBlack  -- When the maximum depth is reached, return an all-black quadtree.
      | otherwise = search (expand qt) (depth + 1)  -- Otherwise, expand the quadtree and continue searching.

-- | Expands a given quadtree by replicating it in all four quadrants.
expand :: Quadtree -> Quadtree
expand qt = clockwise qt qt qt qt