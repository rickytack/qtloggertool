#ifndef LOGGERSET_H
#define LOGGERSET_H

#include <QString>

#include "usereventlogger.h"
#include "appoutputlogger.h"

class QApplication;

class LoggerTool
{
public:
    enum LoggerType{
        AppOutputLog = (1 << 0), // Qt app output (qDebug(), qWarning(),..)
        UserEventLog = (1 << 1), // User activity (mouse and keys)
        CrashLog     = (1 << 2)  // Backtrace in case of crash
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
