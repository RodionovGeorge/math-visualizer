#ifndef MATRIX_H
#define MATRIX_H

#include <QVector>
#include <exception>
#include <iostream>
#include <type_traits>
#include <utility>

class DimensionError:public std::exception
{
public:
    const char* what() const noexcept override
    {
        return "Dimension error!";
    }
};

class RangeError:public std::exception
{
public:
    const char* what() const noexcept override
    {
        return "Range error!";
    }
};

template<typename T>
class Matrix
{
private:
    int rowsNumber;
    int columnsNumber;
    QVector<QVector<T>> matrix; //matrix[0] - first column
    

public:
    Matrix():rowsNumber(0), columnsNumber(0), matrix(QVector<QVector<T>>(0))
    {}

    Matrix(int r, int c, T const &value = T()):rowsNumber(r), columnsNumber(c), matrix(rowsNumber, QVector<T>(columnsNumber, value))
    {}

    Matrix(Matrix<T> const &obj):rowsNumber(obj.rowsNumber), columnsNumber(obj.columnsNumber), matrix(obj.matrix)
    {}

    Matrix(Matrix<T> &&obj)
    {
        matrix = std::move(obj.matrix);
        rowsNumber = obj.rowsNumber;
        columnsNumber = obj.columnsNumber;
    }

    template<typename S>
    operator Matrix<S>() const
    {
        Matrix<S> result(rowsNumber, columnsNumber);
        for (int i = 0; i < rowsNumber; i++) {
            for (int j = 0; j < columnsNumber; j++) {
                result[i][j] = static_cast<S>(matrix[i][j]);
            }
        }
        return result;
    }

    int getColumnsNumber() const noexcept
    {
        return columnsNumber;
    }

    int getRowsNumber() const noexcept
    {
        return rowsNumber;
    }

    void setColumnsNumber(int const columns)
    {
        columnsNumber = columns;
        for (auto &row:matrix) {
            row.resize(columnsNumber);
        }
    }

    void setRowsNumber(int const rows)
    {
        rowsNumber = rows;
        matrix.resize(rowsNumber);
        for (auto &row:matrix) {
            if (row.size() != columnsNumber)
                row.resize(columnsNumber);
        }
    }

    void addColumn(QVector<T> const & column, int place = -1)
    {
        if (column.size() != rowsNumber)
            throw DimensionError();
        place = (place == -1) ? columnsNumber : place;
        for (int i = 0; i < rowsNumber; i++) {
            matrix[i].insert(place, column[i]);
        }
        columnsNumber++;
    }

    void addRow(QVector<T> const & row, int place = -1)
    {
        if (row.size() != columnsNumber)
            throw DimensionError();
        place = (place == -1) ? rowsNumber : place;
        matrix.insert(place, row);
        rowsNumber++;
    }

    void deleteRow(int const rowNumber)
    {
        if (rowNumber >= rowsNumber)
            throw RangeError();
        matrix.remove(rowNumber);
        rowsNumber--;
    }

    void deleteColumn(int const columnNumber)
    {
        if (columnNumber >= columnsNumber)
            throw RangeError();
        for (auto &row:matrix) {
            row.remove(columnNumber);
        }
        columnsNumber--;
    }

    Matrix<T>& operator=(Matrix<T> const &rhs)
    {
        matrix = rhs.matrix;
        rowsNumber = rhs.rowsNumber;
        columnsNumber = rhs.columnsNumber;
        return *this;
    }

    Matrix<T>& operator=(Matrix<T> &&rhs)
    {
        matrix = std::move(rhs.matrix);
        rowsNumber = rhs.rowsNumber;
        columnsNumber = rhs.columnsNumber;
        return *this;
    }

    /* template<typename S>
    Matrix<T>& operator=(Matrix<S> const &rhs)
    {
         for (size_t i = 0; i < rhs.getRowsNumber(); i++) {
             matrix[i].resize(rhs.getColumnsNumber());
             for (size_t j = 0; j < rhs.getColumnsNumber(); j++) {
                 matrix[i][j] = static_cast<T>(rhs[i][j]);
             }
         }
         rowsNumber = rhs.getRowsNumber();
         columnsNumber = rhs.getColumnsNumber();
         return *this;
    }*/

    template<typename S>
    auto operator *(Matrix<S> const &rhs) const
    {
        Matrix<decltype(std::declval<S>() * std::declval<T>())> result(rowsNumber, rhs.getColumnsNumber());
        if (columnsNumber != rhs.getRowsNumber())
            throw DimensionError();
        for (int i = 0; i < rowsNumber; i++) {
            for (int j = 0; j < rhs.getColumnsNumber(); j++) {
                for (int k = 0; k < columnsNumber; k++) {
                    result[i][j] = result[i][j] + matrix[i][k] * rhs[k][j];
                }
            }
        }
        return result;
    }

    template<typename S>
    auto operator*(S const &rhs)
    {
        Matrix<decltype(std::declval<S>() * std::declval<T>())> result(rowsNumber, columnsNumber);
        for (int i = 0; i < columnsNumber; i++) {
            for (int j = 0; j < rowsNumber; j++) {
                result[j][i] = matrix[j][i] * rhs;
            }
        }
        return result;
    }

    template<typename S>
    auto operator+(Matrix<S> const &rhs)
    {
        Matrix<decltype(std::declval<S>() + std::declval<T>())> result(rowsNumber, columnsNumber);
        if ((columnsNumber != rhs.getColumnsNumber()) || (rowsNumber != rhs.getRowsNumber()))
            throw DimensionError();
        for (int i = 0; i < columnsNumber; i++) {
            for (int j = 0; j < rowsNumber; j++) {
                result[j][i] = matrix[j][i] + rhs[j][i];
            }
        }
        return result;
    }

    template<typename S>
    auto operator+(S const &rhs)
    {
        Matrix<decltype(std::declval<S>() + std::declval<T>())> result(rowsNumber, columnsNumber);
        for (int i = 0; i < columnsNumber; i++) {
            for (int j = 0; j < rowsNumber; j++) {
                result[j][i] = matrix[j][i] + rhs;
            }
        }
        return result;
    }

    
    template<typename S>
    auto operator/(S const &rhs)
    {
        Matrix<decltype(std::declval<S>() / std::declval<T>())> result(rowsNumber, columnsNumber);
        for (int i = 0; i < columnsNumber; i++) {
            for (int j = 0; j < rowsNumber; j++) {
                result[j][i] = matrix[j][i] / rhs;
            }
        }
        return result;
    }

    template<typename S>
    Matrix<T> operator -(Matrix<S> const &rhs)
    {
        Matrix<decltype(std::declval<T>() - std::declval<S>())> result(rowsNumber, columnsNumber);
        if ((columnsNumber != rhs.columnsNumber) || (rowsNumber != rhs.rowsNumber))
            throw DimensionError();
        for (int i = 0; i < columnsNumber; i++) {
            for (int j = 0; j < rowsNumber; j++) {
                result[j][i] = matrix[j][i] - rhs[j][i];
            }
        }
        return result;
    }

    QVector<T>& operator[](int const i)
    {
        if (i >= rowsNumber)
            throw RangeError();
        return matrix[i];
    }


    QVector<T> operator[](int const i) const
    {
        if (i >= rowsNumber)
            throw RangeError();
        return matrix[i];
    }

    ~Matrix()
    {}
};

#endif // MATRIX_H
