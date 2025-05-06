#pragma once

#include "Enemy.h"

class Coily : public Enemy {
    Q_OBJECT

public:
    explicit Coily(QWidget* parent = nullptr);
    void setScale(int width, int height) override;
    void setDirection(const QString& direction) override;
    QString getDirection() const override;
    void setHorizonVelo(double d) override;
    double getHorizonVelo() const override;
    void swapPixmap();
	bool getIsCoilyForm() const { return this->isCoilyForm; }

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QPixmap coilyImage;
    QPixmap scaledCoilyImage;
    QString direction;
    double horizontalVelo;
    bool isCoilyForm = false;
};
