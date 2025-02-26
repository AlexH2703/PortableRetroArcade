//#include "PacQt.h"
//#include <QFileDialog>
//#include <QPainter>
//#include <QLabel>
//#include <QGridLayout>
//#include <QDebug>
//#include "Ghost.h"
//
//PacQt::PacQt(QWidget* parent)
//    : QMainWindow(parent),
//    scene(new QGraphicsScene(this)),
//    graphicsView(new QGraphicsView(this)),
//    timer(new QTimer(this)),
//    lives(3),
//    score(0),
//    level(1),
//    frenzyMode(false),
//    pacmanSpeed(2),
//    ghostsEaten(0),
//    pacmanDirection(0, 0),
//    nextDirection(0, 0),
//    level1(),
//    pacman(nullptr),
//    gameStarted(false), // Track whether the game has started
//    blinky(new Blinky(scene, level1)),
//    pinky(new Pinky(scene, level1)),
//    inky(new Inky(scene, level1)),
//    clyde(new Clyde(scene, level1))
//{
//    ui.setupUi(this);
//    this->setWindowState(Qt::WindowMaximized);
//
//
//    graphicsView->setFixedSize(1216, 896); // Set fixed size of graphics view
//    graphicsView->setScene(scene);
//    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); 
//
//    // Install event filter to prevent arrow key panning
//    graphicsView->installEventFilter(this);
//
//    // Ensure Level1 is initialized correctly
//    level1 = Level1();
//    updateGridDisplay(); 
//
//    initLabels();
//
//    // The game will start only after the resize event completes
//    this->show();
//}
//
//void PacQt::initLabels() {
//    QFontDatabase fontDatabase;
//    int fontId = fontDatabase.addApplicationFont(":/PacQt/PressStart2P-Regular.ttf");
//    scoreLabel = new QLabel(this);
//    scoreLabel->setText(QString("Score:\n %1").arg(score));
//    QFont font("Press Start 2P", 36, QFont::Bold);
//    scoreLabel->setFont(font);
//    scoreLabel->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 0.5);");
//    scoreLabel->setFixedSize(300, 110);
//    scoreLabel->move(150, 100); // Position in the top-left corner
//    scoreLabel->show();
//
//    levelLabel = new QLabel(this);
//    levelLabel->setText(QString("Level:\n %1").arg(level));
//    levelLabel->setFont(font);
//    levelLabel->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 0.5);");
//    levelLabel->setFixedSize(300, 110);
//    levelLabel->move(150, 210);
//    levelLabel->show();
//
//    livesLabel = new QLabel(this);
//    livesLabel->setText(QString("Lives:\n %1").arg(lives));
//    livesLabel->setFont(font);
//    livesLabel->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 0.5);");
//    livesLabel->setFixedSize(300, 110);
//    livesLabel->move(150, 320);
//    livesLabel->show();
//
//    resumeLabel = new QLabel(this);
//    resumeLabel->setText(QString("Please Press\n'R' to Start\nor Resume"));
//    resumeLabel->setFont(QFont("Press Start 2P", 26, QFont::Bold));
//    resumeLabel->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 0.5);");
//    resumeLabel->setFixedSize(450, 300);
//    resumeLabel->move(704, 350);
//    resumeLabel->show();
//
//    controlsLabel = new QLabel(this);
//    controlsLabel->setText(
//        "<html><body style='color: white; background-color: rgba(0, 0, 0, 0.5);'>"
//        "Controls:<br>"
//        "<table>"
//        "<tr style='line-height: 1.5;'><td>'\u2191' - Up</td></tr>"
//        "<tr style='line-height: 1.5;'><td>'\u2193' - Down</td></tr>"
//        "<tr style='line-height: 1.5;'><td>'\u2190' - Left</td></tr>"
//        "<tr style='line-height: 1.5;'><td>'\u2192' - Right</td></tr>"
//        "</table><br><br>"
//        "'R' - Resume/Start<br>"
//        "'P' - Pause<br>"
//        "'ESC' - Main Menu"
//        "</body></html>"
//    );
//    controlsLabel->setFont(QFont("Press Start 2P", 17, QFont::Bold));
//    controlsLabel->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 0.5);");
//    controlsLabel->setFixedSize(500, 300);
//    controlsLabel->move(1400, 200);
//    controlsLabel->show();
//
//    gameOverLabel = new QLabel(this);
//    gameOverLabel->setText("GAME OVER");
//    gameOverLabel->setFont(QFont("Press Start 2P", 54, QFont::Bold));
//    gameOverLabel->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 0.5);");
//    gameOverLabel->setFixedSize(650, 110);
//    gameOverLabel->move(610, 200);
//    gameOverLabel->hide();
//}
//
//void PacQt::initPacmanLocation() {
//    if (!pacman) {
//        pacman = new QGraphicsPixmapItem(QPixmap(":/PacQt/pacman.png"));
//        pacman->setPos(416, 736);
//        pacman->setZValue(10);
//        pacman->setTransformOriginPoint(pacman->boundingRect().center());
//        scene->addItem(pacman); // Ensure Pacman is added to the scene
//    }
//}
//
//QPointF& PacQt::getPacmanDirection() {
//    return pacmanDirection;
//}
//
//void PacQt::initGhostLocation() {
//    blinky->setPos(14 * 32, 11 * 32);
//    blinky->setZValue(3);
//    pinky->setPos(16 * 32, 13 * 32);
//    pinky->setZValue(3);
//    inky->setPos(11 * 32, 15 * 32);
//    inky->setZValue(3);
//    clyde->setPos(16 * 32, 15 * 32);
//    clyde->setZValue(3);
//    scene->addItem(blinky);
//    scene->addItem(pinky);
//    scene->addItem(inky);
//    scene->addItem(clyde);
//}
//
//void PacQt::resetPacmanLocation() {
//    pacman->setPos(416, 736);
//    pacmanOldLocation = pacman->pos();
//    pacmanDirection.setX(0);
//    pacmanDirection.setY(0);
//    nextDirection.setX(0);
//    nextDirection.setY(0);
//}
//
//void PacQt::handlePacmanOrientation() {
//    int angle = 0; // Default angle
//    pacman->setTransform(QTransform()); //reset transform
//    if (pacmanDirection == QPointF(0, -1)) {
//        angle = 270; // Up
//    }
//    else if (pacmanDirection == QPointF(-1, 0)) { //Left
//        QTransform flipTransform;
//        flipTransform.translate(pacman->boundingRect().width() / 2.0, 0); // Center the flip
//        flipTransform.scale(-1, 1);                                      // Horizontal flip
//        flipTransform.translate(-pacman->boundingRect().width() / 2.0, 0);
//        pacman->setTransform(flipTransform);
//    }
//    else if (pacmanDirection == QPointF(0, 1)) {
//        angle = 90; // Down
//    }
//    // No need to check (1, 0) as it's the default angle
//
//    pacman->setRotation(angle);
//}
//
//void PacQt::clearSceneExcept(vector<QGraphicsItem*> itemsToKeep) {
//    // Get all items currently in the scene
//    QList<QGraphicsItem*> itemsInScene = scene->items();
//
//    // Iterate through each item
//    for (QGraphicsItem* item : itemsInScene) {
//        // If the item is not the one we want to keep, remove it
//        if (std::find(itemsToKeep.begin(), itemsToKeep.end(), item) == itemsToKeep.end()) {
//            scene->removeItem(item);
//        }
//    }
//}
//
//void PacQt::resizeEvent(QResizeEvent* event) {
//    QMainWindow::resizeEvent(event);
//
//    // Resize graphics view to fit the entire window
//    graphicsView->setFixedSize(this->size());
//
//    // Calculate new width and height for the scene
//    int newWidth = this->width()/2;
//    int newHeight = this->height();
//
//    // Set the scene rectangle to the new dimensions
//    scene->setSceneRect(0, 0, newWidth, newHeight);
//
//    // Ensure Pacman initialization and game start happens after the resize is complete
//    if (!gameStarted) {
//        gameStarted = true; // Mark that the game is starting
//        QTimer::singleShot(0, this, &PacQt::startGameAfterResize); // Delay game start
//    }
//}
//
//void PacQt::startGameAfterResize() {
//    initPacmanLocation(); // Initialize Pacman after resize is done
//    initGhostLocation();
//
//    // Start the game update cycle
//    connect(timer, &QTimer::timeout, this, &PacQt::updateGame);
//    timer->start(16); // Approximately 60 FPS
//    timer->stop();
//}
//
//PacQt::~PacQt() {}
#include "PacQt.h"
#include <QFileDialog>
#include <QPainter>
#include <QLabel>
#include <QGridLayout>
#include <QDebug>
#include "Ghost.h"

PacQt::PacQt(QWidget* parent)
    : QMainWindow(parent),
    scene(new QGraphicsScene(this)),
    graphicsView(new QGraphicsView(this)),
    timer(new QTimer(this)),
    lives(3),
    score(0),
    level(1),
    frenzyMode(false),
    pacmanSpeed(2),
    ghostsEaten(0),
    pacmanDirection(0, 0),
    nextDirection(0, 0),
    level1(),
    pacman(nullptr),
    gameStarted(false), // Track whether the game has started
    itemSize(this->height() / 16),
    blinky(),
    pinky(),
    inky(),
    clyde()
{
    ui.setupUi(this);
    this->setWindowState(Qt::WindowMaximized);

    graphicsView->setFixedSize(1216, 896); // Set fixed size of graphics view
    graphicsView->setScene(scene);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Install event filter to prevent arrow key panning
    graphicsView->installEventFilter(this);

    blinky = new Blinky(scene, level1, itemSize);
    pinky = new Pinky(scene, level1, itemSize);
    inky = new Inky(scene, level1, itemSize);
    clyde = new Clyde(scene, level1, itemSize);

    // Ensure Level1 is initialized correctly
    level1 = Level1();
    updateGridDisplay();

    initLabels();

    // The game will start only after the resize event completes
    this->show();
}

void PacQt::initLabels() {
    // Update label sizes and positions based on new window size
    int screenWidth = this->width();
    int screenHeight = this->height();

    // Dynamic font
    int font1Height = screenHeight * 0.08; // 8%
    int fontResumeHeight = screenHeight * 0.06;
    int fontControlsHeight = screenHeight * 0.04;
    int fontGameOverHeight = screenHeight * 0.08;

    QFontDatabase fontDatabase;
    int fontId = fontDatabase.addApplicationFont(":/PacQt/PressStart2P-Regular.ttf");
    scoreLabel = new QLabel(this);
    scoreLabel->setText(QString("Score:\n %1").arg(score));
    QFont font("Press Start 2P", font1Height, QFont::Bold);
    scoreLabel->setFont(font);
    scoreLabel->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 0.5);");
    /*scoreLabel->setFixedSize(300, 110);
    scoreLabel->move(150, 100);*/ // Position in the top-left corner
    scoreLabel->setFixedSize(screenWidth * 0.45, screenHeight * 0.25);
    scoreLabel->move(screenWidth * 0.08, screenHeight * 0.18);
    scoreLabel->show();

    levelLabel = new QLabel(this);
    levelLabel->setText(QString("Level:\n %1").arg(level));
    levelLabel->setFont(font);
    levelLabel->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 0.5);");
    /*levelLabel->setFixedSize(300, 110);
    levelLabel->move(150, 210);*/
    levelLabel->setFixedSize(screenWidth * 0.45, screenHeight * 0.25);
    levelLabel->move(screenWidth * 0.08, screenHeight * 0.43);
    levelLabel->show();

    livesLabel = new QLabel(this);
    livesLabel->setText(QString("Lives:\n %1").arg(lives));
    livesLabel->setFont(font);
    livesLabel->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 0.5);");
    /*livesLabel->setFixedSize(300, 110);
    livesLabel->move(150, 320);*/
    livesLabel->setFixedSize(screenWidth * 0.45, screenHeight * 0.25);
    livesLabel->move(screenWidth * 0.08, screenHeight * 0.68);
    livesLabel->show();

    resumeLabel = new QLabel(this);
    resumeLabel->setText(QString("Please Press\n'R' to Start\nor Resume"));
    resumeLabel->setFont(QFont("Press Start 2P", fontResumeHeight, QFont::Bold));
    resumeLabel->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 0.5);");
    /*resumeLabel->setFixedSize(450, 300);
    resumeLabel->move(704, 350);*/
    resumeLabel->setFixedSize(screenWidth * 0.7, screenHeight * 0.35);
    resumeLabel->move(screenWidth*0.95, screenHeight);
    resumeLabel->show();

    controlsLabel = new QLabel(this);
    controlsLabel->setText(
        "<html><body style='color: white; background-color: rgba(0, 0, 0, 0.5);'>"
        "Controls:<br>"
        "<table>"
        "<tr style='line-height: 1.5;'><td>'\u2191' - Up</td></tr>"
        "<tr style='line-height: 1.5;'><td>'\u2193' - Down</td></tr>"
        "<tr style='line-height: 1.5;'><td>'\u2190' - Left</td></tr>"
        "<tr style='line-height: 1.5;'><td>'\u2192' - Right</td></tr>"
        "</table><br><br>"
        "'R' - Resume/Start<br>"
        "'P' - Pause<br>"
        "'ESC' - Main Menu"
        "</body></html>"
    );
    controlsLabel->setFont(QFont("Press Start 2P", fontControlsHeight, QFont::Bold));
    controlsLabel->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 0.5);");
    /*controlsLabel->setFixedSize(500, 300);
    controlsLabel->move(1400, 200);*/
    controlsLabel->setFixedSize(screenWidth * 0.65, screenHeight * 0.7);
    controlsLabel->move(screenWidth * 2, screenHeight * 0.18);
    controlsLabel->show();

    gameOverLabel = new QLabel(this);
    gameOverLabel->setText("GAME OVER");
    gameOverLabel->setFont(QFont("Press Start 2P", fontGameOverHeight, QFont::Bold));
    gameOverLabel->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 0.5);");
    /*gameOverLabel->setFixedSize(650, 110);
    gameOverLabel->move(610, 200);*/
    gameOverLabel->setFixedSize(screenWidth * 0.7, screenHeight * 0.3);
    gameOverLabel->move(screenWidth*.95, screenHeight * 0.25);
    gameOverLabel->hide();
}

void PacQt::initPacmanLocation() {
    if (!pacman) {
        pacman = new QGraphicsPixmapItem(QPixmap(":/PacQt/pacman.png"));
        pacman->setPos(13 * itemSize, 23 * itemSize);
        pacman->setZValue(10);
        pacman->setTransformOriginPoint(pacman->boundingRect().center());
        pacman->setScale((itemSize) / 36);
        scene->addItem(pacman); // Ensure Pacman is added to the scene
    }
}

QPointF& PacQt::getPacmanDirection() {
    return pacmanDirection;
}

void PacQt::initGhostLocation() {
    blinky->setPos(14 * itemSize, 11 * itemSize);
    blinky->setZValue(3);
    pinky->setPos(16 * itemSize, 13 * itemSize);
    pinky->setZValue(3);
    inky->setPos(11 * itemSize, 15 * itemSize);
    inky->setZValue(3);
    clyde->setPos(16 * itemSize, 15 * itemSize);
    clyde->setZValue(3);
    scene->addItem(blinky);
    scene->addItem(pinky);
    scene->addItem(inky);
    scene->addItem(clyde);
}

void PacQt::resetPacmanLocation() {
    pacman->setPos(13 * itemSize, 23 * itemSize);
    pacmanOldLocation = pacman->pos();
    pacmanDirection.setX(0);
    pacmanDirection.setY(0);
    nextDirection.setX(0);
    nextDirection.setY(0);
}

void PacQt::handlePacmanOrientation() {
    int angle = 0; // Default angle
    pacman->setTransform(QTransform()); //reset transform
    if (pacmanDirection == QPointF(0, -1)) {
        angle = 270; // Up
    }
    else if (pacmanDirection == QPointF(-1, 0)) { //Left
        QTransform flipTransform;
        flipTransform.translate(pacman->boundingRect().width() / 2.0, 0); // Center the flip
        flipTransform.scale(-1, 1);                                      // Horizontal flip
        flipTransform.translate(-pacman->boundingRect().width() / 2.0, 0);
        pacman->setTransform(flipTransform);
    }
    else if (pacmanDirection == QPointF(0, 1)) {
        angle = 90; // Down
    }
    // No need to check (1, 0) as it's the default angle

    pacman->setRotation(angle);
}

void PacQt::clearSceneExcept(vector<QGraphicsItem*> itemsToKeep) {
    // Get all items currently in the scene
    QList<QGraphicsItem*> itemsInScene = scene->items();

    // Iterate through each item
    for (QGraphicsItem* item : itemsInScene) {
        // If the item is not the one we want to keep, remove it
        if (std::find(itemsToKeep.begin(), itemsToKeep.end(), item) == itemsToKeep.end()) {
            scene->removeItem(item);
        }
    }
}

void PacQt::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);

    // Resize graphics view to fit the entire window
    graphicsView->setFixedSize(this->size());

    // Calculate new width and height for the scene
    int newWidth = this->width() / 1.75;
    int newHeight = this->height();

    // Set the scene rectangle to the new dimensions
    scene->setSceneRect(0, 0, newWidth, newHeight);

    // Ensure Pacman initialization and game start happens after the resize is complete
    if (!gameStarted) {
        gameStarted = true; // Mark that the game is starting
        QTimer::singleShot(0, this, &PacQt::startGameAfterResize); // Delay game start
    }
}

void PacQt::startGameAfterResize() {
    initPacmanLocation(); // Initialize Pacman after resize is done
    initGhostLocation();

    // Start the game update cycle
    connect(timer, &QTimer::timeout, this, &PacQt::updateGame);
    timer->start(16); // Approximately 60 FPS
    timer->stop();
}

PacQt::~PacQt() {}

bool PacQt::eventFilter(QObject* obj, QEvent* event) {
    if (obj == graphicsView && event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Up || keyEvent->key() == Qt::Key_Down ||
            keyEvent->key() == Qt::Key_Left || keyEvent->key() == Qt::Key_Right) {
            keyPressEvent(keyEvent);
            return true; // Block the arrow key event from affecting the view
        }
    }
    // Pass the event on to the parent class
    return QMainWindow::eventFilter(obj, event);
}

void PacQt::keyPressEvent(QKeyEvent* event) {
    if (!event) {
        qDebug() << "Error: KeyEvent is null!";
        return;
    }

    if (event->key() == Qt::Key_Up) {
        nextDirection = QPointF(0, -1);
    }
    else if (event->key() == Qt::Key_Down) {
        nextDirection = QPointF(0, 1);
    }
    else if (event->key() == Qt::Key_Left) {
        nextDirection = QPointF(-1, 0);
    }
    else if (event->key() == Qt::Key_Right) {
        nextDirection = QPointF(1, 0);
    }
    else if (event->key() == Qt::Key_P) {
        timer->stop();
        resumeLabel->show();
    }
    else if (event->key() == Qt::Key_R) {
        timer->start();
        resumeLabel->hide();
        gameOverLabel->hide();
    }
    else if (event->key() == Qt::Key_Escape) {
        timer->stop();
        delete blinky;
        delete pinky;
        delete inky;
        delete clyde;
        delete scene;
        delete graphicsView;
        delete timer;
        close();
    }
}

void PacQt::updateGridDisplay() {
    // Clear existing items before updating
    clearSceneExcept({pacman, blinky, pinky, inky, clyde}); // Clear previous items

    // Add objects based on level data
    for (int i = 0; i <= 30; i++) {
        for (int j = 0; j <= 27; j++) {
            Object obj = level1.getGrid().getObject(i, j);
            QString objectType = QString::fromStdString(obj.getType());

            // Add graphical representations for other objects
            if (objectType == "pellet") {
                QGraphicsPixmapItem* pellet = new QGraphicsPixmapItem(QPixmap(":/PacQt/pellet.png"));
                pellet->setPos(j * itemSize, i * itemSize); // Adjust position as needed
                pellet->setZValue(1);
                scene->addItem(pellet);
            }
            else if (objectType == "superPellet") {
                QGraphicsPixmapItem* superPellet = new QGraphicsPixmapItem(QPixmap(":/PacQt/superPellet.png"));
                superPellet->setPos(j * itemSize, i * itemSize); // Adjust position as needed
                superPellet->setZValue(2);
                scene->addItem(superPellet);
            }
            else if (objectType == "wall") {
                QGraphicsPixmapItem* wall = new QGraphicsPixmapItem(QPixmap(":/PacQt/wall.png"));
                wall->setPos(j * itemSize, i * itemSize); // Adjust position as needed
                wall->setZValue(0);
                scene->addItem(wall);
            }
        }
    }
}

bool PacQt::collidedInDirection(QPointF dir) {
    pacman->setPos(dir);
    QRectF pacmanBoundingRect = pacman->boundingRect();
    QList<QGraphicsItem*> collidingItems = scene->items(pacman->mapToScene(pacmanBoundingRect));

    // Check for collisions with walls
    for (QGraphicsItem* item : collidingItems) {
        if (item->zValue() == 0) { // Wall has zValue 0
            return true;
        }
    }
    return false;
}

void PacQt::enterFrenzyMode() {
    // Set the ghosts to frenzy images
    if (level1.getFrenzyTime() == 0) {
        blinky->setPixmap(QPixmap(":/PacQt/frenzyGhost.png"));
        pinky->setPixmap(QPixmap(":/PacQt/frenzyGhost.png"));
        inky->setPixmap(QPixmap(":/PacQt/frenzyGhost.png"));
        clyde->setPixmap(QPixmap(":/PacQt/frenzyGhost.png"));

        blinky->setSpeed(1);
        pinky->setSpeed(1);
        inky->setSpeed(1);
        clyde->setSpeed(1);

        // Change their mode to scatter
        blinky->setMode("scatter");
        pinky->setMode("scatter");
        inky->setMode("scatter");
        clyde->setMode("scatter");
    }
    else if (level1.getFrenzyTime() >= 8000) {
        blinky->setPixmap(QPixmap(":/PacQt/blinky.png"));
        pinky->setPixmap(QPixmap(":/PacQt/pinky.png"));
        inky->setPixmap(QPixmap(":/PacQt/inky.png"));
        clyde->setPixmap(QPixmap(":/PacQt/clyde.png"));

        if (!blinky->isInHouse()) {
            blinky->setSpeed(2);
        }
        if (!pinky->isInHouse()) {
            pinky->setSpeed(2);
        }
        if (!inky->isInHouse()) {
            inky->setSpeed(2);
        }
        if (!clyde->isInHouse()) {
            clyde->setSpeed(2);
        }

        // Set their mode back to chase
        if (blinky->getMode() != "home"){
            blinky->setMode("chase");
        }
        if (pinky->getMode() != "home") {
            pinky->setMode("chase");
        }
        if (inky->getMode() != "home") {
            inky->setMode("chase");
        }
        if (clyde->getMode() != "home") {
            clyde->setMode("chase");
        }
        level1.setFrenzyTime(0 - timer->interval());
        ghostsEaten = 0;
        frenzyMode = false;

    }
    level1.setFrenzyTime(level1.getFrenzyTime() + timer->interval());
}

void PacQt::handleCollisions() {
    // Calculate pacman's intended new position based on the new direction
    QPointF intendedNewPos = pacman->pos() + nextDirection * pacmanSpeed;
    
    // Checks for walls in the direction pacman is trying to move
    if (!collidedInDirection(intendedNewPos)) {
        // If there's no collision, update pacman's direction
        pacmanDirection = nextDirection;
    }
    else {
        //First pull pacman out of the wall
        QPointF revertPos = pacman->pos() + nextDirection * -pacmanSpeed;
        pacman->setPos(revertPos);
        //Try to move in the current direction
        QPointF newIntendedPos = pacman->pos() + pacmanDirection * pacmanSpeed;
        if (collidedInDirection(newIntendedPos)) { //check wall collision again
            //If there was a collision pull pacman out of this wall
            QPointF revertNewPos = pacman->pos() + pacmanDirection * -pacmanSpeed;
            pacman->setPos(revertNewPos);
        }
    }

    // Handle item collisions (like pellets) after moving
    QList<QGraphicsItem*> collidingItems = pacman->collidingItems(); // Re-check at new position

    for (QGraphicsItem* item : collidingItems) {
        if (item->zValue() == 1) { // Pellets have zValue 1
            level1.getGrid().getObject(item->y() / itemSize, item->x() / itemSize).setType("blank");
            scene->removeItem(item); // Remove the pellet from the scene
            pelletsEaten++;
            score += 10;
            shouldLevelUp = level1.getGrid().ateAllPellets();
        }
        else if (item->zValue() == 2) {     //superPellets have zValue 2
            level1.getGrid().getObject(item->y() / itemSize, item->x() / itemSize).setType("blank");
            scene->removeItem(item); // Remove the pellet from the scene
            pelletsEaten++;
            score += 50;
            shouldLevelUp = level1.getGrid().ateAllPellets();
            frenzyMode = true;
            level1.setFrenzyTime(0);
        }
        else if (item->zValue() == 3) {     //Ghosts have zValue 3
            if (frenzyMode) {
                Ghost* ghost = dynamic_cast<Ghost*>(item);
                if (ghost && (ghost->pixmap().cacheKey() == QPixmap(":/PacQt/frenzyGhost.png").cacheKey()) && ghost->getMode() != "home") {
                    ghost->setMode("home");
                    ghostsEaten++;
                    score += 100 * ghostsEaten;
                }
                else if (ghost->getMode() != "home"){
                    lives--;
                    livesLabel->setText(QString("Lives:\n %1").arg(lives));
                    resetBoard();
                }
            }
            else {
                lives--;
                livesLabel->setText(QString("Lives:\n %1").arg(lives));
                resetBoard();
            }
            
        }
    }

    if (frenzyMode) {
        enterFrenzyMode();
    }
    if (blinky->isInHouse()) {
        if (blinky->getSpeed() != 2) {
            blinky->setSpeed(2);
        }
        blinky->setPixmap(QPixmap(":/PacQt/blinky.png"));
        blinky->setMode("chase");
        blinky->leaveHouse();
    }
    if (pinky->isInHouse() && frenzyMode) {
        pinky->setSpeed(2);
        pinky->setPixmap(QPixmap(":/PacQt/pinky.png"));
        pinky->setMode("chase");
        pinky->leaveHouse();
    }
    else if (pinky->isInHouse() && pelletsEaten > 0) {
        pinky->setSpeed(2);
        pinky->leaveHouse();
    }

    if (inky->isInHouse() && frenzyMode && pelletsEaten > 30) {
        inky->setSpeed(2);
        inky->setPixmap(QPixmap(":/PacQt/inky.png"));
        inky->setMode("chase");
        inky->leaveHouse();
    }
    else if (inky->isInHouse() && pelletsEaten > 30) {
        inky->setSpeed(2);
        inky->leaveHouse();
    }

    if (clyde->isInHouse() && frenzyMode && pelletsEaten > 60) {
        clyde->setSpeed(2);
        clyde->setPixmap(QPixmap(":/PacQt/clyde.png"));
        clyde->setMode("chase");
        clyde->leaveHouse();
    }
    else if (clyde->isInHouse() && pelletsEaten > 90) {
        clyde->setSpeed(2);
        clyde->leaveHouse();
    }
    
    if (floor(pacman->pos().y()) == 14*itemSize && pacman->pos().x() == 2.0) {
        pacman->setPos(27 * itemSize - 2, 14*itemSize);
    }
    else if (floor(pacman->pos().y()) == 14*itemSize && pacman->pos().x() == 27 * itemSize - 2.0) {
        pacman->setPos(2, 14*itemSize);
    }
}

void PacQt::resetBoard() {
    timer->stop();
    resumeLabel->show();
    updateGridDisplay();
    resetPacmanLocation();
    blinky->resetGhost();
    pinky->resetGhost();
    pinky->enterHouse();
    inky->resetGhost();
    inky->enterHouse();
    clyde->resetGhost();
    clyde->enterHouse();
    blinky->setPixmap(QPixmap(":/PacQt/blinky.png"));
    pinky->setPixmap(QPixmap(":/PacQt/pinky.png"));
    inky->setPixmap(QPixmap(":/PacQt/inky.png"));
    clyde->setPixmap(QPixmap(":/PacQt/clyde.png"));
    ghostsEaten = 0;
    pelletsEaten = 0;
    frenzyMode = false;
    level1.setFrenzyTime(0);

}

void PacQt::updateGame() {
    if (shouldLevelUp) {
        level++;
        levelLabel->setText(QString("Level:\n %1").arg(level));
        level1 = Level1();
        resetBoard();
        shouldLevelUp = false;
    }

    handleCollisions();        //Handles collisions such as Pacman with walls and pellets
    handlePacmanOrientation();
    scoreLabel->setText(QString("Score:\n %1").arg(score));
    
    if (!(lives > 0)) {
        gameOverLabel->show();
        lives = 3;
        livesLabel->setText(QString("Lives:\n %1").arg(lives));
        score = 0;
        scoreLabel->setText(QString("Score:\n %1").arg(score));
        level = 0;
        levelLabel->setText(QString("Level:\n %1").arg(level));
        level1 = Level1();
        resetBoard();
        
    }

    if (!blinky->isInHouse()) {
        blinky->updateMovement(pacman->pos());
    }
    if (!pinky->isInHouse()) {
        pinky->updateMovement(pacman->pos(), pacmanDirection);
    }
    if (!inky->isInHouse()) {
        inky->updateMovement(pacman->pos(), pacmanDirection, blinky->pos());
    }
    if (!clyde->isInHouse()) {
        clyde->updateMovement(pacman->pos());
    }
}