#ifndef PACQT_H
#define PACQT_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QLabel>
#include <QFont>
#include <QFontDatabase>
#include <QKeyEvent>
#include "Level1.h"
#include "ui_PacQt.h"
#include "Ghost.h"

class PacQt : public QMainWindow {
    Q_OBJECT

public:
    explicit PacQt(QWidget* parent = nullptr);
    ~PacQt() override;
    QPointF& getPacmanDirection();

protected:
    void resizeEvent(QResizeEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event);

private slots:
    void updateGridDisplay();
    void updateGame(); // Timer slot to update Pacman's position
    void initPacmanLocation();
    void initGhostLocation();
    void resetPacmanLocation();
    void handlePacmanOrientation();
    void startGameAfterResize();
    void clearSceneExcept(vector<QGraphicsItem*> itemsToKeep);
    void handleCollisions();
    bool collidedInDirection(QPointF dir);
    void enterFrenzyMode();
    void resetBoard();
    void initLabels();


private:
    Ui::PacQtClass ui;
    QGraphicsView* graphicsView; // Graphics view for rendering
    QGraphicsScene* scene; // Graphics scene
    QTimer* timer; // Timer for smooth updates
    Level1 level1; // Your level object
    QGraphicsPixmapItem* pacman;// Pacman graphics item
    QLabel* scoreLabel;
    QLabel* levelLabel;
    QLabel* livesLabel;
    QLabel* resumeLabel;
    QLabel* controlsLabel;
    QLabel* gameOverLabel;
    Blinky* blinky;
    Pinky* pinky;
    Inky* inky;
    Clyde* clyde;
    QPointF pacmanDirection; // Direction of Pacman's movement
    QPointF pacmanOldLocation;
    QPointF nextDirection;
    int lives;
    int score;
    int level;
    int distanceMoved;
    int ghostsEaten;
    const qreal pacmanSpeed; // Speed of Pacman
    bool gameStarted;
    bool shouldLevelUp;
    int pelletsEaten;
    bool frenzyMode;
    float itemSize;
};

#endif // PACQT_H
