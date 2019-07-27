#ifndef LOGGERTOOLGLOBAL_H
#define LOGGERTOOLGLOBAL_H

#include <QString>
#include <QObject>
#include <QApplication>
#include <QWidget>

namespace LoggerToolGlobal {

inline QString getFullObjectPath(const QObject *object){

    QString fullPath;

    if(object){
        fullPath = object->objectName();

        QObject *parent = object->parent();
        while(parent){
            fullPath.prepend(parent->objectName() + "/");
            parent = parent->parent();
        }
    }
    return fullPath;
}

inline QWidget *findWidgetByFullObjectPath(const QString &fullObjectPath){

    QWidgetList widgetList = QApplication::allWidgets();
    auto iterator = std::find_if(widgetList.constBegin(), widgetList.constEnd(),
                 [&](const QWidget *widget)
                 { return (LoggerToolGlobal::getFullObjectPath(widget) == fullObjectPath);});

    if(iterator != widgetList.constEnd()) return *iterator;
    else return nullptr;
}

}

#endif // LOGGERTOOLGLOBAL_H
