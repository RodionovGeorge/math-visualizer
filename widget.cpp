#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    screen = new Screen(1000, 900);
    setFixedSize(1010, 1010);
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    QHBoxLayout *hLayout = new QHBoxLayout(this);
    QRadioButton *splineRB = new QRadioButton("Spline");
    QRadioButton *iplRB = new QRadioButton("IPL");
    iplRB->setChecked(true);
    hLayout->addWidget(splineRB);
    hLayout->addWidget(iplRB);
    QPushButton *bClear = new QPushButton("Clear");
    vLayout->setContentsMargins(5,5,5,5);
    connect(bClear, &QPushButton::clicked, screen, &Screen::deletePoints);
    connect(splineRB, &QRadioButton::toggled, this, &Widget::splineButton);
    connect(iplRB, &QRadioButton::toggled, this, &Widget::IPLButton);
    connect(this, &Widget::splineSignal, screen, &Screen::changeAlgorithm);
    connect(this, &Widget::IPLSignal, screen, &Screen::changeAlgorithm);
    vLayout->addWidget(bClear);
    vLayout->addLayout(hLayout);
    vLayout->addWidget(screen);
    setLayout(vLayout);
}

void Widget::splineButton(bool checked)
{
    if (checked) {
        emit splineSignal(AlgorithmID::Spline);
    }
}

void Widget::IPLButton(bool checked)
{
    if (checked) {
        emit splineSignal(AlgorithmID::IPL);
    }
}

Widget::~Widget()
{
    delete screen;
}
