#ifndef REDBALL_H
#define REDBALL_H

#include "Enemy.h"
#include <QPixmap>
#include <QTimer>
#include <QApplication>
#include <QScreen>
#include <QPainter>

class RedBall : public Enemy {
    Q_OBJECT

public:
    explicit RedBall(QWidget* parent = nullptr);

    void setScale(int width, int height) override;
    void setDirection(const QString& direction) override;
    QString getDirection() const override;
    void setHorizonVelo(double d) override;
    double getHorizonVelo() const override;
    void startMoving();
    void paintEvent(QPaintEvent* event) override;

private:
    QPixmap redBallImage;
    QPixmap scaledRedBallImage;
    QString direction;
    double horizontalVelo;
};

#endif // REDBALL_H
