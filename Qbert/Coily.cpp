#include "Coily.h"
#include <QPainter>

Coily::Coily(QWidget* parent)
    : Enemy(parent),
    coilyImage(":/Qbert/coilyBall.png"), // Load the Coily sprite map
    horizontalVelo(0.0) {
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

void Coily::setScale(int width, int height) {
    int targetHeight = isCoilyForm ? height * 2 : height; 
    int targetWidth = (coilyImage.width() * targetHeight) / coilyImage.height(); // Maintain aspect ratio
    scaledCoilyImage = coilyImage.scaled(targetWidth, targetHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    resize(scaledCoilyImage.size());
}

void Coily::setDirection(const QString& direction) {
    this->direction = direction;
}

QString Coily::getDirection() const {
    return direction;
}

void Coily::setHorizonVelo(double d) {
    horizontalVelo = d;
}

double Coily::getHorizonVelo() const {
    return horizontalVelo;
}

void Coily::swapPixmap() {
    if (!isCoilyForm) {
        // Switch to coily.png (snake form)
        this->coilyImage.load(":/Qbert/coily.png");
        isCoilyForm = true; // Update the flag

        // Determine the target height
        int targetHeight = isCoilyForm ? this->height() * 2 : this->height();
        int targetWidth = (this->coilyImage.width() * targetHeight) / this->coilyImage.height(); // Maintain aspect ratio

        // Scale the image
        this->scaledCoilyImage = this->coilyImage.scaled(targetWidth, targetHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // Resize the widget to match the new image size
        this->resize(scaledCoilyImage.size());

        // Adjust the vertical position based on the form
        int verticalOffset = isCoilyForm ? -this->height() / 2 : this->height(); // Raise in snake form, lower in ball form
        this->move(this->x(), this->y() + verticalOffset);

        // Update the widget to repaint with the new image
        this->update();
    }

}


void Coily::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, scaledCoilyImage);

}
