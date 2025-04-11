#include "Cube.h"

Cube::Cube(QWidget* parent, int cW, int cH, int d) : QWidget(parent) {
    setFixedSize(cW*2, cH*2); // Set a fixed size for each cube
	cubeWidth = cW;
    cubeHeight = cH;
	cubeDepth = d;
	topFaceColor = QColor(255, 0, 0); // Default top face color (red)
}

void Cube::setTopFaceColor(const QColor& color) {
	topFaceColor = color;
	update(); // Trigger a repaint to apply the new color
}

void Cube::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    
    // Colors for different cube faces
    QColor leftColor(0, 200, 0); // Green
    QColor rightColor(0, 0, 200); // Blue

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
