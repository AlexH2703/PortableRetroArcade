#ifndef DISC_H
#define DISC_H

#include <QWidget>
#include <QPixmap>

class Disc : public QWidget {
    Q_OBJECT

public:
    Disc(QWidget* parent = nullptr, int row = 0, const QString& side = "");
    void setScale(int width, int height);
    int getRow() const;
    QString getSide() const;
protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QPixmap discImage;
    QPixmap scaledDiscImage;
    int row;
    QString side;
};

#endif // DISC_H
