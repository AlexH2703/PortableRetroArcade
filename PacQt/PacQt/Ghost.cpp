#include <QGraphicsPixmapItem>
#include "PacQt.h"
#include "ghost.h"
#include <QDebug>
#include <cmath> // for std::floor


Ghost::Ghost(const QPixmap& pixmap, const QPointF& scatterTarget, int speed, QGraphicsScene* scene, Level1& level, float itemSize)
    : QGraphicsPixmapItem(pixmap),
    scatterTarget(scatterTarget),
    speed(speed),
    scene(scene),
    level1(level),
    direction(-1, 0)
{
    target = scatterTarget;
    mode = "chase";
    distanceInTunnel = 0;
    enteringHouse = false;
    this->itemSize = itemSize;
    this->setScale(this->itemSize / 34);
}

QPointF Ghost::getStartingPosition() {
    if (dynamic_cast<Blinky*>(this)) {
        return QPointF(14 * itemSize, 11 * itemSize);
    }
    else if (dynamic_cast<Pinky*>(this)) {
        return QPointF(16 * itemSize, 13 * itemSize);
    }
    else if (dynamic_cast<Inky*>(this)) {
        return QPointF(11 * itemSize, 15 * itemSize);
    }
    else if (dynamic_cast<Clyde*>(this)) {
        return QPointF(16 * itemSize, 15 * itemSize);
    }
    return QPointF(-1, -1);
}

void Ghost::handleTunnel() {
    qreal y = this->pos().y();
    qreal x = this->pos().x();
    if ((y == 14*itemSize && x > 0 && x < 5 * itemSize) || (y == 14*itemSize && x > 22 * itemSize && x < 27 * itemSize)) {
        this->setSpeed(1);
        this->distanceInTunnel += 2;
    }
    else if (this->distanceInTunnel >= 19.56*itemSize) {
        this->setSpeed(2);
        this->distanceInTunnel = 0;
    }
    if (y == 14 * itemSize && x == 2) {
        this->setPos(27 * itemSize - 2, 14*itemSize);
    }
    else if (y == 14 * itemSize && x == 27 * itemSize - 2) {
        this->setPos(2, 14 * itemSize);
    }
}

void Ghost::resetGhost() {
    this->setPos(getStartingPosition());
    this->direction = QPointF(-1, 0);
}

void Ghost::setTarget(const QPointF& targetPos) {
    this->target = targetPos;  // Set the ghost's current target position
}

void Ghost::setDirection(const QPointF& dir) {
    this->direction = dir;
}

void Ghost::setMode(string newMode) {
    this->mode = newMode;
}

void Ghost::setSpeed(int i) {
    this->speed = i;
}

int Ghost::getSpeed() {
    return this->speed;
}

void Ghost::updateMovement() {
    for (int i = 0; i < speed; i++) {
        bool currentlyAtIntersection = atIntersection(); 
        
        if (currentlyAtIntersection && !isInIntersection) { //Check if it is entering at an intersection
            isInIntersection = true;  
        }
        else if (!currentlyAtIntersection) {
            isInIntersection = false;  // Reset the flag when leaving the intersection
        }

        QVector<QPointF> availableDirections = possibleDirections(); //gets all directions that don't have a wall
        // move in the shortest direction while it is in an intersection or if it is at a corner pick the direction that
        // is not where the ghost came from
        if (isInIntersection || (availableDirections.length() == 1 && availableDirections.at(0) != direction)) {
            setPos(pos() + availableDirections.at(0)); 
            direction = availableDirections.at(0);
        }
        else {
            setPos(pos() + direction); 
        }

        handleTunnel();
    }
}

bool Ghost::isWallInDirection(const QPointF& pos, const QPointF& direction) {
    QRectF ghostBoundingRect = QRectF(pos.x() + direction.x() * 1, pos.y() + direction.y() * 1, itemSize, itemSize);    //multiply by speed

    // Calculate grid-aligned positions using floor for consistent positioning
    int top = std::floor((ghostBoundingRect.top()+0.00001) / itemSize);
    int bottom = std::floor((ghostBoundingRect.bottom()-0.00000001) / itemSize);
    int left = std::floor((ghostBoundingRect.left()+0.00001) / itemSize);
    int right = std::floor((ghostBoundingRect.right()-0.00000001) / itemSize);

    bool topLeft = level1.getGrid().getObject(top, left).getType() == "wall";
    bool topRight = level1.getGrid().getObject(top, right).getType() == "wall";
    bool bottomLeft = level1.getGrid().getObject(bottom, left).getType() == "wall";
    bool bottomRight = level1.getGrid().getObject(bottom, right).getType() == "wall";

    if (direction == QPointF(0, -1)) {  // up
        return topLeft || topRight;
    }
    else if (direction == QPointF(0, 1)) {  // down
        return bottomLeft || bottomRight;
    }
    else if (direction == QPointF(-1, 0)) {  // left
        return bottomLeft || topLeft;  
    }
    else if (direction == QPointF(1, 0)) {  // right
        return bottomRight || topRight;  
    }

    return false;
}

bool Ghost::atIntersection() {
    QVector<QPointF> possibleDirections = { QPointF(0, -1), QPointF(0, 1), QPointF(-1, 0), QPointF(1, 0) };

    int openPaths = 0;
    for (const QPointF& dir : possibleDirections) {
        QPointF currentPos = this->pos();
        if (!isWallInDirection(currentPos, dir)) {
            openPaths++;
        }
    }
    // Intersection if there are more than two open paths
    return openPaths > 2;
}

QVector<QPointF> Ghost::possibleDirections() {
    QVector<QPointF> possibleDirections = { QPointF(0, -1), QPointF(0, 1), QPointF(-1, 0), QPointF(1, 0) };
    QVector<QPointF> validDirections; // Store only directions that aren't blocked by walls

    QPointF currentPos = this->pos();  // Store current position

    // Find valid directions that are not blocked by walls
    for (const QPointF& dir : possibleDirections) {
        if (!isWallInDirection(currentPos, dir) && direction != -dir) {
            validDirections.append(dir);
        }
    }
    
    sortDirectionsByDistance(validDirections, this->pos());

    return validDirections;
}

void Ghost::sortDirectionsByDistance(QVector<QPointF>& directions, const QPointF& currentPos) {
    QVector<QPointF> sortedDirections = directions; // Copy the input directions

    // Sort using a simple bubble sort (or you can use std::sort with a custom comparator)
    for (int i = 0; i < sortedDirections.size(); ++i) {
        for (int j = 0; j < sortedDirections.size() - i - 1; ++j) {
            // Calculate the new position after applying the direction
            QPointF newPos = currentPos + sortedDirections[j] * itemSize;
            QPointF newPosNext = currentPos + sortedDirections[j + 1] * itemSize;

            // Calculate distances to the target
            qreal distA = QLineF(newPos, target).length();
            qreal distB = QLineF(newPosNext, target).length();

            // If the next direction is closer to the target, swap them
            if (distA > distB) {
                std::swap(sortedDirections[j], sortedDirections[j + 1]);
            }
        }
    }
    
    directions = sortedDirections;
}

bool Ghost::isInHouse() {
    return this->inHouse;
}

void Ghost::enterHouse() {
    this->inHouse = true;
}

void Ghost::leaveHouse() {
    this->setPos(14 * itemSize, this->pos().y() -speed/2);
    if (this->pos() == QPointF(14 * itemSize, 11 * itemSize)) {
        this->inHouse = false;
        this->setDirection(QPointF(-1, 0));
        this->setMode("chase");
    }
}

void Ghost::enterHouseAnimation() {
    this->setPos(14 * itemSize, this->pos().y() + speed);
    if ((floor(this->pos().x() / speed) == floor(14 * itemSize / speed)) && (floor(this->pos().y() / speed) == floor(13 * itemSize / speed))) {
        this->enteringHouse = false;
        this->inHouse = true;
    }
}

// Blinky constructor
Blinky::Blinky(QGraphicsScene* scene, Level1& level, float itemSize)
    : Ghost(QPixmap(":/PacQt/blinky.png"), QPointF(27 * itemSize, 0), 2, scene, level, itemSize) {
    inHouse = false;
}

void Blinky::updateMovement(const QPointF& pacmanPos) {
    if (mode == "chase") {
        this->setSpeed(2);
        this->setTarget(pacmanPos);
    }
    else if (mode == "scatter") {
        this->setTarget(scatterTarget);
    }
    else if (mode == "home") {
        this->setSpeed(6);
        if ((floor(this->pos().x() / speed) == floor(14*itemSize / speed)) && (floor(this->pos().y() / speed) == floor(11 * itemSize / speed)) || this->enteringHouse) {
            this->enteringHouse = true;
            this->enterHouseAnimation();
        }
        else if (!this->isInHouse()) {
            this->setTarget(QPointF(14 * itemSize, 11 * itemSize));
        }
    }
    if (!this->enteringHouse) {
        Ghost::updateMovement();
    }
}

// Pinky constructor
Pinky::Pinky(QGraphicsScene* scene, Level1& level, float itemSize)
    : Ghost(QPixmap(":/PacQt/pinky.png"), QPointF(0, 0), 2, scene, level, itemSize) {
    inHouse = true;
}

void Pinky::updateMovement(const QPointF& pacmanPos, const QPointF& pacmanDirection) {
    if (mode == "chase") {
         this->setTarget(pacmanPos + pacmanDirection * 4 * itemSize);
    }
    else if (mode == "scatter") {
        this->setTarget(scatterTarget);
    }
    else if (mode == "home") {
        this->setSpeed(6);
        if ((floor(this->pos().x() / speed) == floor(14 * itemSize / speed)) && (floor(this->pos().y() / speed) == floor(11 * itemSize / speed)) || this->enteringHouse) {
            this->enteringHouse = true;
            this->enterHouseAnimation();
        }
        else if (!this->isInHouse()) {
            this->setTarget(QPointF(14 * itemSize, 11 * itemSize));
        }
    }
    if (!this->enteringHouse) {
        Ghost::updateMovement();
    }
}


Inky::Inky(QGraphicsScene* scene, Level1& level, float itemSize)
    : Ghost(QPixmap(":/PacQt/inky.png"), QPointF(27 * itemSize, 31*itemSize), 2, scene, level, itemSize) {
    inHouse = true;
}

void Inky::updateMovement(const QPointF& pacmanPos, const QPointF& pacmanDirection, const QPointF& blinkyPos) {
    if (mode == "chase") {
        QPointF pacmanOffset = pacmanPos + pacmanDirection * 2 * itemSize;
        QLineF inkyVector = QLineF(blinkyPos, pacmanOffset);
        inkyVector.setLength(inkyVector.length() * 2);
        QPointF inkyTarget = inkyVector.p2();
        this->setTarget(inkyTarget);
    }
    else if (mode == "scatter") {
        this->setTarget(scatterTarget);
    }
    else if (mode == "home") {
        this->setSpeed(6);
        if ((floor(this->pos().x() / speed) == floor(14 * itemSize / speed)) && (floor(this->pos().y() / speed) == floor(11 * itemSize / speed)) || this->enteringHouse) {
            this->enteringHouse = true;
            this->enterHouseAnimation();
        }
        else if (!this->isInHouse()) {
            this->setTarget(QPointF(14 * itemSize, 11 * itemSize));
        }
    }
    if (!this->enteringHouse) {
        Ghost::updateMovement();
    }
}

Clyde::Clyde(QGraphicsScene* scene, Level1& level, float itemSize)
    : Ghost(QPixmap(":/PacQt/clyde.png"), QPointF(0, 31*itemSize), 2, scene, level, itemSize) {
    inHouse = true;
}

void Clyde::updateMovement(const QPointF& pacmanPos) {
    if (mode == "chase") {
        qreal distanceToPacman = QLineF(this->pos(), pacmanPos).length();
        if (distanceToPacman < 8 * itemSize) {
            this->setTarget(scatterTarget);
        }
        else {
            this->setTarget(pacmanPos);
        }
    }
    else if (mode == "scatter") {
        this->setTarget(scatterTarget);
    }
    else if (mode == "home") {
        this->setSpeed(6);
        if ((floor(this->pos().x() / speed) == floor(14 * itemSize / speed)) && (floor(this->pos().y() / speed) == floor(11 * itemSize / speed)) || this->enteringHouse) {
            this->enteringHouse = true;
            this->enterHouseAnimation();
        }
        else if (!this->isInHouse()) {
            this->setTarget(QPointF(14 * itemSize, 11 * itemSize));
        }
    }
    if (!this->enteringHouse) {
        Ghost::updateMovement();
    }
}
