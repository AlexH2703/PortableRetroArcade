// CubeWidget.h
#ifndef CUBEWIDGET_H
#define CUBEWIDGET_H

#include <QWidget>
#include "Cube.h"
#include "Qbert.h"

class CubeWidget : public QWidget {
    Q_OBJECT

public:
    CubeWidget(QWidget* parent = nullptr);
    void setPyramidSize(int size);
    QVector<Cube*> getCubes() const { return cubes; }
    void arrangeCubes();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:

    int pyramidSize;
    QVector<Cube*> cubes;
};

#endif // CUBEWIDGET_H
