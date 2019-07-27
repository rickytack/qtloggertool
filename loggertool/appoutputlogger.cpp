#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <qglobal.h>

#include <sstream>
#include <iostream>

#include "appoutputlogger.h"

QStringList AppOutputLogger::message_type =
{
    "[D] ", //QtDebugMsg
    "[W] ", //QtWarningMsg
    "[C] ", //QtCriticalMsg
    "[F] ", //QtFatalMsg
    "[I] ", //QtInfoMsg
    "[S] "  //QtSystemMsg
};

QFile AppOutputLogger::logFile;
QMutex AppOutputLogger::fileMutex;
QTextStream AppOutputLogger::outTextStream;
bool AppOutputLogger::showPlace;

//========================================================================
AppOutputLogger::AppOutputLogger(){


}
//========================================================================
AppOutputLogger::~AppOutputLogger(){

    stopLog();
}
//========================================================================
void AppOutputLogger::startLog(const QString &logFileName, const QString &appName,
                               const QString &appVersion){

    if(!m_isStarted){
        showPlace = false;
#       if QT_VERSION >= 0x050000
        qInstallMessageHandler(messageOutput);
#       else
        qInstallMsgHandler(messageOutput);
#       endif

        // open log file
        logFile.setFileName(logFileName/*createFileNameAndDir("appoutput.txt")*/);
        if (logFile.open(QFile::WriteOnly | QFile::Truncate)){
            outTextStream.setDevice(&logFile);
        }
        else qWarning() << "AppOutputLogger::startLog: can't open file";

        qDebug() << "----------------------------------------";
        qDebug() << appName << " version " << appVersion;
        qDebug() << "----------------------------------------";
        qDebug() << "Build: " __DATE__ " at " __TIME__;
        qDebug() << "Based on Qt " << QString(QT_VERSION_STR);
        qDebug() << "Run: " << QTime::currentTime().toString();
        showPlace = true;
        m_isStarted = true;
    }
}
//========================================================================
void AppOutputLogger::stopLog(){

    if(m_isStarted){
#   if QT_VERSION >= 0x050000
        qInstallMessageHandler(0);
#   else
        qInstallMsgHandler(0);
#   endif
        logFile.close();
        m_isStarted = false;
    }
}
//========================================================================
#if QT_VERSION >= 0x050000
void AppOutputLogger::messageOutput(QtMsgType type, const QMessageLogContext &context,
                                    const QString &msg){

    QMutexLocker locker(&fileMutex);
    QString strData;
    QString strMessage = message_type[type] + QTime::currentTime().toString() + "  " + msg;
    if(showPlace){
        QString strPlace = QString(" ") + QString(context.file) + ":"
                + QString::number(context.line);
        strData = strMessage + strPlace + "\n";
    }
    else strData = strMessage + "\n";

    std::cerr << strData.toStdString();
    if ( logFile.isOpen() ){
        outTextStream << strData;
        outTextStream.flush();
    }
}
#else
void AppOutputLogger::messageOutput(QtMsgType type, const char* msg){

    QMutexLocker locker(&fileMutex);
    QString strData = message_type[type] + QTime::currentTime().toString()+ " " + msg + "\n";

    char *rawData = strData.toUtf8().data();
    std::cerr << rawData;
    if ( logFile.isOpen() ){
//        logFile.write(rawData, strData.size());
//        logFile.flush();

        outTextStream << strData;
        outTextStream.flush();
    }
}
#endif
