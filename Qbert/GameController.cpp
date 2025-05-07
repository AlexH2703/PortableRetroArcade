#include "GameController.h"
#include <QTimer>
#include <QScreen>
#include <QApplication>
#include <QKeyEvent>
#include <QDebug>
#include <QFontDatabase>
#include <QQueue>
#include <QPointer>
#include <QInputDialog>

// Define the spawn pattern for each round
struct SpawnPattern {
    QVector<QString> pattern; // Sequence of enemies to spawn (e.g., {"C", "R", "R", "R"})
    int interval;             // Time interval between spawns in milliseconds
};

// Add a map to store spawn patterns for each round
QMap<int, SpawnPattern> spawnPatterns = {
    {1, {{"C", "R"}, 5000}}
};

// Add a queue to track the current spawn sequence
QQueue<QString> currentSpawnQueue;

GameController::GameController(QWidget* parent)
    : QMainWindow(parent),
    qbert(nullptr),
    cubeWidget(nullptr),
    isJumping(false),
	pausedLabel(new QLabel(this)),
    controlsLabel(new QLabel(this)),
	gameInfoLabel(new QLabel(this)),
	gameOverLabel(new QLabel(this)),
	highScoreLabel(new QLabel(this)),
	currentCube(nullptr)
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
			screenHeight = screenGeometry.height();
            qbert->move(topCube->x() + screenWidth / 4 + qbert->width()/4.5, topCube->y() - qbert->height() / 2);
            qbert->show();
        }

        // Periodically check Qbert's position
        positionCheckTimer = new QTimer(this);
        connect(positionCheckTimer, &QTimer::timeout, this, &GameController::updateGame);
        positionCheckTimer->start(16);

        // Central enemy spawn timer
        enemySpawnTimer = new QTimer(this);
        connect(enemySpawnTimer, &QTimer::timeout, this, &GameController::spawnNextEnemy);
        startSpawnPattern(1); // Start with round 1-1
    }

    discPlacements = {
        //Level, Disc Location
        {1, {{5, "left"}, {5, "right"}}},
        //{1, {{1, "left"}, {2, "left"}, {3, "left"}, {4, "left"}, {5, "left"}, {6, "left"}, {7, "left"}, {1, "right"}, {2, "right"}, {3, "right"}, {4, "right"}, {5, "right"}, {6, "right"}, {7, "right"}}},
        {2, {{6, "left"}, {7, "right"}}},
		{3, {{4, "left"}, {6, "right"}}},
		{4, {{1, "left"}, {7, "right"}}},
		{5, {{4, "left"}, {1, "right"}, {5, "right"}}},
		{6, {{6, "left"}, {1, "right"}, {6, "right"}}},
		{7, {{3, "left"}, {6, "right"}}},
		{8, {{2, "left"}, {4, "right"}}},
		{9, {{3, "left"}, {5, "left"}, {3, "right"}, {4, "right"}}},
		{10, {{3, "left"}, {4, "left"}, {4, "right"}, {6, "right"}}},
		{11, {{1, "left"}, {3, "left"}, {5, "right"}}},
		{12, {{2, "right"}, {5, "right"}, {7, "right"}}},
		{13, {{1, "left"}, {4, "left"}, {6, "left"}, {2, "right"}, {6, "right"}, {7, "right"}}},
		{14, {{1, "left"}, {5, "left"}, {6, "left"}, {2, "right"}, {6, "right"}, {7, "right"}}},
		{15, {{3, "left"}, {5, "left"}, {2, "right"}, {3, "right"}, {5, "right"}}},
		{16, {{1, "left"}, {2, "left"}, {4, "right"}, {7, "right"}}},
		{17, {{1, "left"}, {2, "left"}, {4, "left"}, {6, "left"}, {1, "right"}, {3, "right"}, {7, "right"}}},
		{18, {{1, "left"}, {5, "left"}, {6, "left"}, {2, "right"}, {4, "right"}, {5, "right"}}},
		{19, {{3, "left"}, {5, "left"}, {3, "right"}, {4, "right"}, {7, "right"}}}
    };
    addDiscs(1);
    
    initLabels();

	dbHelper::connectDatabase();
    displayHighScores();
}

GameController::~GameController() {}

void GameController::initLabels() {
    // Load the custom font
    int fontId = QFontDatabase::addApplicationFont(":PressStart2P-Regular.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    int pauseFont = height() / 30;
    int controlFont = height() / 40;
    int gameInfoFont = height() / 25;
    int gameOverFont = height() / 20;

    // Set up the "Paused" label
    pausedLabel->setText("  Press R to start/resume");
    pausedLabel->setAlignment(Qt::AlignCenter);
    pausedLabel->setStyleSheet(QString("font-family: '%1'; font-size: %2px; color: white; background-color: rgba(0, 0, 0, 128);").arg(fontFamily, QString::number(pauseFont)));
    pausedLabel->setGeometry(0, 0, width(), height());
    pausedLabel->show();
    pausedLabel->raise();

    // Set up the "Controls" label
    controlsLabel->setText("Controls:\n"
        "ESC - Exit\n"
        "R - Start/Resume\n"
        "P - Pause\n"
        "J - Up Left\n"
        "K - Up Right\n"
        "M - Down Left\n"
        ", - Down Right");
    controlsLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    controlsLabel->setStyleSheet(QString("font-family: '%1'; font-size: %2px; color: white; background-color: rgba(0, 0, 0, 128);").arg(fontFamily, QString::number(controlFont)));
    controlsLabel->setGeometry(width() / 1.35, height() /75, width() / 4, height() / 4.5); // Position on the right side
    controlsLabel->show();

    // Set up the "Score" label
    gameInfoLabel->setText(QString("Score: \n%1\nLevel: \n%2\nLives: \n%3").arg(score).arg(level).arg(lives));
    gameInfoLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    gameInfoLabel->setStyleSheet(QString("font-family: '%1'; font-size: %2px; color: white; background-color: rgba(0, 0, 0, 128);").arg(fontFamily, QString::number(gameInfoFont)));
    gameInfoLabel->setGeometry(width() / 50, height() / 16, width() / 6, height() / 4); // Position on the left side
    gameInfoLabel->show();

    // Set up the "Goal Color" label
    QLabel* goalColorLabel = new QLabel(this);
    goalColorLabel->setText("Goal \nColor:");
    goalColorLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    goalColorLabel->setStyleSheet(QString("font-family: '%1'; font-size: %2px; color: white; background-color: rgba(0, 0, 0, 128);").arg(fontFamily, QString::number(gameInfoFont)));
    goalColorLabel->setGeometry(width() / 50, height() / 2.5, width() / 6, height() / 10); // Position below the "Lives" label
    goalColorLabel->show();

    // Set up the cube to display the goal color
    Cube* goalColorCube = new Cube(this, this->width() / 16, this->width() / 19, this->width() / 53);
    goalColorCube->setTopFaceColor(QColor(255, 255, 255)); // Set to the goal color (white)
    goalColorCube->move(width() / 40, height() / 2); // Position below the "Goal Color" label
    goalColorCube->show();

    // Set up the "Game Over" label
    gameOverLabel->setText("GAME OVER\nPress R to Restart");
    gameOverLabel->setAlignment(Qt::AlignCenter);
    gameOverLabel->setStyleSheet(QString("font-family: '%1'; font-size: %2px; color: white; background-color: rgba(0, 0, 0, 128);").arg(fontFamily, QString::number(controlFont)));
    gameOverLabel->setGeometry(0, 0, width(), height());
    gameOverLabel->hide(); // Initially hidden
}

void GameController::startSpawnPattern(int round) {
    if (!spawnPatterns.contains(round)) return;

    // Get the spawn pattern for the round
    const SpawnPattern& pattern = spawnPatterns[round];

    // Initialize the spawn queue
    currentSpawnQueue.clear(); // Clear any existing queue
    for (const QString& enemy : pattern.pattern) {
        currentSpawnQueue.enqueue(enemy); // Enqueue each enemy type
    }

    // Set the timer interval
    enemySpawnTimer->setInterval(pattern.interval);
}

void GameController::spawnNextEnemy() {
    if (currentSpawnQueue.isEmpty()) {
        // Restart the spawn order by re-enqueuing the original pattern
        int currentRound = 1; // Replace with the actual logic to track the current round
        if (spawnPatterns.contains(currentRound)) {
            const SpawnPattern& pattern = spawnPatterns[currentRound];
            for (const QString& enemy : pattern.pattern) {
                currentSpawnQueue.enqueue(enemy);
            }
        }
    }

    if (currentSpawnQueue.isEmpty()) {
        enemySpawnTimer->stop(); // Stop the timer if the queue is still empty
        return;
    }

    QString enemyType = currentSpawnQueue.dequeue();

    // Handle the special case for Coily
    if (enemyType == "C") {
        bool coilyExists = false;
        for (Enemy* enemy : enemies) {
            if (dynamic_cast<Coily*>(enemy)) {
                coilyExists = true;
                break;
            }
        }

        if (coilyExists) {
            // Skip this Coily spawn and move to the next enemy
            QTimer::singleShot(0, this, &GameController::spawnNextEnemy);
            return;
        }

        spawnCoily();
    }
    else if (enemyType == "R") {
        spawnRedBall();
    }

    // Ensure the first enemy's animation is properly started
    if (!activeAnimations.isEmpty()) {
        for (QVariantAnimation* animation : activeAnimations) {
            if (animation->state() != QAbstractAnimation::Running) {
                animation->start(); // Restart any paused or stopped animations
            }
        }
    }
}

void GameController::addDiscs(int level) {
    if (!cubeWidget || !discPlacements.contains(level)) return;

    // Clear existing discs
    qDeleteAll(discs);
    discs.clear();

    // Define cube dimensions based on CubeWidget's math
    int cubeWidth = cubeWidget->width() / 8;
    int cubeHeight = cubeWidget->width() / 9.5;
    int depth = cubeWidget->width() / 26.5;

    // Define the starting Y position of the pyramid
    int startY = cubeWidget->height() / 6;

    // Get the disc placements for the current level
    QVector<QPair<int, QString>> placements = discPlacements[(level > 19)? 19 : level];

    for (const QPair<int, QString>& placement : placements) {
        int row = placement.first;
        QString side = placement.second;

        // Calculate the Y position of the selected row
        int rowY = startY + row * cubeHeight;

        // Create a new disc
        Disc* disc = new Disc(this, row, side);
        disc->setScale(cubeWidget->width() / 20, cubeWidget->width() / 20); // Adjust size as needed

        // Position the disc based on the side (left or right)
        if (side == "left") {
            int rowX = cubeWidget->width() / 2 - (row * cubeWidth/2); // Starting X for the row
            disc->move(cubeWidget->x() + rowX + cubeWidth*0.12, rowY - cubeHeight*1.2); // Move left and up
            
        }
        else if (side == "right") {
            int rowX = cubeWidget->width() / 2 + (row * cubeWidth / 2); // Ending X for the row
            disc->move(cubeWidget->x() + rowX + cubeWidth*0.37, rowY - cubeHeight*1.2); // Move right and up
        }

        // Show the disc and add it to the list
        disc->show();
        disc->lower();
        discs.append(disc);

    }
}

void GameController::updateGameInfoLabel() {
    gameInfoLabel->setText(QString("Score: \n%1\nLevel: \n%2\nLives: \n%3").arg(score).arg(level).arg(lives));
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
    case Qt::Key_R:
        if (pausedLabel->isVisible() || gameOverLabel->isVisible()) {
            positionCheckTimer->start(16);
            enemySpawnTimer->start(); // Start enemy spawning
            resumeAnimations(); // Resume animations
            pausedLabel->hide();
            gameOverLabel->hide();
			paused = false;
        }
        break;
    case Qt::Key_P:
        positionCheckTimer->stop();
        enemySpawnTimer->stop();
        pauseAnimations(); // Pause animations
        pausedLabel->show();
        paused = true;
        break;
    default: QMainWindow::keyPressEvent(event); return;
    }

    if (positionCheckTimer->isActive()) {
        if (isJumping) {
            if (queuedDirection.isEmpty()) {
                queuedDirection = inputDirection;
            }
        }
        else {
            moveQbert(inputDirection);
        }
    }
}

void GameController::moveQbert(const QString& direction) {
    if (!qbert) return;

    int horizontalStep = screenWidth / 32;
    int verticalStep = screenWidth / 19.277;

    QPoint startPos = qbert->pos();
    QPoint endPos = startPos;

    if (direction == "up_left") {
        endPos.setX(endPos.x() - horizontalStep);
        endPos.setY(endPos.y() - verticalStep);
    }
    else if (direction == "up_right") {
        endPos.setX(endPos.x() + horizontalStep);
        endPos.setY(endPos.y() - verticalStep);
    }
    else if (direction == "down_left") {
        endPos.setX(endPos.x() - horizontalStep);
        endPos.setY(endPos.y() + verticalStep);
    }
    else if (direction == "down_right") {
        endPos.setX(endPos.x() + horizontalStep);
        endPos.setY(endPos.y() + verticalStep);
    }
    animateQbertMovement(startPos, endPos);
}

void GameController::resetQbert() {
    Cube* topCube = cubeWidget->getCubes().first();
    QScreen* screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    screenWidth = screenGeometry.width();
    qbert->move(topCube->x() + screenWidth / 4 + qbert->width() / 4.5, topCube->y() - qbert->height() / 2);
    qbert->repaint();
    qbert->raise();
}

void GameController::resetAfterQbertFall() {
    // Pause the game and all components
    enemySpawnTimer->stop();
    pauseAnimations();

    // Properly stop and delete all active animations
    for (QVariantAnimation* animation : activeAnimations) {
        animation->stop();
        animation->deleteLater();
    }
    activeAnimations.clear();

    // Clear all enemies
    for (Enemy* enemy : enemies) {
        enemy->deleteLater();
    }
    enemies.clear();

    // Clear the queued direction
    queuedDirection.clear();

    coilyChasing = false;
    isJumping = false;

    // Decrease lives by 1
    lives--;

    // Check for game over
    if (lives <= 0) {
        handleGameOver();
        return;
    }

    // Reset Qbert to the top of the pyramid
    resetQbert();

    // Restart the spawn queue
    startSpawnPattern(level);

    // Automatically pause the game
    pausedLabel->show();
    pausedLabel->raise();

    // Update the game info label
    updateGameInfoLabel();
}

void GameController::animateQbertMovement(const QPoint& startPos, const QPoint& endPos) {
    int duration = 600;

    qbert->setHorizonVelo(static_cast<double>(endPos.x() - startPos.x()) / duration);

    QVariantAnimation* xAnim = new QVariantAnimation(this);
    QVariantAnimation* yAnim = new QVariantAnimation(this);

    xAnim->setStartValue(startPos.x());
    xAnim->setEndValue(endPos.x());
    xAnim->setDuration(duration);
    xAnim->setEasingCurve(QEasingCurve::Linear);

    QEasingCurve::Type curveType = (endPos.y() < startPos.y()) ? QEasingCurve::OutCubic : QEasingCurve::InCubic;
    yAnim->setStartValue(startPos.y());
    yAnim->setEndValue(endPos.y());
    yAnim->setDuration(duration);
    yAnim->setEasingCurve(curveType);

    // Add animations to the active list
    activeAnimations.append(xAnim);
    activeAnimations.append(yAnim);

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
            if (!findQbertCube()) {
                fallQbertToBottom(qbert->getHorizonVelo(), curveType, endPos.y());
                return;
            }

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

    // Remove animations from the active list when finished
    connect(xAnim, &QVariantAnimation::finished, this, [=]() {
        activeAnimations.removeOne(xAnim);
        xAnim->deleteLater();
        });
    connect(yAnim, &QVariantAnimation::finished, this, [=]() {
        activeAnimations.removeOne(yAnim);
        yAnim->deleteLater();
        });

    isJumping = true;
    xAnim->start();
    yAnim->start();
}

void GameController::updateGame() {
    if (!cubeWidget) return;

    Cube* cube = findQbertCube(); 
    if (cube && cube != currentCube) { // Only update if Qbert has moved to a new cube
        currentCube = cube; 
        int scoreChange = cube->setState(level); // Update the cube's state and get the score change
        score += scoreChange; 
    }

    checkAndLevelUp();
    updateGameInfoLabel();
}

void GameController::checkAndLevelUp() {
    if (!cubeWidget) return;

    // Get all cubes
    QVector<Cube*> cubes = cubeWidget->getCubes();

    // Check if all cubes have white top faces
    for (Cube* cube : cubes) {
        if (cube->getTopFaceColor() != QColor(255, 255, 255)) {
            return; // Exit if any cube is not white
        }
    }
    score += 750 + level / 4 * 1000 + level % 4 * 250;
	score += discs.size() * 50; // Add score for discs

    // Check if Qbert qualifies for an extra life
    while (score >= nextExtraLife) {
        lives++; // Grant an extra life
        nextExtraLife += 14000; // Update the threshold for the next extra life
    }

    // All cubes are white, level up
    level++;
    resetQbert();

    // Reset all cubes' top face color to red and state to 0
    for (Cube* cube : cubes) {
        cube->resetState();
    }

	coilyChasing = false;
    currentCube = nullptr; 
    addDiscs(level);

    // Stop and clear all animations
    for (QVariantAnimation* animation : activeAnimations) {
        animation->stop();
        animation->deleteLater();
    }
    activeAnimations.clear();

    // Disconnect and delete all enemies
    for (Enemy* enemy : enemies) {
        disconnect(enemy, nullptr, this, nullptr); // Disconnect all signals
        enemy->deleteLater();
    }
    enemies.clear();

    // Restart the enemy spawn pattern
    startSpawnPattern(level);
    enemySpawnTimer->start();

}

Cube* GameController::findQbertCube() {
    if (!cubeWidget) return nullptr;

    QPoint qbertPos = qbert->geometry().center();
    qbertPos.setX(qbertPos.x() - screenWidth / 4 - qbert->width() / 1.42);
    qbertPos.setY(qbertPos.y());

    for (Cube* cube : cubeWidget->getCubes()) {
        QRect cubeTopFace(cube->x(), cube->y(), cube->width(), cube->depth());
        if (cubeTopFace.x() == qbertPos.x() && cubeTopFace.y() - qbertPos.y() < 5 && cubeTopFace.y() - qbertPos.y() >= 0) {
            return cube;
        }
    }
    return nullptr;
}

Cube* GameController::findEnemyCube(Enemy* enemy) {
    if (!cubeWidget || !enemy) return nullptr;

    QPoint enemyPos = enemy->geometry().center();
    // Check if the enemy is Coily and adjust the Y position based on its form
    if (Coily* coily = dynamic_cast<Coily*>(enemy)) {
        if (coily->getIsCoilyForm()) {
            enemyPos.setX(enemyPos.x() - screenWidth / 4 - enemy->width() * 1.33);
            enemyPos.setY(enemyPos.y() - enemy->height() / 4.2); //snake form
        }
        else {
            enemyPos.setX(enemyPos.x() - screenWidth / 4 - enemy->width() * 1.225);
            enemyPos.setY(enemyPos.y() - enemy->height()); //ball form
        }
    }
    else {
        // Default adjustment for other enemies
        enemyPos.setY(enemyPos.y() - enemy->height());
    }
    for (Cube* cube : cubeWidget->getCubes()) {
        QRect cubeTopFace(cube->x(), cube->y(), cube->width(), cube->depth());
        if (cubeTopFace.x()-enemyPos.x() < 5 && cubeTopFace.x()-enemyPos.x() >= -2 && cubeTopFace.y() - enemyPos.y() < 5 && cubeTopFace.y() - enemyPos.y() >= -2) {
            return cube;
        }
    }
    return nullptr;
}

void GameController::fallQbertToBottom(double horizontalVelocity, QEasingCurve::Type curveType, double easedYStart) {
    int screenHeight = QApplication::primaryScreen()->availableGeometry().height();
    QPoint startPos = qbert->pos();
    positionCheckTimer->stop();

    // Define the current position
    double currentX = startPos.x();
    double currentY = easedYStart; // Start from the eased Y position

    // Total fall distance
    double fallDistance = screenHeight - currentY - qbert->height();

    //Determine the easing curve for vertical movement
    QEasingCurve easingCurve = (curveType == QEasingCurve::OutCubic)
                                ? QEasingCurve(QEasingCurve::InQuad)
                                : QEasingCurve(QEasingCurve::Linear);

    double startProgress = 0.0;

    // Timer to update Qbert's position
    QTimer* fallTimer = new QTimer(this);
    QElapsedTimer elapsedTime;
    elapsedTime.start();
    
    // Total duration for the fall
    int totalDuration = fallDistance * 1.32; 

    connect(fallTimer, &QTimer::timeout, this, [=]() mutable {
        // Calculate elapsed time as a fraction of total duration
        double t = startProgress + static_cast<double>(elapsedTime.elapsed()) / totalDuration;

        // Stop the timer if the fall is complete
        if (t >= 1.0) {
            t = 1.0; // Clamp to 1.0
            fallTimer->stop();
            fallTimer->deleteLater();
            QTimer::singleShot(17, this, [=]() {
				resetAfterQbertFall();
                });
        }

        // Calculate the eased vertical position
        double easedY = currentY + easingCurve.valueForProgress(t) * fallDistance;

        // Update Qbert's position
        currentX += horizontalVelocity * 16; // Move horizontally based on velocity
        qbert->move(static_cast<int>(currentX), static_cast<int>(easedY));

        // Check for collision with a disc
        for (Disc* disc : discs) {
            // Get Qbert's current row
            Cube* topCube = cubeWidget->getCubes().first();
            int qbertRow = (qbert->pos().y() + qbert->height() - topCube->y() + screenWidth / 19.277) / topCube->height() + 1;

            // Check if the disc matches Qbert's row and intersects
            if (disc->getRow() == qbertRow && qbert->geometry().intersects(disc->geometry())) {
                // Stop the fall animation
                fallTimer->stop();
                fallTimer->deleteLater();

                // Animate Qbert and the disc to the top of the pyramid
                animateQbertAndDiscToTop(disc);
                return;
            }
        }
        });
    cubeWidget->raise();
	qbert->stackUnder(cubeWidget);

    // Start the timer with a small interval (e.g., 16 ms for ~60 FPS)
    fallTimer->start(16);
}

void GameController::animateQbertAndDiscToTop(Disc* disc) {
    if (!cubeWidget || !qbert || !disc) return;

    // Get the top cube's position
    Cube* topCube = cubeWidget->getCubes().first();
    QPoint qbertStartPos = qbert->pos();
    QPoint discStartPos = disc->pos();

    // Define the end positions
    QPoint qbertEndPosition(topCube->x() + screenWidth / 4, topCube->y() - screenHeight/6); 
    QPoint discEndPosition(qbertEndPosition.x() + qbert->width()/3.9, topCube->y() - qbert->height()*1.2);

    int duration = 2000; // Duration for the animation in milliseconds

    // Single animation to synchronize both Qbert and the disc
    QVariantAnimation* animation = new QVariantAnimation(this);
    animation->setStartValue(0.0); // Start progress
    animation->setEndValue(1.0);   // End progress
    animation->setDuration(duration);
    animation->setEasingCurve(QEasingCurve::Linear);

    connect(animation, &QVariantAnimation::valueChanged, this, [=](const QVariant& value) {
        double progress = value.toDouble();

        // Interpolate positions for Qbert and the disc
        QPoint qbertPos = qbertStartPos + (qbertEndPosition - qbertStartPos) * progress;
        QPoint discPos = discStartPos + (discEndPosition - discStartPos) * progress;

        // Update positions
        qbert->move(qbertPos);
        disc->move(discPos);
        });

    // Cleanup after animation
    connect(animation, &QVariantAnimation::finished, this, [=]() {
        animation->deleteLater();

        // Reset Qbert to the top of the pyramid
        resetQbert();
        positionCheckTimer->start(16);
		queuedDirection.clear();

        // Remove the disc
        discs.removeOne(disc);
        disc->deleteLater();
        });

    // Start the animation
    animation->start();
}

void GameController::handleGameOver() {
    // Check if the player's score qualifies as a high score
    if (isHighScore(score)) {
        QString initials = promptForHighScoreName();
        dbHelper::addHighScore(initials, score);
    }
	displayHighScores();

    // Reset game state
    score = 0;
    level = 1;
    lives = 3;

    // Stop and clear all animations
    for (QVariantAnimation* animation : activeAnimations) {
        animation->stop();
        animation->deleteLater();
    }
    activeAnimations.clear();

    // Reset all cubes' top face colors to red
    if (cubeWidget) {
        QVector<Cube*> cubes = cubeWidget->getCubes();
        for (Cube* cube : cubes) {
            cube->resetState();
            cube->setTopFaceColor(QColor(255, 0, 0));
        }
    }

    // Remove all enemies
    for (Enemy* enemy : enemies) {
        disconnect(enemy, nullptr, this, nullptr); // Disconnect all signals
        enemy->deleteLater();
    }
    enemies.clear();

    // Update the game info label
    updateGameInfoLabel();

    // Display the "Game Over" label
    gameOverLabel->show();
    gameOverLabel->raise();

    // Stop the game timer
    if (positionCheckTimer) {
        positionCheckTimer->stop();
    }

    addDiscs(1);
    resetQbert();
}

void GameController::spawnRedBall() {
    if (!cubeWidget) return;

    // Create a new red ball
    RedBall* redBall = new RedBall(this);
    redBall->setScale(cubeWidget->width() / 21, cubeWidget->width() / 25);

    // Randomly pick a direction ("down_left" or "down_right")
    QString direction = (std::rand() % 2 == 0) ? "down_left" : "down_right";
    redBall->setDirection(direction);

    // Position the red ball at the top of the pyramid
    Cube* topCube = cubeWidget->getCubes().first();
    int startX = (direction == "down_left") ? topCube->x() + screenWidth / 4 - redBall->width() / 2
        : topCube->x() + screenWidth / 4 + topCube->width() - redBall->width() / 2;
    int startY = topCube->y() + topCube->height() + redBall->height() / 2;
    redBall->move(startX, startY);

    // Show the red ball
    redBall->show();
    // Add the red ball to the list
    enemies.append(redBall);

    // Animate the red ball's movement
    QPoint startPos = redBall->pos();
    QPoint endPos = (direction == "down_left")
        ? QPoint(startPos.x() - screenWidth / 32, startPos.y() + screenWidth / 19.277)
        : QPoint(startPos.x() + screenWidth / 32, startPos.y() + screenWidth / 19.277);
    animateEnemy(redBall, startPos, endPos);

    // Connect the red ball's signal to handle when it falls off the pyramid
    connect(redBall, &Enemy::reachedBottomSignal, this, [=]() {
        fallEnemyToBottom(redBall, redBall->getHorizonVelo(), QEasingCurve::InQuad, redBall->pos().y());
        });

}

void GameController::animateEnemy(Enemy* enemy, const QPoint& startPos, const QPoint& endPos) {
    if (enemies.isEmpty()) {
        return; 
    }
    // Check if this is a dummy animation (start and end positions are the same)
    if (startPos == endPos) {
        // Immediately proceed to the next recursive call
        QString newDirection = (std::rand() % 2 == 0) ? "down_left" : "down_right";
        QPoint nextStartPos = enemy->pos();
        QPoint nextEndPos = (newDirection == "down_left")
            ? QPoint(nextStartPos.x() - screenWidth / 32, nextStartPos.y() + screenWidth / 19.277)
            : QPoint(nextStartPos.x() + screenWidth / 32, nextStartPos.y() + screenWidth / 19.277);
        animateEnemy(enemy, nextStartPos, nextEndPos);
        return;
    }
    int duration = 600;

    enemy->setHorizonVelo(static_cast<double>(endPos.x() - startPos.x()) / duration);

    QVariantAnimation* xAnim = new QVariantAnimation(this);
    QVariantAnimation* yAnim = new QVariantAnimation(this);

    xAnim->setStartValue(startPos.x());
    xAnim->setEndValue(endPos.x());
    xAnim->setDuration(duration);
    xAnim->setEasingCurve(QEasingCurve::Linear);

    QEasingCurve::Type curveType = (endPos.y() < startPos.y()) ? QEasingCurve::OutCubic : QEasingCurve::InCubic;
    yAnim->setStartValue(startPos.y());
    yAnim->setEndValue(endPos.y());
    yAnim->setDuration(duration);
    yAnim->setEasingCurve(curveType);

    // Add animations to the active list
    activeAnimations.append(xAnim);
    activeAnimations.append(yAnim);
	enemy->isAnimating = true; 

    // Sync movement
    connect(xAnim, &QVariantAnimation::valueChanged, this, [=](const QVariant& xVal) {
        if (!enemies.contains(enemy)) {
            return; // Check if enemy is valid
        }
        enemy->move(xVal.toInt(), enemy->y());
        });

    connect(yAnim, &QVariantAnimation::valueChanged, this, [=](const QVariant& yVal) {
        if (!enemies.contains(enemy)) {
            return; // Check if enemy is valid
        }
        enemy->move(enemy->x(), yVal.toInt());
        });


    // Use shared counter to track both animations finishing
    auto finishedCount = QSharedPointer<int>::create(0);

    auto onFinished = [=]() mutable {
        (*finishedCount)++;
        if (*finishedCount == 2) {
			if (!enemies.contains(enemy)) return; // Check if enemy is valid
			enemy->isAnimating = false;
            // Check for collision with Qbert
            if (qbert->collisionRect().translated(qbert->pos()).intersects(enemy->geometry())) {
                // Handle collision
                resetAfterCoilyCatch();
                return;
            }
			// Check if the enemy is Coily and handle if it needs to fall
            if (dynamic_cast<Coily*>(enemy)) {
                if (!findEnemyCube(enemy)){
                    fallEnemyToBottom(enemy, enemy->getHorizonVelo(), curveType, enemy->y());
                    return;
                }
            }

            // Get the y position of the last row of cubes
            QVector<Cube*> cubes = cubeWidget->getCubes();
            if (!cubes.isEmpty()) {
                int lastRowY = cubes.last()->y() + cubes.last()->height(); // Default for red balls
                int enemyBottomY = enemy->y() + enemy->height(); // Get the bottom of the enemy

                // Adjust logic for Coily
                if (dynamic_cast<Coily*>(enemy)) {
                    lastRowY = cubes.last()->y(); // Coily stops at the top of the last row
                }

                // Check if the enemy has reached or passed the last row
                if (enemyBottomY >= lastRowY) {
                    emit enemy->reachedBottomSignal();
                    return;
                }
            }

            // Randomly pick a new direction for the next jump
            QString newDirection = (std::rand() % 2 == 0) ? "down_left" : "down_right";

            // Calculate the next position for the enemy based on the new direction
            QPoint nextStartPos = enemy->pos();
            QPoint nextEndPos = (newDirection == "down_left")
                ? QPoint(nextStartPos.x() - screenWidth / 32, nextStartPos.y() + screenWidth / 19.277)
                : QPoint(nextStartPos.x() + screenWidth / 32, nextStartPos.y() + screenWidth / 19.277);
            if (!coilyChasing || !dynamic_cast<Coily*>(enemy) ) {
                // Delay the recursive call using QTimer::singleShot
				if (!enemies.contains(enemy) || paused) {
					return; // Check if enemy is valid
				}
                QTimer::singleShot(300, this, [=]() {
                    animateEnemy(enemy, nextStartPos, nextEndPos);
                    });
            }
            else if (coilyChasing && dynamic_cast<Coily*>(enemy) && !paused) {
                // Calculate the next direction and continue moving
                QString nextDirection = calculateShortestDirection(endPos, qbert->pos());
                QTimer::singleShot(600, this, [=]() {
                    if (!paused && enemies.contains(coily)) { // Check if Coily is still valid
                        moveCoily(coily, nextDirection);
                    }
                    });
            }
            
        }
        };

    connect(xAnim, &QVariantAnimation::finished, this, onFinished);
    connect(yAnim, &QVariantAnimation::finished, this, onFinished);

    // Remove animations from the active list when finished
    connect(xAnim, &QVariantAnimation::finished, this, [=]() {
        activeAnimations.removeOne(xAnim);
        xAnim->deleteLater();
        });
    connect(yAnim, &QVariantAnimation::finished, this, [=]() {
        activeAnimations.removeOne(yAnim);
        yAnim->deleteLater();
        });

    xAnim->start();
    yAnim->start();
}

void GameController::fallEnemyToBottom(Enemy* enemy, double horizontalVelocity, QEasingCurve::Type curveType, double easedYStart) {
    int screenHeight = QApplication::primaryScreen()->availableGeometry().height();
    QPoint startPos = enemy->pos();

    // Define the current position
    double currentX = startPos.x();
    double currentY = easedYStart; // Start from the eased Y position

    // Total fall distance
    double fallDistance = screenHeight - currentY - enemy->height();

    // Determine the easing curve for vertical movement
    QEasingCurve easingCurve = (curveType == QEasingCurve::OutCubic)
        ? QEasingCurve(QEasingCurve::InQuad)
        : QEasingCurve(QEasingCurve::Linear);

    double startProgress = 0.0;

    // Timer to update the enemy's position
    QTimer* fallTimer = new QTimer(this);
    QElapsedTimer elapsedTime;
    elapsedTime.start();

    // Total duration for the fall
    int totalDuration = fallDistance * 1.32;

    connect(fallTimer, &QTimer::timeout, this, [=]() mutable {
        // Calculate elapsed time as a fraction of total duration
        double t = startProgress + static_cast<double>(elapsedTime.elapsed()) / totalDuration;

        // Stop the timer if the fall is complete
        if (t >= 1.0) {
            t = 1.0; // Clamp to 1.0
            fallTimer->stop();
            fallTimer->deleteLater();
            disconnect(enemy, nullptr, this, nullptr);
            enemies.removeOne(enemy);
            enemy->deleteLater();
            if (dynamic_cast<Coily*>(enemy)) {
				coilyChasing = false;
                score += 500;
            }
            return;
        }

        // Calculate the eased vertical position
        double easedY = currentY + easingCurve.valueForProgress(t) * fallDistance;

        // Update the enemy's position
        currentX += horizontalVelocity * 16; // Move horizontally based on velocity
        enemy->move(static_cast<int>(currentX), static_cast<int>(easedY));
        });

    enemy->stackUnder(cubeWidget);

    // Start the timer with a small interval (e.g., 16 ms for ~60 FPS)
    fallTimer->start(16);
}

void GameController::spawnCoily() {
    if (!cubeWidget) return;

    // Create a new Coily
    coily = new Coily(this);
    coily->setScale(cubeWidget->width() / 25, cubeWidget->width() / 25);

    // Randomly pick a direction ("down_left" or "down_right")
    QString direction = (std::rand() % 2 == 0) ? "down_left" : "down_right";
    coily->setDirection(direction);

    // Position Coily at the top of the pyramid
    Cube* topCube = cubeWidget->getCubes().first();
    int startX = (direction == "down_left") ? topCube->x() + screenWidth / 4 - coily->width() / 2
        : topCube->x() + screenWidth / 4 + topCube->width() - coily->width() / 2;
    int startY = topCube->y() + topCube->height() + coily->height() / 2;
    coily->move(startX, startY);

    // Show Coily
    coily->show();
    // Add Coily to the list of enemies
    enemies.append(coily);

    // Animate Coily's movement
    QPoint startPos = coily->pos();
    QPoint endPos = (direction == "down_left")
        ? QPoint(startPos.x() - screenWidth / 32, startPos.y() + screenWidth / 19.277)
        : QPoint(startPos.x() + screenWidth / 32, startPos.y() + screenWidth / 19.277);
    animateEnemy(coily, startPos, endPos);

    // Connect Coily's signal to handle when it reaches the bottom
    connect(coily, &Enemy::reachedBottomSignal, this, [=]() {
        startCoilyChase(coily);
        });
}

void GameController::pauseAnimations() {
    for (QVariantAnimation* animation : activeAnimations) {
        if (animation->state() == QAbstractAnimation::Running) {
            animation->pause();
        }
    }
}

void GameController::resumeAnimations() {
    // Resume all paused animations
    for (QVariantAnimation* animation : activeAnimations) {
        if (animation->state() == QAbstractAnimation::Paused) {
            animation->resume();
        }
    }

    // Restart recursive calls for all enemies that are not currently animating
    for (Enemy* enemy : enemies) {
        if (!enemy->isAnimating) {
			if (dynamic_cast<Coily*>(enemy) && coilyChasing) {
				moveCoily(dynamic_cast<Coily*>(enemy), "");
			}
            else {
                // Create a dummy animation to restart the recursive call
                QPoint currentPos = enemy->pos();
                animateEnemy(enemy, currentPos, currentPos); // Dummy animation
            }
        }
    }
}

void GameController::startCoilyChase(Coily* coily) {
    if (!qbert || !cubeWidget) return;

    // Calculate the shortest direction to Qbert
    QString direction = calculateShortestDirection(coily->pos(), qbert->pos());
    if (!coilyChasing) {
        // Start moving Coily in the calculated direction
        QTimer::singleShot(2000, this, [=]() {
            coily->swapPixmap();
			coilyChasing = true;
            moveCoily(coily, direction);
            });
	}
    else {
        // Continue moving Coily in the calculated direction
        QTimer::singleShot(600, this, [=]() {
            if (!paused && enemies.contains(coily)) { // Check if Coily is still valid
                moveCoily(coily,direction);
            }
            });
    }
}

QString GameController::calculateShortestDirection(const QPoint& coilyPos, const QPoint& qbertPos) {
    int dx = qbertPos.x() - coilyPos.x();
    int dy = qbertPos.y() - coilyPos.y();

    if (dx > 0 && dy < 0) {
        return "up_right";
    }
    else if (dx < 0 && dy < 0) {
        return "up_left";
    }
    else if (dx < 0 && dy > 0) {
        return "down_left";
    }
    else if (dx > 0 && dy > 0) {
        return "down_right";
    }

    return ""; // Default case (should not happen)
}

void GameController::moveCoily(Coily* coily, const QString& direction) {
    if (!coily || !qbert || enemies.isEmpty()) return;

    QString moveDirection = direction.isEmpty() ? coily->getDirection() : direction;

    int horizontalStep = screenWidth / 32;
    int verticalStep = screenWidth / 19.277;

    QPoint startPos = coily->pos();
    QPoint endPos = startPos;

    if (moveDirection == "up_left") {
        endPos.setX(endPos.x() - horizontalStep);
        endPos.setY(endPos.y() - verticalStep);
    }
    else if (moveDirection == "up_right") {
        endPos.setX(endPos.x() + horizontalStep);
        endPos.setY(endPos.y() - verticalStep);
    }
    else if (moveDirection == "down_left") {
        endPos.setX(endPos.x() - horizontalStep);
        endPos.setY(endPos.y() + verticalStep);
    }
    else if (moveDirection == "down_right") {
        endPos.setX(endPos.x() + horizontalStep);
        endPos.setY(endPos.y() + verticalStep);
    }

    // Update Coily's direction
    coily->setDirection(moveDirection);

    // Animate Coily's movement
    animateEnemy(coily, startPos, endPos);

    // Total animation duration
    int duration = 600; // Match the animation duration

    // Check collision at the end of the animation
    QTimer::singleShot(duration, this, [=]() {
        if (paused || !enemies.contains(coily)) return; // Check if Coily is still valid
        if (coily && coily->geometry().intersects(qbert->geometry())) {
            resetAfterCoilyCatch();
        }
        });
	/*
    // Calculate the next direction and continue moving
    QString nextDirection = calculateShortestDirection(endPos, qbert->pos());
    QTimer::singleShot(900, this, [=]() {
        if (!paused && enemies.contains(coily)) { // Check if Coily is still valid
            moveCoily(coily, nextDirection);
        }
        });
    */
}

void GameController::resetAfterCoilyCatch() {
    // Pause the game and all components
	positionCheckTimer->stop();
    enemySpawnTimer->stop();
    pauseAnimations();

    // Properly stop and delete all active animations
    for (QVariantAnimation* animation : activeAnimations) {
        animation->stop();
        animation->deleteLater();
    }
    activeAnimations.clear();

    // Clear all enemies
    for (Enemy* enemy : enemies) {
        enemy->deleteLater();
    }
    enemies.clear();

    // Clear the queued direction
    queuedDirection.clear();

	coilyChasing = false;
    isJumping = false;

    if (currentCube) {
        qbert->move(currentCube->x() + screenWidth / 4 + qbert->width() / 4.5,
            currentCube->y() - qbert->height() / 2);
        qbert->repaint();
        qbert->raise();
    }
    else {
        // If no currentCube is set, reset Qbert to the top of the pyramid
        resetQbert();
    }

    // Decrease lives by 1
    lives--;

    // Check for game over
    if (lives <= 0) {
        handleGameOver();
        return;
    }

    // Restart the spawn queue and timer
    startSpawnPattern(level); 

    // Automatically pause the game
    pausedLabel->show();
    pausedLabel->raise();

    // Update the game info label
    updateGameInfoLabel();
}

void GameController::displayHighScores() {
    // Retrieve the top 5 high scores
    QList<QPair<QString, int>> highScores = dbHelper::getTopHighScores(5);

    // Create a string to display the high scores
    QString highScoresText = "High Scores:\n";
    for (int i = 0; i < highScores.size(); ++i) {
        highScoresText += QString("%1. %2 - %3\n")
            .arg(i + 1)
            .arg(highScores[i].first)
            .arg(highScores[i].second);
    }

    // Create a QLabel to display the high scores
    highScoreLabel->setText(highScoresText);
    highScoreLabel->setFont(QFont("Press Start 2P", this->height() /40, QFont::Bold));
    highScoreLabel->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 0.5);");
    highScoreLabel->setAlignment(Qt::AlignCenter);
    highScoreLabel->setFixedSize(this->width() * 0.245, this->height() * 0.25);
    highScoreLabel->move(this->width() * 0.74, this->height() * 0.25);
    highScoreLabel->show();
}

bool GameController::isHighScore(int score) {
    // Retrieve the top 5 high scores
    QList<QPair<QString, int>> highScores = dbHelper::getTopHighScores(5);

    // Check if the new score is higher than the lowest score in the list
    return highScores.size() < 5 || score > highScores.last().second;
}

QString GameController::promptForHighScoreName() {
    bool ok;
    QInputDialog inputDialog(this);
    inputDialog.setWindowTitle(tr("New High Score!"));
    inputDialog.setLabelText(tr("Enter your initials:"));
    inputDialog.setTextValue("");

    // Custom StyleSheet
    QString inputDialogStyle = R"(
        QDialog {
            background-color: rgba(0, 0, 0, 0.5);
            color: white;
        }
        QLabel {
            color: white;
            font-family: 'Press Start 2P';
            font-size: 30px;
        }
        QLineEdit {
            background-color: white;
            color: black;
            font-family: 'Press Start 2P';
            font-size: 30px;
        }
        QPushButton {
            background-color: white;
            color: black;
            font-family: 'Press Start 2P';
            font-size: 30px;
        }
    )";
    inputDialog.setStyleSheet(inputDialogStyle);

    // Set custom font
    QFont font("Press Start 2P", this->height() * 0.1, QFont::Bold);
    inputDialog.setFont(font);

    if (inputDialog.exec() == QDialog::Accepted) {
        QString name = inputDialog.textValue();
        if (!name.isEmpty()) {
            return name.left(3).toUpper();
        }
    }
    return promptForHighScoreName();
}
