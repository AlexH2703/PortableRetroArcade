#include "Disc.h"
#include <QPainter>
#include <QDebug>
#include <QScreen>
#include <QApplication>

Disc::Disc(QWidget* parent, int rw, const QString& sid) : QWidget(parent) {
    if (discImage.load(":/disk.png")) {
        discImage.load(":/disk.png");
    }
    else {
        qDebug() << "Failed to load Disc image.";
    }
    QScreen* screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    int screenWidth = screenGeometry.width();
    setScale(screenWidth / 20, screenWidth / 20); // Adjust scale as needed
	row = rw;
	side = sid;
}

int Disc::getRow() const { return row; }
QString Disc::getSide() const { return side; }

void Disc::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, scaledDiscImage);
}

void Disc::setScale(int width, int height) {
    scaledDiscImage = discImage.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setFixedSize(scaledDiscImage.size());
    update();
}
