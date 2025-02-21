#include "Object.h"

// Constructor
Object::Object(int xcor, int ycor, string t) {
    x = xcor;
    y = ycor;
    type = t;
}

// Getters
int Object::getX() { return x; }
int Object::getY() { return y; }
string Object::getType() { return type; }


// Setters
void Object::setX(int newx) { x = newx; }
void Object::setY(int newy) { y = newy; }
void Object::setType(string newType) { type = newType; }



// Display method
void Object::display() {
    if (type == "pacman") {
        cout << "P";
    }
    else if (type == "ghost") {
        cout << "G";
    }
    else if (type == "wall") {
        cout << "#";
    }
    else if (type == "pellet") {
        cout << "*";
    }
    else if (type == "superPellet") {
        cout << "@";
    }
    else if (type == "empty") {
        cout << "_";
    }
    else {
        cout << "?";
    }
}
