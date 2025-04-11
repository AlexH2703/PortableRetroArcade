#include "GameController.h"
#include <QTimer>
#include <QScreen>
#include <QApplication>
#include <QKeyEvent>
#include <QDebug>

GameController::GameController(QWidget* parent)
    : QMainWindow(parent),
    qbert(nullptr),
    cubeWidget(nullptr),
    positionCheckTimer(new QTimer(this)),
    qbertAnimation(new QPropertyAnimation(this))
{
    ui.setupUi(this);
    this->setWindowState(Qt::WindowMaximized);

    cubeWidget = findChild<CubeWidget*>("widget");
    qbert = findChild<Qbert*>("qbert");
    if (cubeWidget) {
        cubeWidget->setPyramidSize(7);
        cubeWidget->arrangeCubes();
        cubeWidget->repaint();

        if (!cubeWidget->getCubes().isEmpty()) {
            Cube* topCube = cubeWidget->getCubes().first();
            QScreen* screen = QApplication::primaryScreen();
            QRect screenGeometry = screen->availableGeometry();
            screenWidth = screenGeometry.width();
            qbert->move(topCube->x() + screenWidth / 4, topCube->y() - qbert->height() / 2);
            qbert->show();
        }

        connect(positionCheckTimer, &QTimer::timeout, this, &GameController::checkQbertPosition);
        positionCheckTimer->start(16);
    }

    // Set up Qbert animation
    qbertAnimation->setTargetObject(qbert);
    qbertAnimation->setPropertyName("pos");
    qbertAnimation->setDuration(700);
    qbertAnimation->setEasingCurve(QEasingCurve::OutQuad); // Arc-like motion
}

GameController::~GameController()
{
}

void GameController::keyPressEvent(QKeyEvent* event) {
    if (!qbert) return;

    QString inputDirection;

    switch (event->key()) {
    case Qt::Key_J: inputDirection = "up_left"; break;
    case Qt::Key_K: inputDirection = "up_right"; break;
    case Qt::Key_M: inputDirection = "down_left"; break;
    case Qt::Key_Comma: inputDirection = "down_right"; break;
    case Qt::Key_Escape: close(); return;
    default: QMainWindow::keyPressEvent(event); return;
    }

    if (isJumping) {
        if (queuedDirection.isEmpty()) {
            queuedDirection = inputDirection;
        }
    }
    else {
        moveQbert(inputDirection);
    }
}

void GameController::moveQbert(const QString& direction) {
    if (!qbert) return;

    int stepX = screenWidth / 32;
    int stepY = screenWidth / 19.277;

    QPoint startPos = qbert->pos();
    QPoint endPos = startPos;

    if (direction == "up_left") {
        endPos.setX(endPos.x() - stepX);
        endPos.setY(endPos.y() - stepY);
    }
    else if (direction == "up_right") {
        endPos.setX(endPos.x() + stepX);
        endPos.setY(endPos.y() - stepY);
    }
    else if (direction == "down_left") {
        endPos.setX(endPos.x() - stepX);
        endPos.setY(endPos.y() + stepY);
    }
    else if (direction == "down_right") {
        endPos.setX(endPos.x() + stepX);
        endPos.setY(endPos.y() + stepY);
    }

    int duration = 600;

    QVariantAnimation* xAnim = new QVariantAnimation(this);
    QVariantAnimation* yAnim = new QVariantAnimation(this);

    xAnim->setStartValue(startPos.x());
    xAnim->setEndValue(endPos.x());
    xAnim->setDuration(duration);
    xAnim->setEasingCurve(QEasingCurve::Linear);

    yAnim->setStartValue(startPos.y());
    yAnim->setEndValue(endPos.y());
    yAnim->setDuration(duration);
    yAnim->setEasingCurve(endPos.y() < startPos.y() ? QEasingCurve::OutCubic : QEasingCurve::InCubic);

    // Sync movement
    connect(xAnim, &QVariantAnimation::valueChanged, this, [=](const QVariant& xVal) {
        qbert->move(xVal.toInt(), qbert->y());
        });

    connect(yAnim, &QVariantAnimation::valueChanged, this, [=](const QVariant& yVal) {
        qbert->move(qbert->x(), yVal.toInt());
        });

    // Use shared counter to track both animations finishing
    auto finishedCount = QSharedPointer<int>::create(0);

    auto onFinished = [=]() mutable {
        (*finishedCount)++;
        if (*finishedCount == 2) {
            isJumping = false;

            if (!queuedDirection.isEmpty()) {
                QString nextMove = queuedDirection;
                queuedDirection.clear();

                QTimer::singleShot(0, this, [=]() {
                    moveQbert(nextMove);
                    });
            }
        }
        };

    connect(xAnim, &QVariantAnimation::finished, this, onFinished);
    connect(yAnim, &QVariantAnimation::finished, this, onFinished);
    connect(xAnim, &QVariantAnimation::finished, xAnim, &QObject::deleteLater);
    connect(yAnim, &QVariantAnimation::finished, yAnim, &QObject::deleteLater);

    isJumping = true;
    xAnim->start();
    yAnim->start();
}


void GameController::checkQbertPosition() {
    if (!cubeWidget) return;
    qbert->raise();
    for (Cube* cube : cubeWidget->getCubes()) {
        QRect cubeTopFace(cube->x(), cube->y(), cube->width(), cube->depth());
        QPoint qbertPos = qbert->geometry().center();
        qbertPos.setX(qbertPos.x() - screenWidth / 4 - 43);
        qbertPos.setY(qbertPos.y() + 1);
        if (cubeTopFace.x() == qbertPos.x() && cubeTopFace.y()-qbertPos.y() < 5 && cubeTopFace.y()-qbertPos.y() >= 0) {
            // Handle interactions here
			cube->setTopFaceColor(QColor(255, 255, 255));
            break;
        }
    }
}
