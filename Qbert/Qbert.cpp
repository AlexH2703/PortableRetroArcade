#include "Qbert.h"
#include <QPainter>
#include <QDebug>
#include <QScreen>
#include <QApplication>

Qbert::Qbert(QWidget* parent) : QWidget(parent) {
    if (qbertImage.load(":/qbert.png")) {
        qbertImage.load(":/qbert.png");
	}
    else {
        qDebug() << "Failed to load Qbert image.";
    }
    QScreen* screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    int screenWidth = screenGeometry.width();
    setScale(screenWidth/24, screenWidth/24);
}


void Qbert::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, scaledQbertImage);
}

void Qbert::setScale(int width, int height) {
    scaledQbertImage = qbertImage.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setFixedSize(scaledQbertImage.size());
    update();
}

QRect Qbert::collisionRect() {
    int hitboxWidth = this->width() * 0.983;
    int hitboxHeight = this->height() * 0.909;
    int offsetX = (width() - hitboxWidth) / 2;
    int offsetY = (height() - hitboxHeight) / 2;
    return QRect(offsetX, offsetY, hitboxWidth, hitboxHeight);
}

