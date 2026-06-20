# 2D Graphics Editors in C

This repository contains two command-line interactive **2D Graphics Editors** written in C. They enable drawing shapes (Lines, Rectangles, Circles, and Triangles) onto a text-based canvas using ascii characters (`_` for empty space and `*` for lines/points).

---

## Table of Contents
1. [Programs Overview](#programs-overview)
2. [Features Comparison](#features-comparison)
3. [Detailed Shape Algorithms](#detailed-shape-algorithms)
4. [File Format (for Dynamic Editor)](#file-format-for-dynamic-editor)
5. [Compilation and Usage](#compilation-and-usage)
6. [Project Structure](#project-structure)

---

## Programs Overview

### 1. Dynamic Vector Graphics Editor ([2D_editor.c](file:///c:/c%20programming%20files/2D_editor.c))
This is the **advanced version** of the graphics editor. It models shapes as discrete vector items stored in a dynamic linked list.
* **Vector Design**: Instead of immediately rasterizing shapes onto a fixed board, it keeps them in memory as a linked list (`ShapeNode`). 
* **Layer Control**: You can list, modify, or delete existing shapes at any time using their unique auto-incrementing ID.
* **Persistency**: Supports saving the vector shapes to a text file and loading them back to resume editing.
* **Canvas Size**: `22` rows by `65` columns.

### 2. Static Graphics Editor ([y.c](file:///c:/c%20programming%20files/y.c))
This is a **simpler, direct-render version**.
* **Raster Design**: Shapes are rasterized directly onto a static 2D character matrix (`char picture[HEIGHT][WIDTH]`) immediately after input.
* **Draw-and-Display**: Once a shape is drawn, its underlying parameters are forgotten; it becomes part of the raw pixel grid.
* **Canvas Size**: `24` rows by `80` columns.

---

## Features Comparison

| Feature | Dynamic Editor (`2D_editor.c`) | Static Editor (`y.c`) |
| :--- | :---: | :---: |
| **Canvas Size** | 22 x 65 | 24 x 80 |
| **Shape Management** | Linked list (individual shape nodes) | Static char grid |
| **Modify Shapes** | Yes (via Shape ID) | No (shapes are baked in) |
| **Delete Shapes** | Yes (via Shape ID) | No |
| **Save & Load** | Yes (Text-based shape data) | No |
| **Supported Shapes** | Line, Rectangle, Circle, Triangle | Line, Rectangle, Circle, Triangle |
| **Memory Allocation** | Dynamic (`malloc`/`free`) | Static Array |

---

## Detailed Shape Algorithms

Both applications implement custom computer graphics rasterization algorithms:
* **Lines**: Both utilize **Bresenham's Line Algorithm** to accurately project a line between any two coordinate pairs `(x1, y1)` and `(x2, y2)` without using floating-point math.
* **Rectangles**: Constructed by drawing 4 lines connecting the corners.
* **Circles**:
  * `2D_editor.c` implements the **Midpoint Circle Algorithm** (Bresenham's Circle) to draw only the perimeter of the circle.
  * `y.c` implements a filled circle approach using coordinate bounds check ($dx^2 + dy^2 \le r^2$).
* **Triangles**: Constructed by drawing 3 lines connecting the three input vertices.

---

## File Format (for Dynamic Editor)

When saving a drawing in `2D_editor.c`, shapes are serialized line-by-line into a text file using the following structure:
```text
<shape_id> <shape_type_enum> <coordinates...>
```

### Shape Type Enums
* `1` - Line
* `2` - Rectangle
* `3` - Circle
* `4` - Triangle

### Serialization Structure Examples

* **Line**: `ID 1` from `(0,0)` to `(10,12)`
  ```text
  1 1 0 0 10 12
  ```
* **Rectangle**: `ID 2` with top-left `(5,5)` and bottom-right `(15,10)`
  ```text
  2 2 5 5 15 10
  ```
* **Circle**: `ID 3` centered at `(30,10)` with radius `5`
  ```text
  3 3 30 10 5
  ```
* **Triangle**: `ID 4` with vertices `(10,5)`, `(20,15)`, and `(5,15)`
  ```text
  4 4 10 5 20 15 5 15
  ```

---

## Compilation and Usage

To compile the projects, you can use any standard C compiler (like `gcc`).

### Compiling
```bash
# Compile the Dynamic Vector Editor
gcc 2D_editor.c -o 2D_editor.exe

# Compile the Static Editor
gcc y.c -o y.exe
```

### Running
```bash
# Run the Dynamic Vector Editor
./2D_editor.exe

# Run the Static Editor
./y.exe
```

---

## Project Structure

* [2D_editor.c](file:///c:/c%20programming%20files/2D_editor.c) - Dynamic shape-based graphics editor.
* [y.c](file:///c:/c%20programming%20files/y.c) - Static pixel-grid graphics editor.
* [.gitignore](file:///c:/c%20programming%20files/.gitignore) - Excludes compiled binaries (`*.exe`) and VS Code metadata.
* `README.md` - This documentation.
