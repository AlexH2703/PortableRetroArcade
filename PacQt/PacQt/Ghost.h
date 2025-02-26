#pragma once
#ifndef GHOST_H
#define GHOST_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPointF>
#include <QVector>
#include <QLineF>
#include "Level1.h" // Include the Level1 class for grid handling


class Ghost : public QGraphicsPixmapItem {
public:
    Ghost(const QPixmap& pixmap, const QPointF& scatterTarget, int speed, QGraphicsScene* scene, Level1& level, float itemSize);

    // Update ghost's movement based on the mode and Pacman's position
    void updateMovement();

    QPointF getStartingPosition();

    void handleTunnel();

    void resetGhost();

    // Set the current target (used in chase mode to follow Pacman or in scatter mode to head to a corner)
    void setTarget(const QPointF& targetPos);

    void setDirection(const QPointF& dir);

    void setMode(string newMode);

    void setSpeed(int i);

    int getSpeed();

    // Get the chase mode status
    string getMode() const { return mode; }

    bool isInHouse();

    void enterHouse();

    void leaveHouse();

    void enterHouseAnimation();


protected:
    QPointF direction;       // Current direction of movement
    QPointF target;          // Target position (can be Pacman or a scatter point)
    QPointF scatterTarget;   // The scatter point (corner) for each ghost
    int speed;               // Ghost movement speed
    string mode;          // Determines whether the ghost is in chase or scatter mode
    QGraphicsScene* scene;   // The graphics scene where the ghost moves
    Level1& level1;          // Reference to the level to check walls and paths
    bool isInIntersection = false;
    bool inHouse;
    int distanceInTunnel;
    bool enteringHouse;
    float itemSize;

    // Check if the ghost is at an intersection
    bool atIntersection();

    // Choose the best direction at an intersection based on the target
    QVector<QPointF> possibleDirections();

    void sortDirectionsByDistance(QVector<QPointF>& directions, const QPointF& currentPos);

    // Check if there's a wall in a specific direction
    bool isWallInDirection(const QPointF& pos, const QPointF& direction);
};

// Specific ghost subclasses
class Blinky : public Ghost {
public:
    Blinky(QGraphicsScene* scene, Level1& level, float itemSize);
    void updateMovement(const QPointF& pacmanPos);
};

class Pinky : public Ghost {
public:
    Pinky(QGraphicsScene* scene, Level1& level, float itemSize);

    // Override updateMovement to add Pinky's specific chase behavior
    void updateMovement(const QPointF& pacmanPos, const QPointF& pacmanDirection);
};

class Inky : public Ghost {
public:
    Inky(QGraphicsScene* scene, Level1& level, float itemSize);

    void updateMovement(const QPointF& pacmanPos, const QPointF& pacmanDirection, const QPointF& blinkyPos);
};

class Clyde : public Ghost {
public:
    Clyde(QGraphicsScene* scene, Level1& level, float itemSize);

    void updateMovement(const QPointF& pacmanPos);

};

#endif // GHOST_H
