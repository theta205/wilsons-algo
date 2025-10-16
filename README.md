# Wilson’s Algorithm Maze Generator

A C++ implementation of **Wilson’s Algorithm** for generating perfect mazes — mazes with exactly **one unique path** between any two cells.

---

## How Wilson’s Algorithm Works

1. Start with a grid of walls (`#`).
2. Choose one random cell and mark it as part of the maze.
3. Pick another random cell not in the maze.
4. Perform a **random walk** until the walk touches the existing maze.
   - If a loop forms during the walk, erase it (loop erasure).
5. Convert the walk’s cells into maze passages (`' '`).
6. Repeat until every cell is part of the maze or a "wall".

This produces a **perfect maze** — every cell is reachable, and no cycles exist.

---

## Valid cmds

1. make (builds executable)
2. make clean (removes the executable)
3. make run (runs executable)

## File Structure
````
Wilson_Algo/
├── main.cpp        # Core implementation of Wilson's algorithm
├── Makefile        # Build commands
├── README.md       # Project documentation
└── .gitignore      # Ignore build artifacts

