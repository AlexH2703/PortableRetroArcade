#pragma once

#include <QtWidgets/QMainWindow>
#include <QPropertyAnimation>
#include <QTimer>
#include "ui_Qbert.h"
#include "CubeWidget.h"
#include "Qbert.h"

class GameController : public QMainWindow
{
    Q_OBJECT

public:
    GameController(QWidget* parent = nullptr);
    ~GameController();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void checkQbertPosition();

private:
    void moveQbert(const QString& direction);

    Ui::QbertClass ui;
    CubeWidget* cubeWidget;
    Qbert* qbert;
    QTimer* positionCheckTimer;
    QPropertyAnimation* qbertAnimation;
    int screenWidth;
    bool isJumping = false;
    QString queuedDirection;

};
