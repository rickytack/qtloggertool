/****************************************************************************
**
** Copyright (C) 2017 Ricky Tack
** Contact: rickytack9@gmail.com
**
****************************************************************************/

#include <QDateTime>
#include <QApplication>
#include <QDebug>
#include <QMouseEvent>

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
                          << separator << "ckick_position"
                          << separator << "receiver"
                          << separator << "parent_of_receiver" << endl; // 7 columns

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
bool UserEventLogger::eventCallback(void** data){

    QObject* receiver = reinterpret_cast<QObject*>(data[0]);
    QEvent* event = reinterpret_cast<QEvent*>(data[1]);

    //  mouse events
    if(event->type() == QEvent::MouseButtonPress ||
            event->type() == QEvent::MouseButtonRelease ||
            event->type() == QEvent::MouseButtonDblClick){

        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        outTextStream << QTime::currentTime().toString()
                      << separator << event->type()
                      << separator << mouseEvent->button()
                      << separator << mouseEvent->modifiers()
                      << separator << mouseEvent->pos().x() << ":" << mouseEvent->pos().y()
                      << separator << receiver->objectName();
        QObject *parent = receiver->parent();
        if(parent) outTextStream << separator << parent->objectName();
        else outTextStream << separator << "Unknown";
        outTextStream << endl;
    }

    // key events
    else if(event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease){

        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        outTextStream << QTime::currentTime().toString()
                      << separator <<  event->type()
                      << separator << keyEvent->key()
                      << separator
                      << separator
                      << separator << receiver->objectName();
        QObject *parent = receiver->parent();
        if(parent) outTextStream << separator << parent->objectName();
        else outTextStream << separator << "Unknown";
        outTextStream << endl;
    }
    return false;
}
