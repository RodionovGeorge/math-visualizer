#ifndef SCREEN_H
#define SCREEN_H

#include <QWidget>
#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <vector>
#include <QMouseEvent>
#include <memory>
#include "point.h"
#include "algorithms.h"

class Screen:public QWidget
{
    Q_OBJECT

private:
    int WIDTH;
    int HEIGHT;
    std::vector<Point<int> > points;
    std::unique_ptr<QImage> image;
    QLabel *label;
    QPoint dragPos;
    std::unique_ptr<Algorithm> al;
    bool workingMode;
    int choosenPointNumber;
    void addPoint(QPoint const &p);
    void deletePoint(QPoint const &p);
    void updateAxis();
    void drawLines();

public:
    Screen(int const WIDTH, int const HEIGHT, QWidget* parent = nullptr);
    void drawPoint(Point<int> const &, Qt::GlobalColor);
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void updateScreen();
    ~Screen() = default;
        
public slots:
    void deletePoints();
    void changeAlgorithm(AlgorithmID id);
};

#endif // SCREEN_H
