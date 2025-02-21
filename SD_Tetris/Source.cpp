using namespace System;
using namespace System::Drawing;

public ref class GameBoard {
private:
    const int width = 10;  // Width of the board
    const int height = 20; // Height of the board
    array<int>^ board;     // 1D array to hold the game state

public:
    GameBoard() {
        // Initialize the board with zeros (empty cells)
        board = gcnew array<int>(width * height);
    }

    // Method to get the value at a specific coordinate
    int GetValue(int x, int y) {
        return board[x + y * width];
    }

    // Method to set the value at a specific coordinate
    void SetValue(int x, int y, int value) {
        board[x + y * width] = value;
    }

    // Method to draw the board
    void Draw(Graphics^ g) {
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                if (GetValue(x, y) != 0) {
                    // Draw filled square for occupied cells
                    g->FillRectangle(Brushes::Blue, x * 20, y * 20, 20, 20);
                }
                // Optionally draw empty cell outline
                g->DrawRectangle(Pens::Black, x * 20, y * 20, 20, 20);
            }
        }
    }

    // Method to clear completed lines
    void ClearLines() {
        for (int y = height - 1; y >= 0; y--) {
            bool lineFull = true;
            for (int x = 0; x < width; x++) {
                if (GetValue(x, y) == 0) {
                    lineFull = false;
                    break;
                }
            }
            if (lineFull) {
                // Shift all lines down
                for (int shiftY = y; shiftY > 0; shiftY--) {
                    for (int x = 0; x < width; x++) {
                        SetValue(x, shiftY, GetValue(x, shiftY - 1));
                    }
                }
                // Clear the top line
                for (int x = 0; x < width; x++) {
                    SetValue(x, 0, 0);
                }
                y++; // Check this line again
            }
        }
    }

    // Method to place a piece (placeholder for actual implementation)
    bool PlacePiece(int x, int y, int pieceId) {
        // Example: Check bounds and place the piece
        if (x >= 0 && x < width && y >= 0 && y < height) {
            SetValue(x, y, pieceId); // Place the piece
            return true;
        }
        return false; // Placement failed
    }
};
