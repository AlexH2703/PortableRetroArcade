#include "Grid.h"

// Default constructor
Grid::Grid() {}

// Constructor with parameters
Grid::Grid(int rows, int cols, vector<Object*> walls) {
    spots.resize(rows + 1, vector<Object>(cols + 1, Object(0, 0, "blank"))); // Initialize with default Objects

    // Fill the grid with wall objects if they are valid
    for (auto wall : walls) {
        int x = wall->getX();
        int y = wall->getY();
        if (x >= 0 && x < rows && y >= 0 && y < cols) {
            spots.at(x).at(y) = *wall; // Dereference to copy the Object
        }
    }
    spots.at(23).at(13).setType("pacman");

    for (int i = 9; i <= 19; i++) {
        for (int j = 7; j <= 20; j++) {
            if (spots.at(i).at(j).getType() == "blank") {
                spots.at(i).at(j).setType("empty");
            }
        }
    }
    for (int j = 0; j <= 5; j++) {
        spots.at(14).at(j).setType("empty");
    }
    for (int j = 22; j <= 27; j++) {
        spots.at(14).at(j).setType("empty");
    }
    
    spots.at(3).at(1).setType("superPellet");
    spots.at(3).at(26).setType("superPellet");
    spots.at(23).at(1).setType("superPellet");
    spots.at(23).at(26).setType("superPellet");
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (spots.at(i).at(j).getType() == "blank") {
                spots.at(i).at(j).setType("pellet");
            }
        }
    }
}

// Method to get an Object at specific coordinates
Object& Grid::getObject(int x, int y) {
    return spots.at(x).at(y);
}

void Grid::setObject(int x, int y, Object obj) {
    if (x >= 0 && x < spots.size() && y >= 0 && y < spots[0].size()) {
        spots[x][y] = obj; // Set the new object
    }
    else {
        cout << "Cannot set object at (" << x << ", " << y << ") - out of bounds!" << endl;
    }
}

vector<int> Grid::getPacman() {
    for (int i = 0; i < spots.size() - 1; i++) {
        for (int j = 0; j < spots.at(i).size() - 1; j++) {
            if (spots.at(i).at(j).getType() == "pacman") {
                vector<int> coords = { i, j };
                return coords;
            }
        }
    }
    return { -1, -1};
}

bool Grid::ateAllPellets() {
    for (int i = 0; i < spots.size() - 1; i++) {
        for (int j = 0; j < spots.at(i).size() - 1; j++) {
            if (getObject(i, j).getType() == "pellet" || getObject(i, j).getType() == "superPellet") {
                return false;
            }
        }
    }
    return true;
}
