/****************************************************************************
**
** Copyright (C) 2017 Ricky Tack
** Contact: rickytack9@gmail.com
**
****************************************************************************/

#ifndef USEREVENTLOGGER_H
#define USEREVENTLOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>

class UserEventLogger
{
public:
    explicit UserEventLogger();
    ~UserEventLogger();

    void startLog(const QString &logFileName);
    void stopLog();

private: // methods
    static bool eventCallback(void** data);

private: // feilds
    bool m_isStarted = false;
    static QFile logFile;
    static QTextStream outTextStream;

    static QString separator;
};

#endif // USEREVENTLOGGER_H
