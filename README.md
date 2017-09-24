# qtloggertool
A tool for logging Qt application output, user events(mouse, key) and crash reports.

To use:
1) Add the next 2 lines to your application .pro file:

INCLUDEPATH += ../loggertool

include(../loggertool/loggertool.pri)

2) Add the code below in main function of your project:

LoggerTool loggerSet;

loggerSet.initLog(LoggerTool::AppOutputLog | LoggerTool::UserEventLog | LoggerTool::CrashLog);
                      
When you start the application, a folder with log files will be created next to the executable file.

