#ifndef POINT_H
#define POINT_H

#include "matrix.h"
#include <QPainter>
#include <QRect>

const int RADIUS = 5;

template<typename T>
class Point
{
private:
    Matrix<T> *coordinates;
    int number;

public:
    Point():coordinates(new Matrix<T>()), number(0)
    {
    }

    Point(T const & x, T const & y, int pointNumber):coordinates(new Matrix<T>(1, 2)), number(pointNumber)
    {
        (*coordinates)[0][0] = x;
        (*coordinates)[0][1] = y;
    }

    Point(Point const &obj) = delete;
    Point& operator=(Point const&) = delete;
    Point& operator=(Point &&p)
    {
        coordinates = p.coordinates;
        number = p.number;
        p.number = 0;
        p.coordinates = nullptr;
        return *this;
    }

    operator QPoint() const
    {
        return QPoint(getX(), getY());
    }

    Point(Point &&obj):coordinates(obj.coordinates), number(obj.number)
    {
        obj.number = 0;
        obj.coordinates = nullptr;
    }

    T getX() const
    {
        return (*coordinates)[0][0];
    }

    T getY() const
    {
        return (*coordinates)[0][1];
    }

    void setX(T const & x)
    {
        (*coordinates)[0][0] = x;
    }

    void setY(T const & y)
    {
        (*coordinates)[0][1] = y;
    }

    Matrix<T> getCoordinates() const
    {
        return *coordinates;
    }

    void setCoordinates(Matrix<T> const &c)
    {
        *coordinates = c;
    }

    bool checkClick(QPoint const &p)
    {
        bool result = false;
        if ((p.x() - (*coordinates)[0][0]) * (p.x() - (*coordinates)[0][0]) +
            (p.y() - (*coordinates)[0][1]) * (p.y() - (*coordinates)[0][1]) <= RADIUS * RADIUS)
            result = true;
        return result;
    }

    int getNumber() const noexcept
    {
        return number;
    }

    void setNumber(int newNumber)
    {
        number = newNumber;
    }

    ~Point()
    {
        delete coordinates;
    }
};

#endif // POINT_H
