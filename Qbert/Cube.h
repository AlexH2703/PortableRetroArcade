#ifndef CUBE_H
#define CUBE_H

#include <QWidget>
#include <QPainter>

class Cube : public QWidget {
    Q_OBJECT

public:
    Cube(QWidget* parent = nullptr, int cW = 100, int cH = 100, int d = 100);
    int width() const { return cubeWidth; };
    int height() const { return cubeHeight; };
    int depth() const { return cubeDepth; };
    void setTopFaceColor(const QColor& color);
    QColor getTopFaceColor() const;
    void paintEvent(QPaintEvent* event) override;

    int setState(int level); // Update state based on level
    void resetState();        // Reset state to the initial state

private:
    int cubeWidth;
    int cubeDepth;
    int cubeHeight;
    QColor topFaceColor;
    QColor startColor;  // Start color
    QColor middleColor; // Middle color
    QColor goalColor;   // Goal color
    int state;          // Current state of the cube
};
#endif // CUBE_H