#include "Cube.h"

Cube::Cube(QWidget* parent, int cW, int cH, int d) : QWidget(parent) {
    setFixedSize(cW * 2, cH * 2); // Set a fixed size for each cube
    cubeWidth = cW;
    cubeHeight = cH;
    cubeDepth = d;

    // Initialize colors
    startColor = QColor(255, 0, 0);  // Red
    middleColor = QColor(255, 255, 0); // Yellow
    goalColor = QColor(255, 255, 255);   // White

    topFaceColor = startColor; // Default top face color
    state = 0; // Initial state
}

void Cube::setTopFaceColor(const QColor& color) {
    topFaceColor = color;
    update(); // Trigger a repaint to apply the new color
}

QColor Cube::getTopFaceColor() const {
    return topFaceColor;
}

int Cube::setState(int level) {
    switch (level) {
    case 1: case 2: case 3: case 4: // Levels 1-4
        if (state == 0) {
            setTopFaceColor(goalColor);
            state = 1;
            return 25; // Changed to goal color
        }
        break;

    case 5: case 6: case 7: case 8: // Levels 5-8
        if (state == 0) {
            setTopFaceColor(middleColor);
            state = 1;
            return 15; // Changed to middle color
        }
        else if (state == 1) {
            setTopFaceColor(goalColor);
            state = 2;
            return 25; // Changed to goal color
        }
        break;

    case 9: case 10: case 11: case 12: // Levels 9-12
        if (state == 0) {
            setTopFaceColor(goalColor);
            state = 1;
            return 25; // Changed to goal color
        }
        else if (state == 1) {
            setTopFaceColor(startColor);
            state = 0;
            return 0; // No score for reverting to start color
        }
        break;

    case 13: case 14: case 15: case 16: // Levels 13-16
        if (state == 0) {
            setTopFaceColor(middleColor);
            state = 1;
            return 15; // Changed to middle color
        }
        else if (state == 1) {
            setTopFaceColor(goalColor);
            state = 2;
            return 25; // Changed to goal color
        }
        else if (state == 2) {
            setTopFaceColor(middleColor);
            state = 1;
            return 0; // No score for reverting to middle color
        }
        break;

    default: // Levels 17+
        if (state == 0) {
            setTopFaceColor(middleColor);
            state = 1;
            return 15; // Changed to middle color
        }
        else if (state == 1) {
            setTopFaceColor(goalColor);
            state = 2;
            return 25; // Changed to goal color
        }
        else if (state == 2) {
            setTopFaceColor(startColor);
            state = 0;
            return 0; // No score for reverting to start color
        }
        break;
    }
    return 0; // No change
}

void Cube::resetState() {
    setTopFaceColor(startColor);
    state = 0;
}

void Cube::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Colors for different cube faces
    QColor leftColor(0, 200, 0); 
    QColor rightColor(0, 0, 200); 

    // Define cube points
    int x = 0;
    int y = 0;
    QPoint top(x + cubeWidth / 2, y);
    QPoint leftTop(x, y + cubeDepth);
    QPoint rightTop(x + cubeWidth, y + cubeDepth);
    QPoint leftBottom(x, y + cubeHeight);
    QPoint rightBottom(x + cubeWidth, y + cubeHeight);
    QPoint bottom(x + cubeWidth / 2, y + cubeDepth + cubeHeight);
    QPoint middle(x + cubeWidth / 2, y + cubeDepth * 2);

    // Draw top face
    QPolygon topFace({ top, leftTop, middle, rightTop });
    painter.setBrush(topFaceColor);
    painter.drawPolygon(topFace);

    // Draw left face
    QPolygon leftFace({ leftTop, leftBottom, bottom, middle });
    painter.setBrush(leftColor);
    painter.drawPolygon(leftFace);

    // Draw right face
    QPolygon rightFace({ rightTop, middle, bottom, rightBottom });
    painter.setBrush(rightColor);
    painter.drawPolygon(rightFace);
}
