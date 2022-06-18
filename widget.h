#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include "screen.h"
#include <QVBoxLayout>
#include <QRadioButton>

class Widget : public QWidget
{
    Q_OBJECT

private:
    Screen *screen;

private slots:
    void splineButton(bool checked);
    void IPLButton(bool checked);
    
signals:
    void splineSignal(AlgorithmID id);
    void IPLSignal(AlgorithmID id);
    
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
};
#endif // WIDGET_H
