/****************************************************************************
**
** Copyright (C) 2017 Ricky Tack
** Contact: rickytack9@gmail.com
**
****************************************************************************/

#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include "crashhandler.h"

namespace CrashHandler{

static const int nameSize = 200;
static char fileName[nameSize];

void crush_handler(int sig){

    const int arraySize = 20;
    void *array[arraySize];
    int size;

//    if(sig == SIGSEGV) printf("Segmentation violation (ANSI)\n");
//    else if(sig == SIGABRT) printf("Abort (ANSI)\n");
//    else if(sig == SIGFPE) printf("Floating-point exception (ANSI)\n");
    fprintf(stderr, "Error: signal %d:\n", sig);

    // get void*'s for all entries on the stack
    size = backtrace(array, arraySize);

    // to console
    backtrace_symbols_fd(array, size, STDERR_FILENO);

    // // to file
    int pfd;
    if ((pfd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC,
                    S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1){
        perror("Cannot open output file\n");
    }
    else{
        const char *buf = "Backtrace\n";
        write(pfd, buf, strlen(buf));

        backtrace_symbols_fd(array, size, pfd);
    }

    exit(1);
}
//========================================================================
void installHandler(const QString logFileName){

    if(!logFileName.trimmed().isEmpty() && logFileName.length() < nameSize){

        QByteArray ba = logFileName.toLatin1();
        memcpy(&fileName[0], ba.data(), ba.size());

        signal(SIGSEGV, crush_handler);
        signal(SIGABRT, crush_handler);
        signal(SIGFPE, crush_handler);
    }
}

}
