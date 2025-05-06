#pragma once  

#include <QtWidgets/QMainWindow>  
#include <QVariantAnimation>  
#include "ui_Qbert.h"  
#include "CubeWidget.h"  
#include "Qbert.h"  
#include <QLabel>  
#include <Disc.h>  
#include "RedBall.h"  
#include <Coily.h>  

class GameController : public QMainWindow  
{  
   Q_OBJECT  

public:  
   // Constructor and Destructor  
   GameController(QWidget* parent = nullptr);  
   ~GameController();  
   void startSpawnPattern(int round);  
   void spawnNextEnemy();  
   void addDiscs(int count);  

protected:  
   // Handles key press events for Qbert's movement  
   void keyPressEvent(QKeyEvent* event) override;  

private slots:  
   // Periodically checks Qbert's position on the cubes  
   void updateGame();  

private:  
   void moveQbert(const QString& direction);  
   void resetQbert();
   void resetAfterQbertFall();
   void animateQbertMovement(const QPoint& startPos, const QPoint& endPos);  
   Cube* findQbertCube();
   Cube* findEnemyCube(Enemy* enemy);
   void fallQbertToBottom(double horizontalVelocity, QEasingCurve::Type curveType, double easedYStart);  
   void updateGameInfoLabel();  
   void checkAndLevelUp();  
   void handleGameOver();  
   void animateQbertAndDiscToTop(Disc* disc);  
   void spawnRedBall();  
   void animateEnemy(Enemy* redball, const QPoint& startPos, const QPoint& endPos);  
   void fallEnemyToBottom(Enemy* enemy, double horizontalVelocity, QEasingCurve::Type curveType, double easedYStart);  
   void spawnCoily();  
   void pauseAnimations();  
   void resumeAnimations();  
   void startCoilyChase(Coily* coily);  
   QString calculateShortestDirection(const QPoint& coilyPos, const QPoint& qbertPos);  
   void moveCoily(Coily* coily, const QString& direction);  
   void resetAfterCoilyCatch();

   // UI and game components  
   Ui::QbertClass ui;  
   CubeWidget* cubeWidget;    
   Qbert* qbert;      
   QTimer* positionCheckTimer;   
   QTimer* enemySpawnTimer;  
   QTimer* coilySpawnTimer;  
   QLabel* pausedLabel;  
   QLabel* controlsLabel;  
   QLabel* gameInfoLabel;  
   QLabel* gameOverLabel;  
   QVector<Disc*> discs;  
   QMap<int, QVector<QPair<int, QString>>> discPlacements;  
   QVector<Enemy*> enemies;  
   Coily* coily;  
   QList<QVariantAnimation*> activeAnimations;   

   // Game state variables  
   int screenWidth;       
   int screenHeight;  
   bool isJumping = false;    
   bool coilyChasing = false;  
   QString queuedDirection;   
   int lives = 3;  
   int level = 1;  
   int score = 0;  
   Cube* currentCube;  
   bool paused = false;
};
