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
#include <errno.h>

#include "crashhandler.h"

namespace CrashHandler{

static const int nameSize = 200;
static char fileName[nameSize];

void crush_handler(int sig){

    const int arraySize = 20;
    void *array[arraySize];
    int size;

    fprintf(stderr, "Oops! Error: %s.\n", strsignal(sig));

    // get void*'s for all entries on the stack
    size = backtrace(array, arraySize);

    // launch addr2line in shell
    // write to file its output
    char syscom[256];
    for (int i=0; i < size; i++){
        if ( array[i] < (void *)0x420000 && array[i] > (void *)0x400000) {
            sprintf(syscom, "addr2line %p -f -e %s | tee -a %s",
                    array[i],
                    program_invocation_short_name,
                    fileName);
            system(syscom);
        }
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
