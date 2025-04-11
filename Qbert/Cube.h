#ifndef CUBE_H
#define CUBE_H

#include <QWidget>
#include <QPainter>

class Cube : public QWidget {
    Q_OBJECT

public:
    Cube(QWidget* parent = nullptr, int cubeWidth=100, int cubeHeight=100, int cubeDepth=100);
	int width() const { return cubeWidth; }
	int height() const { return cubeHeight; }
	int depth() const { return cubeDepth; }
	void setTopFaceColor(const QColor& color);

protected:
    void paintEvent(QPaintEvent* event) override;
private:
	int cubeWidth;
	int cubeDepth;
	int cubeHeight;
	QColor topFaceColor;
};

#endif // CUBE_H
