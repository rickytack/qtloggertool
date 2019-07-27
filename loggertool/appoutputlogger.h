#ifndef APPOUTPUTLOGGER_H
#define APPOUTPUTLOGGER_H

#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QMutex>
//
class AppOutputLogger
{

public:
    AppOutputLogger();
    ~AppOutputLogger();

    void startLog(const QString &logFileName, const QString& appName,
                  const QString& appVersion);
    void stopLog();

private:

    // callbacks
#if QT_VERSION >= 0x050000
    static void messageOutput(QtMsgType type,
                    const QMessageLogContext &context, const QString &msg);
#else
    static void messageOutput(QtMsgType type, const char* msg);
#endif

private: // feilds
    bool m_isStarted = false;

    static QStringList message_type;

    static QFile logFile;
    static QTextStream outTextStream;

    static QMutex fileMutex;

    static bool showPlace;
};

#endif /* APPOUTPUT LOGGER_H */
