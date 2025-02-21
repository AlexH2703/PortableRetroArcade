#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <string>
using namespace std;

class Object {
private:
    int x;
    int y;
    string type;

public:
    // Constructor
    Object(int xcor, int ycor, string t);

    // Getters
    int getX();
    int getY();
    string getType();

    // Setters
    void setX(int newx);
    void setY(int newy);
    void setType(string newType);

    // Display method
    void display();
};

#endif // OBJECT_H
