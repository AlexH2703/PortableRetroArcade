#pragma once
#ifndef LEVEL1_H
#define LEVEL1_H

#include <iostream>
#include <vector>
#include "Object.h"
#include "Grid.h"

using namespace std;

class Level1 {
private:
    vector<Object*> walls; // Vector to hold wall objects
    Grid level1;           // Grid representing the level
    int frenzyTimer;

public:
    // Default constructor
    Level1();

    int getFrenzyTime();

    void setFrenzyTime(int i);

    Grid& getGrid();

    void addWalls(int startRow, int endRow, int startCol, int endCol);

};

#endif // LEVEL1_H
