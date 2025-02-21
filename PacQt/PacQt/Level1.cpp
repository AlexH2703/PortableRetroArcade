#include "Level1.h"


// Helper function to add walls in a rectangular area
void Level1::addWalls(int startRow, int endRow, int startCol, int endCol) {
    for (int row = startRow; row <= endRow; row++) {
        for (int col = startCol; col <= endCol; col++) {
            walls.push_back(new Object(row, col, "wall"));
        }
    }
}

// Default constructor
Level1::Level1() {

    // Initialize walls along the borders
    for (int i = 0; i <= 30; i++) {
        walls.push_back(new Object(i, 0, "wall")); // Left
        walls.push_back(new Object(i, 27, "wall")); // Right
    }
    for (int j = 0; j <= 27; j++) {
        walls.push_back(new Object(0, j, "wall")); // Top
        walls.push_back(new Object(30, j, "wall")); // Bottom
    }


    addWalls(2, 4, 2, 5);
    addWalls(2, 4, 7, 11);
    addWalls(2, 4, 16, 20);
    addWalls(2, 4, 22, 25);
    addWalls(1, 4, 13, 14);
    addWalls(9, 13, 1, 5);
    addWalls(9, 13, 22, 27);
    addWalls(15, 19, 1, 5);
    addWalls(15, 19, 22, 27);
    addWalls(24, 25, 1, 2);
    addWalls(24, 25, 25, 27);
    addWalls(6, 7, 2, 5);
    addWalls(6, 7, 22, 25);
    addWalls(15, 19, 7, 8);
    addWalls(15, 19, 19, 20);
    addWalls(21, 22, 7, 11);
    addWalls(21, 22, 16, 20);
    addWalls(21, 22, 2, 5);
    addWalls(23, 25, 4, 5);
    addWalls(21, 22, 22, 25);
    addWalls(23, 25, 22, 23);
    addWalls(6, 7, 10, 17);
    addWalls(8, 10, 13, 14);
    addWalls(18, 19, 10, 17);
    addWalls(20, 22, 13, 14);
    addWalls(24, 25, 10, 17);
    addWalls(26, 28, 13, 14);
    addWalls(6, 13, 7, 8);
    addWalls(9, 10, 9, 11);
    addWalls(6, 13, 19, 20);
    addWalls(9, 10, 16, 18);
    addWalls(27, 28, 2, 11);
    addWalls(24, 27, 7, 8);
    addWalls(27, 28, 16, 25);
    addWalls(24, 27, 19, 20);
    addWalls(12, 12, 10, 17);
    addWalls(16, 16, 10, 17);
    addWalls(13, 15, 10, 10);
    addWalls(13, 15, 17, 17);

    // Initialize the Grid with the walls vector
    level1 = Grid(31, 28, walls);

    frenzyTimer = 0;
}


int Level1::getFrenzyTime() {
    return this->frenzyTimer;
}

void Level1::setFrenzyTime(int i) {
    this->frenzyTimer = i;
}

Grid& Level1::getGrid() {
    return level1;
}
