#pragma once
#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <vector>
#include "Object.h"

using namespace std;

class Grid {
private:
    vector<vector<Object>> spots; // Change this to hold actual Objects or pointers as needed

public:
    // Default constructor
    Grid();

    // Constructor with parameters
    Grid(int rows, int cols, vector<Object*> walls);

    // Method to get an Object at specific coordinates
    Object& getObject(int x, int y); // Consider returning a reference

    void setObject(int x, int y, Object obj);

    vector<int> getPacman();
    bool ateAllPellets();
};

#endif // GRID_H
