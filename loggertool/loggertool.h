/****************************************************************************
**
** Copyright (C) 2017 Ricky Tack
** Contact: rickytack9@gmail.com
**
****************************************************************************/

#ifndef LOGGERSET_H
#define LOGGERSET_H

#include <QString>

#include "usereventlogger.h"
#include "appoutputlogger.h"

class LoggerTool
{
public:
    enum LoggerType{
        AppOutputLog = 0b0000001, // Qt app output (qDebug(), qWarning(),..)
        UserEventLog = 0b0000010, // User activity (mouse and keys)
        CrashLog     = 0b0000100  // Backtrace in case of crash
    };

    LoggerTool();

    void initLog(int LoggerTypeConfig, const QString &appName = "", const QString &appVersion = "");

private: // methods
    QString generateCurrentLogDir();
    bool removeDir(const QString & dirName);

private: // feilds
    UserEventLogger m_userEventLogger;
    AppOutputLogger m_appOutputLogger;
};

#endif // LOGGERSET_H
