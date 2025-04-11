#include "CubeWidget.h"
#include <QScreen>
#include <QApplication>

CubeWidget::CubeWidget(QWidget* parent) :
    QWidget(parent),
    pyramidSize(7)
{}

void CubeWidget::setPyramidSize(int size) {
    pyramidSize = size;
}

void CubeWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    QScreen* screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    this->setFixedSize(screenWidth / 2, screenHeight);
    this->move(screenWidth / 4, 0);
}


void CubeWidget::arrangeCubes() {
    // Clear existing cubes
	resizeEvent(nullptr);
    qDeleteAll(cubes);
    cubes.clear();

    // Define base cube size
    int cubeWidth = this->width()/8;
	int cubeHeight = this->width() / 9.5;
    int depth = this->width()/26.5;

    // Start drawing pyramid from top
    int startY = this->height() /6;

    // Loop through rows of the pyramid
    for (int row = 0; row < pyramidSize; row++) {
        int startX = this->width() / 2 - (row * cubeWidth/2);

        for (int col = 0; col <= row; col++) {
            int x = startX + (col * cubeWidth);
            int y = startY + row * (cubeWidth-depth/1.8);

            Cube* cube = new Cube(this, cubeWidth, cubeHeight, depth);
            cube->move(x, y);
            cube->lower();
            cube->show();
            cubes.append(cube);
        }
    }
}
