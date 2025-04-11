#include "Qbert.h"
#include <QPainter>
#include <QDebug>
#include <QScreen>
#include <QApplication>

Qbert::Qbert(QWidget* parent) : QWidget(parent) {
    if (qbertImage.load(":/Qbert/qbert.png")) {
        qbertImage.load(":/Qbert/qbert.png");
	}
    else {
        qDebug() << "Failed to load Qbert image.";
    }
    QScreen* screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    int screenWidth = screenGeometry.width();
    setScale(screenWidth/18, screenWidth/18);
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
