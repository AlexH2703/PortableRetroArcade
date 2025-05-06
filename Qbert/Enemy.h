#ifndef ENEMY_H
#define ENEMY_H

#include <QWidget>

class Enemy : public QWidget {
    Q_OBJECT

public:
    explicit Enemy(QWidget* parent = nullptr);
    virtual ~Enemy();

    void reachedBottom();

    virtual void setScale(int width, int height) = 0;
    virtual void setDirection(const QString& direction) = 0;
    virtual QString getDirection() const = 0;
    virtual void setHorizonVelo(double d) = 0;
    virtual double getHorizonVelo() const = 0;
    bool isAnimating = false;

signals:
    void reachedBottomSignal(); // Signal emitted when the enemy reaches the bottom
};

#endif // ENEMY_H
