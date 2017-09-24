#include <QDebug>

#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QGraphicsScene *scene = new QGraphicsScene;
    scene->addRect(20, 20, 40, 40, QPen(Qt::blue));

    ui->graphicsView->setScene(scene);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButtonTest_clicked(){

    qWarning() << "Widget::on_pushButtonTest_clicked";
}
