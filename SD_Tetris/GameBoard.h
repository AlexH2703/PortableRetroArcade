#include "TetrisForm.h"
#include <iostream>

using namespace System;
using namespace System::Drawing;

public ref class GameBoard {
private:
    array<array<int>^>^ board; // 2D array to represent the game board
    const int rows = 20;
    const int cols = 10;
    const int blockSize = 25;
    bool isGameOver = false;
    int score;
    int level;
    int linesCleared;

public:
    GameBoard() {
        // Initialize the outer array
        board = gcnew array<array<int>^>(rows);

        // Initialize each row of the outer array
        for (int i = 0; i < rows; i++) {
            board[i] = gcnew array<int>(cols); // Allocate space for each column in the row
            for (int j = 0; j < cols; j++) {
                board[i][j] = 0; // Set each cell to 0 (empty)
            }
        }
    }

    property int Score {
        int get() { return score; }
        void set(int value) { score = value; }
    }

    property int Level {
        int get() { return level; }
        void set(int value) { level = value; }
    }

    property int LinesCleared {
        int get() { return linesCleared; }
        void set(int value) { linesCleared = value; }
    }

    void Reset() {
        Clear(); // Clear the game board
        score = 0; // Reset score
        level = 0; // Reset level (or any starting level)
        linesCleared = 0; // Reset lines cleared
        isGameOver = false; // Reset game over state
    }


    bool CanPlacePiece(int x, int y, array<Point>^ shapeCoords) {
        
        for each (Point p in shapeCoords) {
            int newX = x + p.X;
            int newY = y + p.Y;

            // Check if the point is out of bounds
            if (newX < 0 || newX >= cols || newY < 0 || newY >= rows) {
                return false;
            }

            //Check if the spot is occupied by another shape
            if (newY >= 0 && board[newY][newX] != 0) {
                return false;
            }
        }
        return true; // Piece can be placed
    }

    // Public property to check if the game is over
    property bool IsGameOver {
        bool get() { return isGameOver; }
        void set(bool value) { isGameOver = value; }
    }

    // Modify PlacePiece to set isGameOver if needed
    void PlacePiece(int x, int y, array<Point>^ shapeCoords, int value) {
        for each (Point p in shapeCoords) {
            int newX = x + p.X;
            int newY = y + p.Y;
            if (newY >= 0) { // Ensure it doesn't write to an invalid row
                board[newY][newX] = value; // Mark the space as occupied
                //Console::WriteLine("Placing: {0}, {1}", newX, newY);

                // Check for game over condition
                if (newY < 0) { // If any part of the piece is in the top row
                    IsGameOver = true; // Set the game over flag
                    return; // Exit the method
                }
            }
        }
        ClearLines(); // Call ClearLines after placing the piece
        PrintBoard(); // Print the board after placing the piece
    }

    void ClearLines() {
        int linesClearedInPass = 0;

        for (int row = rows - 1; row >= 0; row--) {
            bool lineComplete = true;

            for (int col = 0; col < cols; col++) {
                if (board[row][col] == 0) {
                    lineComplete = false;
                    break;
                }
            }

            if (lineComplete) {
                linesClearedInPass++; // Count this line as cleared
                linesCleared++;

                // Shift down all lines above
                for (int r = row; r > 0; r--) {
                    for (int c = 0; c < cols; c++) {
                        board[r][c] = board[r - 1][c];
                    }
                }

                // Clear the top row
                for (int c = 0; c < cols; c++) {
                    board[0][c] = 0;
                }

                row++; // Recheck this row after shifting
            }
        }
        int targetLinesForNextLevel = (level + 1) * 10;
        if (linesCleared >= targetLinesForNextLevel) {
            level++; // Increase level
            //Console::WriteLine("Level up! New level: {0}", level);
        }
        // Score multiplier based on lines cleared in a single move
        switch (linesClearedInPass) {
        case 1:
            score += 100; // Single line clear
            break;
        case 2:
            score += 300; // Double line clear
            break;
        case 3:
            score += 500; // Triple line clear
            break;
        case 4:
            score += 800; // Tetris (quadruple line clear)
            break;
        default:
            break;
        }

        Console::WriteLine("Lines Cleared: {0}, Score: {1}, Level: {2}", linesCleared, score, level);
    }


    void Draw(Graphics^ g) {
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                if (board[row][col] != 0) { // If the cell is filled
                    Color blockColor = GetColorForBlock(board[row][col]);
                    SolidBrush^ brush = gcnew SolidBrush(blockColor);
                    Rectangle rect = Rectangle(col * blockSize, row * blockSize, blockSize, blockSize);
                    g->FillRectangle(brush, rect);
                    DrawShapeGrid(g, rect); // Draw the grid on the filled block
                    delete brush;
                }
            }
        }
    }

    Color GetColorForBlock(int blockType) {
        // Define colors based on the block type
        switch (blockType) {
        case 1: return Color::LimeGreen;
        case 2: return Color::LightCoral;
        case 3: return Color::LightSkyBlue;
        case 4: return Color::DeepPink;
        case 5: return Color::DarkGreen;
        case 6: return Color::MediumBlue;
        case 7: return Color::Orchid;
        default: return Color::Transparent;
        }
    }

    void DrawShapeGrid(Graphics^ g, Rectangle shape) {
        // Draw grid inside the shape
        Pen^ gridPen = Pens::Black;

        // Draw vertical grid lines
        for (int x = shape.X; x <= shape.X + shape.Width; x += shape.Width) {
            g->DrawLine(gridPen, x, shape.Y, x, shape.Y + shape.Height);
        }

        // Draw horizontal grid lines
        for (int y = shape.Y; y <= shape.Y + shape.Height; y += shape.Height) {
            g->DrawLine(gridPen, shape.X, y, shape.X + shape.Width, y);
        }
    }

    bool IsOccupied(int x, int y) {
        // Check boundaries
        if (x < 0 || x >= cols || y < 0 || y >= rows) {
            return true; // Out of bounds means occupied
        }
        return board[y][x] != 0; // Return true if the cell is not empty
    }

    void PrintBoard() {
        // Print the board to the console
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                Console::Write(board[row][col] + " "); // Print each cell followed by a space
            }
            Console::WriteLine(); // Move to the next line after each row
        }
        Console::WriteLine(); // Add an empty line for better readability
    }

    void Clear() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                board[i][j] = 0; // Set all cells to 0 (empty)
            }
        }
    }
};
