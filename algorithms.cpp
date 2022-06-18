#include "algorithms.h"

Algorithm* Algorithm::createAlgorithm(AlgorithmID id)
{
    Algorithm *al;
    switch (id)
    {
        case AlgorithmID::Spline:
            al = new Spline();
            break;
        case AlgorithmID::IPL:
            al = new IPL();
            break;
        default:
            throw std::logic_error("Wrond class ID");
    }
    return al;
}
std::vector<Matrix<double>> Spline::findDerivative(std::vector<double> tArray, std::vector<Point<int>> const &points)
{
    Matrix<double> lhsMatrix(tArray.size(), tArray.size(), 0.);
    Matrix<Matrix<double>> rhsMatrix(tArray.size(), 1, Matrix<double>(1, 2, 0));
    Matrix<double> tmp(1, 2, 0);
    std::vector<Matrix<double>> result(tArray.size(), Matrix<double>(1, 2, 0));

    lhsMatrix[0][0] = 1; //Start condition
    lhsMatrix[tArray.size() - 1][tArray.size() - 1] = 1; //too
    for (size_t i = 1; i < tArray.size() - 1; i++) {
        lhsMatrix[i][i - 1] = tArray[i + 1];
        lhsMatrix[i][i] = 2 * (tArray[i] + tArray[i + 1]);
        lhsMatrix[i][i + 1] = tArray[i - 1];
    }
    //Start condition
    rhsMatrix[0][0][0][0] = 1;
    rhsMatrix[0][0][0][1] = 1;
    rhsMatrix[tArray.size() - 1][0][0][0] = 1;
    rhsMatrix[tArray.size() - 1][0][0][1] = 1;

    for (size_t i = 1; i < tArray.size() - 1; i++) {
        rhsMatrix[i][0] = points[i + 1].getCoordinates() - points[i].getCoordinates();
        rhsMatrix[i][0] = rhsMatrix[i][0] * tArray[i] * tArray[i];
        tmp = points[i].getCoordinates() - points[i - 1].getCoordinates();
        tmp = tmp * tArray[i + 1] * tArray[i + 1];
        rhsMatrix[i][0] = rhsMatrix[i][0];
        rhsMatrix[i][0] = rhsMatrix[i][0] / tArray[i] / tArray[i + 1] * 3;
    }

    for (int i = 1; i < lhsMatrix.getRowsNumber(); i++) {
        lhsMatrix[i][i] = lhsMatrix[i][i] - lhsMatrix[i - 1][i] * lhsMatrix[i][i - 1] / lhsMatrix[i - 1][i - 1];
        rhsMatrix[i][0] = rhsMatrix[i][0] - rhsMatrix[i - 1][0] * lhsMatrix[i][i - 1] / lhsMatrix[i - 1][i - 1];
        lhsMatrix[i][i - 1] = 0;
    }

    result[result.size() - 1] = rhsMatrix[result.size() - 1][0] / lhsMatrix[result.size() - 1][result.size() - 1];
    for (int i = result.size() - 2; i >= 0; i--) {
        result[i] = (rhsMatrix[i][0] - result[i + 1] * lhsMatrix[i][i + 1]) / lhsMatrix[i][i];
    }
    return result;
}

void Spline::update(const std::vector<Point<int>> &points)
{
    size_t segmentsCount = points.size() - 1;
    size_t i;
    tArray.resize(points.size());
    std::vector<Matrix<double>> fDerivativeArray;
    segments.resize(segmentsCount, Matrix<Matrix<double>>(1, 4, Matrix<double>(1, 2, 0)));
    tArray[0] = 0.;
    for (i = 1; i < points.size(); i++) {
        tArray[i] = qSqrt(qPow(points[i].getX() - points[i - 1].getX(), 2) +
                          qPow(points[i].getY() - points[i - 1].getY(), 2));
    }
    fDerivativeArray = findDerivative(tArray, points);
    for (i = 0; i < segmentsCount; i++) {
        segments[i][0][0] = points[i].getCoordinates();
        segments[i][0][1] = fDerivativeArray[i];
        segments[i][0][2] = points[i + 1].getCoordinates() - points[i].getCoordinates();
        segments[i][0][2] = segments[i][0][2] * 3 / tArray[i + 1] / tArray[i + 1] - fDerivativeArray[i] * 2 / tArray[i + 1]
                            - fDerivativeArray[i + 1] / tArray[i + 1];
        segments[i][0][3] = points[i].getCoordinates() - points[i + 1].getCoordinates();
        segments[i][0][3] = segments[i][0][3] * 2 / tArray[i + 1] / tArray[i + 1] / tArray[i + 1] + fDerivativeArray[i]  / tArray[i + 1] / tArray[i + 1]
                            + fDerivativeArray[i + 1] / tArray[i + 1] / tArray[i + 1];
    }
}

QPoint Spline::transformCoordinates(QPoint const &point, int const imageWidth, int const imageHeight) const
{
    return QPoint(point.x() - imageWidth / 2, imageHeight / 2 - point.y()); 
}

QPoint Spline::reverseTransformCoordinates(QPoint const &point, int const imageWidth, int const imageHeight) const
{
    return QPoint(point.x() + imageWidth / 2, imageHeight / 2 - point.y()); 
}

void Spline::draw(std::unique_ptr<QImage> const & image, Qt::GlobalColor const &c)
{
    double tx, ty;
    QPainter p(image.get());
    p.setPen(QPen(c));
    p.setBrush(QBrush(c, Qt::SolidPattern));
    for (size_t i = 0; i < tArray.size() - 1; i++) {
        for (int t = 0; t < static_cast<int>(tArray[i + 1]); t++) {
            tx = segments[i][0][0][0][0] + segments[i][0][1][0][0] * t + segments[i][0][2][0][0] * t * t + segments[i][0][3][0][0] * t * t * t;
            ty = segments[i][0][0][0][1] + segments[i][0][1][0][1] * t + segments[i][0][2][0][1] * t * t + segments[i][0][3][0][1] * t * t * t;
            p.drawEllipse(reverseTransformCoordinates(QPoint(tx, ty), image->width(), image->height()), POINT_RADIUS, POINT_RADIUS);
        }
    }
}

QPoint IPL::transformCoordinates(QPoint const &point, int const imageWidth, int const imageHeight) const
{
    return QPoint(point.x() - imageWidth / 2, imageHeight / 2 - point.y()); 
}

QPoint IPL::reverseTransformCoordinates(QPoint const &point, int const imageWidth, int const imageHeight) const
{
    return QPoint(point.x() + imageWidth / 2, imageHeight / 2 - point.y()); 
}

void IPL::update(std::vector<Point<int>> const &p)
{
    points.resize(p.size());
    for (size_t i = 0; i < points.size(); i++) {
        points[i] = p[i];
    }
}

double IPL::findValue(int x) const
{
    uint64_t i, j;
    double t, s;
    s = 0.;
    for (i = 0; i < points.size(); i++) {
        t = 1.;
        for (j = 0; j < points.size(); j++) {
            if (i != j) {
                t = t * (x - points[j].x()) / (points[i].x() - points[j].x());
            }
        }
        s = s + t * points[i].y();
    }
    return s;
}

void IPL::draw(std::unique_ptr<QImage> const &image, Qt::GlobalColor const &c)
{
    QPainter p(image.get()); 
    for (int64_t x = -image->width() / 2; x <= image->width() / 2 - 1; x++) {
        p.drawLine(reverseTransformCoordinates(QPoint(x, std::round(findValue(x))), image->width(), image->height()),
                   reverseTransformCoordinates(QPoint(x + 1, std::round(findValue(x + 1))), image->width(), image->height()));
    } 
}
