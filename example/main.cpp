#include "widget.h"
#include <QApplication>
#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include "loggertool.h"

void foo_bad(){
    int *foo = (int*)-1;
    printf("%d\n", *foo);
}

int main(int argc, char *argv[]){

    QApplication a(argc, argv);

    LoggerTool loggerSet;
    loggerSet.initLog(LoggerTool::AppOutputLog | LoggerTool::UserEventLog |
                      LoggerTool::CrashLog);

    foo_bad();


    Widget w;
    w.show();

    return a.exec();
}
