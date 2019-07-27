#include <QDateTime>
#include <QApplication>
#include <QDebug>
#include <QMouseEvent>

#include "loggertoolglobal.h"
#include "usereventlogger.h"

QFile UserEventLogger::logFile;
QTextStream UserEventLogger::outTextStream;
QString UserEventLogger::separator = ";";

UserEventLogger::UserEventLogger()
{

}
//=========================================================================
UserEventLogger::~UserEventLogger(){

    stopLog();
}
//=========================================================================
void UserEventLogger::startLog(const QString &logFileName){

    if(!m_isStarted){
        logFile.setFileName(logFileName/*createFileNameAndDir("events.csv")*/);
        if (logFile.open(QFile::WriteOnly | QFile::Truncate)){
            outTextStream.setDevice(&logFile);

            // write first row (headers)
            outTextStream << "time"
                          << separator << "event_type"
                          << separator << "button_or_key"
                          << separator << "modifiers"
                          << separator << "local_ckick_position"
                          << separator << "global_ckick_position"
                          //<< separator << "receiver"
                          << separator << "full_path" << endl; // 7 columns

            // register callback
            QInternal::registerCallback(QInternal::EventNotifyCallback, eventCallback);
            m_isStarted = true;
        }
    }
}
//=========================================================================
void UserEventLogger::stopLog(){

    if(m_isStarted){
        QInternal::unregisterCallback(QInternal::EventNotifyCallback, eventCallback);
        logFile.close();
        outTextStream.reset();
        m_isStarted = false;
    }
}
//=========================================================================
//DragEnter = 60,                         // drag moves into widget
//DragMove = 61,                          // drag moves in widget
//DragLeave = 62,                         // drag leaves or is cancelled
//Drop = 63,                              // actual drop
//DragResponse = 64,                      // drag accepted/rejected
bool UserEventLogger::eventCallback(void** data){

    QObject* receiver = reinterpret_cast<QObject*>(data[0]);
    QEvent* event = reinterpret_cast<QEvent*>(data[1]);
//
    // getFullObjectPath
    QString fullObjectPath = LoggerToolGlobal::getFullObjectPath(receiver);
//
    //  mouse events
    if(event->type() == QEvent::MouseButtonPress ||
            event->type() == QEvent::MouseButtonRelease ||
            event->type() == QEvent::MouseButtonDblClick){

        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        outTextStream << QTime::currentTime().toString("HH:mm:ss:zzz")
                << separator << event->type()
                << separator << mouseEvent->button()
                << separator << mouseEvent->modifiers()
                << separator << mouseEvent->pos().x() << ":" << mouseEvent->pos().y()
                << separator << mouseEvent->globalPos().x() << ":" << mouseEvent->globalPos().y()
                << separator << fullObjectPath;
        //qDebug() << (int)event->type() << "fullObjectPath" << fullObjectPath << mouseEvent->pos();
        outTextStream << endl;
    }

    // key events
    else if(event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease){

        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        outTextStream << QTime::currentTime().toString("HH:mm:ss:zzz")
                << separator <<  event->type()
                << separator << keyEvent->key()
                << separator
                << separator
                << separator
                << separator << fullObjectPath;
        outTextStream << endl;
        //qDebug() << (int)event->type() << "fullObjectPath" << fullObjectPath << keyEvent->key();
    }

    // drag and drop
    else if(event->type() == QEvent::DragEnter || event->type() == QEvent::DragLeave ||
            event->type() == QEvent::Drop){

        //qDebug() << "DrafDropEvent" << event->type();
    }

    return false;
}
//=========================================================================

