

//========================================================================
#include "widget.h"
#include <QApplication>
#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include "loggertool.h"

class TestClass{

public:
    void badMethod(){
        char * p = NULL;
        *p = 0;
    }
};

void bad_foo(){
    TestClass testClass;
    testClass.badMethod();
}

int main(int argc, char *argv[]){

    QApplication a(argc, argv);

    LoggerTool loggerSet;
    loggerSet.initLog(LoggerTool::AppOutputLog | LoggerTool::UserEventLog |
                      LoggerTool::CrashLog);

    // test crash
    bad_foo();


    Widget w;
    w.show();

    return a.exec();
}

