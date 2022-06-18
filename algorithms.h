#ifndef SPLINE_H
#define ALGORITHMS_H

#include <QImage>
#include <QPainter>
#include <QtMath>
#include "point.h"
#include <qnamespace.h>
#include <utility>
#include <vector>
#include <stdexcept>

enum class AlgorithmID
{
    Spline,
    IPL
};

class Algorithm
{
public:
    virtual void update(std::vector<Point<int>> const &) = 0;
    virtual void draw(std::unique_ptr<QImage> const &, Qt::GlobalColor const & c = Qt::red) = 0;
    virtual QPoint transformCoordinates(QPoint const &point, int const imageWidth, int const imageHeight) const = 0;
    virtual QPoint reverseTransformCoordinates(QPoint const &point, int const imageWidth, int const imageHeight) const = 0;
    static Algorithm* createAlgorithm(AlgorithmID id);
    virtual ~Algorithm() = default;
};

class Spline:public Algorithm
{
private:
    int const POINT_RADIUS = 2;
    std::vector<Matrix<Matrix<double>>> segments;
    std::vector<double> tArray;
    std::vector<Matrix<double>> findDerivative(std::vector<double> tArray, std::vector<Point<int>> const &points);
public:
    Spline() = default;
    void update(std::vector<Point<int>> const &) override;
    void draw(std::unique_ptr<QImage> const &, Qt::GlobalColor const &c = Qt::red) override;
    QPoint transformCoordinates(QPoint const &point, int const imageWidth, int const imageHeight) const override;
    QPoint reverseTransformCoordinates(QPoint const &point, int const imageWidth, int const imageHeight) const override;
    ~Spline() = default;
};

class IPL:public Algorithm
{
private:
    int const POINT_RADIUS = 2;
    std::vector<QPoint> points;
    double findValue(int x) const;
public:
    IPL() = default;
    void update(std::vector<Point<int>> const &) override;
    void draw(std::unique_ptr<QImage> const &, Qt::GlobalColor const & c = Qt::red) override;
    QPoint transformCoordinates(QPoint const & point, int const imageWidth, int const imageHeight) const override;
    QPoint reverseTransformCoordinates(QPoint const & point, int const imageWidth, int const imageHeight) const override;
    ~IPL() = default;
};

#endif // ALGORITHMS_H
