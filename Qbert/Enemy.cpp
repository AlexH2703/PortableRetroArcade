#include "Enemy.h"

Enemy::Enemy(QWidget* parent) : QWidget(parent) {}

Enemy::~Enemy() {}

void Enemy::reachedBottom() {
    emit reachedBottomSignal();
}
