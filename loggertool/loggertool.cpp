/****************************************************************************
**
** Copyright (C) 2017 Ricky Tack
** Contact: rickytack9@gmail.com
**
****************************************************************************/

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>

#include "crashhandler.h"
#include "loggertool.h"

LoggerTool::LoggerTool()
{

}
//========================================================================
void LoggerTool::initLog(int LoggerTypeConfig, const QString &appName, const QString &appVersion){

    QString currentLogDir = LoggerTool::generateCurrentLogDir();
    if(currentLogDir.trimmed().isEmpty()){
        qWarning() << "LoggerTool::initLog: can't start logging";
        return;
    }
    //
    if(LoggerTypeConfig & AppOutputLog)
        m_appOutputLogger.startLog(currentLogDir + "/appoutputlog.txt", appName, appVersion);

    if(LoggerTypeConfig & UserEventLog)
        m_userEventLogger.startLog(currentLogDir + "/eventslog.csv");

    if(LoggerTypeConfig & CrashLog)
        CrashHandler::installHandler(currentLogDir + "/crashlog.txt");
}
//========================================================================
QString LoggerTool::generateCurrentLogDir(){

    // dir for all types of debug logs
    QString superLogDir = QCoreApplication::applicationDirPath() + "/log_info";
    if(!QDir(superLogDir).exists() && !QDir().mkdir(superLogDir)){
        qWarning() << "Cant create dir" << superLogDir;
        return QString();
    }
    // internal log dir
    // log dir name generation
    QString dateStr = "date_" + QDate::currentDate().toString("dd_MM_yyyy");
    QString timeStr = "_time_" + QTime::currentTime().toString("hh_mm_ss");
    QString currentLogDir = superLogDir + "/log_" + dateStr + timeStr;

    if(!QDir(currentLogDir).exists() && !QDir().mkdir(currentLogDir)){
        qWarning() << "Cant create dir" << currentLogDir;
        return QString();
    }

    // remove old log folders (files from other days)
    QStringList dirNamesList = QDir(superLogDir).entryList(QDir::Dirs | QDir::Writable |
                                           QDir::NoDotAndDotDot  | QDir::NoSymLinks);
    foreach (const QString &direName, dirNamesList){
        if (!direName.contains(dateStr)){//
            removeDir(superLogDir + "/" + direName);
        }
    }

    return currentLogDir;
}
//========================================================================
bool LoggerTool::removeDir(const QString & dirName){
    bool result = true;
    QDir dir(dirName);

    if (dir.exists()) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System |
                         QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = QDir().rmdir(dirName);
    }
    return result;
}
