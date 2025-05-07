#include "RedBall.h"

RedBall::RedBall(QWidget* parent)
    : Enemy(parent), horizontalVelo(0.0) {
    redBallImage.load(":/redBall.png"); // Load the red ball image
}

void RedBall::setScale(int width, int height) {
    scaledRedBallImage = redBallImage.scaled(width, height, Qt::KeepAspectRatio);
    resize(width, height);
}

void RedBall::setDirection(const QString& direction) {
    this->direction = direction;
}

QString RedBall::getDirection() const {
    return direction;
}

void RedBall::setHorizonVelo(double d) {
    horizontalVelo = d;
}

double RedBall::getHorizonVelo() const {
    return horizontalVelo;
}

void RedBall::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, scaledRedBallImage);
}

