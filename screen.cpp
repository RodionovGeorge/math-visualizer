#include "screen.h"
#include <qnamespace.h>

void Screen::changeAlgorithm(AlgorithmID id)
{
    al.reset(Algorithm::createAlgorithm(id));
    updateScreen();
}

Screen::Screen(int const WIDTH, int const HEIGHT, QWidget *parent):QWidget(parent), WIDTH(WIDTH), HEIGHT(HEIGHT) 
{
    image = std::make_unique<QImage>(WIDTH, HEIGHT, QImage::Format_RGB32);
    image->fill(Qt::white);
    label = new QLabel();
    workingMode = false;
    al = std::unique_ptr<Algorithm>(new IPL);
    setFixedSize(WIDTH, HEIGHT);
    setFocusPolicy(Qt::StrongFocus);
    choosenPointNumber = -1;
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(label);
    setLayout(layout);
    updateScreen();
}

void Screen::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        if ((choosenPointNumber != -1) && workingMode) {
            QPoint p = al->transformCoordinates(event->pos(), WIDTH, HEIGHT);
            points[choosenPointNumber].setX(p.x());
            points[choosenPointNumber].setY(p.y());
            updateScreen();
        }
    }
    QWidget::mouseMoveEvent(event);
}

void Screen::updateAxis()
{
    QPainter painter(image.get());
    QPen p(Qt::black);
    painter.setPen(p);
    QPoint xFirstPoint = al->reverseTransformCoordinates(QPoint(0, HEIGHT / 2), WIDTH, HEIGHT);
    QPoint xSecondPoint = al->reverseTransformCoordinates(QPoint(0, -HEIGHT / 2), WIDTH, HEIGHT);
    QPoint yFirstPoint = al->reverseTransformCoordinates(QPoint(-WIDTH / 2, 0), WIDTH, HEIGHT);
    QPoint ySecondPoint = al->reverseTransformCoordinates(QPoint(WIDTH / 2, 0), WIDTH, HEIGHT);
    painter.drawLine(xFirstPoint, xSecondPoint);
    painter.drawLine(yFirstPoint, ySecondPoint);
}

void Screen::addPoint(QPoint const &p)
{
    Point<int> newP(p.x(), p.y(), points.size());
    points.push_back(std::move(newP));
}

void Screen::deletePoint(QPoint const &p)
{
    for (size_t i = 0; i < points.size(); i++) {
        if (points[i].checkClick(p)) {
            points.erase(points.begin()  + i);
            for (size_t j = i; j < points.size(); j++) {
                points[j].setNumber(j);
            }
            break;
        }
    }
}


void Screen::drawLines()
{
    size_t pointsNumber = points.size();
    QPainter painter(image.get());
    painter.setPen(Qt::red);
    for (size_t i = 0; i < pointsNumber - 1; i++) {
        painter.drawLine(al->reverseTransformCoordinates(points[i], WIDTH, HEIGHT),
                         al->reverseTransformCoordinates(points[i + 1], WIDTH, HEIGHT));
    }
}

void Screen::mousePressEvent(QMouseEvent *event)
{
    if ((event->button() == Qt::LeftButton) && !workingMode)
        addPoint(al->transformCoordinates(event->pos(), WIDTH, HEIGHT));
    if ((event->button() == Qt::LeftButton) && workingMode) {
        for (size_t i = 0; i < points.size(); i++) {
            if (points[i].checkClick(al->transformCoordinates(event->pos(), WIDTH, HEIGHT))) { 
                choosenPointNumber = i;
                break;
            } else
                choosenPointNumber = -1;
        }
    }
    if (event->button() == Qt::RightButton)
        deletePoint(al->transformCoordinates(event->pos(), WIDTH, HEIGHT));
    updateScreen();
    QWidget::mousePressEvent(event);
}

void Screen::drawPoint(Point<int> const &point, Qt::GlobalColor c = Qt::red)
{
    QPainter painter(image.get());
    painter.setPen(QPen(c));
    painter.setBrush(QBrush(c, Qt::SolidPattern));
    painter.drawEllipse(al->reverseTransformCoordinates(point, WIDTH, HEIGHT), RADIUS, RADIUS);
}

void Screen::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Shift) {
        workingMode = true;
    }
    QWidget::keyPressEvent(event);
}

void Screen::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Shift) {
        workingMode = false;
    }
    QWidget::keyReleaseEvent(event);
}

void Screen::updateScreen()
{
    image->fill(Qt::white);
    updateAxis();
    for (auto &p:points) {
        drawPoint(p);
    }
    if (points.size() > 1) {
        // drawLines();
        al->update(points);
        al->draw(image, Qt::blue);
    }
    label->setPixmap(QPixmap::fromImage(*image));
}

void Screen::deletePoints()
{
    points.clear();
    updateScreen();
}
