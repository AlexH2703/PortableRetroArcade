#ifndef QBERT_H
#define QBERT_H

#include <QWidget>
#include <QPixmap>

class Qbert : public QWidget {
    Q_OBJECT

public:
    Qbert(QWidget* parent = nullptr);
    void setScale(int width, int height);
    QRect collisionRect();
    void setHorizonVelo(double d) { horizontalVelo = d; };
    double getHorizonVelo() { return horizontalVelo; };

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QPixmap qbertImage;
    QPixmap scaledQbertImage;
    double horizontalVelo;
};

#endif // QBERT_H
