#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <cxxabi.h>
#include <signal.h>

#include "crashhandler.h"

namespace CrashHandler{

/** Print a demangled stack backtrace of the caller function to FILE* out. */
static inline void print_stacktrace(FILE *out)
{
    fprintf(out, "stack trace:\n");
    fprintf(stderr, "stack trace:\n");

    // storage array for stack trace address data
    const int listlength = 64;
    void* addrlist[listlength];

    // retrieve current stack addresses
    int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));

    if (addrlen == 0) {
    fprintf(out, "  <empty, possibly corrupt>\n");
    return;
    }

    // resolve addresses into strings containing "filename(function+address)",
    // this array must be free()-ed
    char** symbollist = backtrace_symbols(addrlist, addrlen);

    // allocate string which will be filled with the demangled function name
    size_t funcnamesize = 256;
    char* funcname = (char*)malloc(funcnamesize);

    // iterate over the returned symbol lines. skip the first, it is the
    // address of this function.
    for (int i = 1; i < addrlen; i++)
    {
    char *begin_name = 0, *begin_offset = 0, *end_offset = 0;

    // find parentheses and +address offset surrounding the mangled name:
    // ./module(function+0x15c) [0x8048a6d]
    for (char *p = symbollist[i]; *p; ++p)
    {
        if (*p == '(')
        begin_name = p;
        else if (*p == '+')
        begin_offset = p;
        else if (*p == ')' && begin_offset) {
        end_offset = p;
        break;
        }
    }

    if (begin_name && begin_offset && end_offset
        && begin_name < begin_offset)
    {
        *begin_name++ = '\0';
        *begin_offset++ = '\0';
        *end_offset = '\0';

        // mangled name is now in [begin_name, begin_offset) and caller
        // offset in [begin_offset, end_offset). now apply
        // __cxa_demangle():

        int status;
        char* ret = abi::__cxa_demangle(begin_name,
                        funcname, &funcnamesize, &status);
        if (status == 0) {
        funcname = ret; // use possibly realloc()-ed string
        fprintf(out, "  %s : %s+%s\n", symbollist[i], funcname, begin_offset);
        fprintf(stderr, "  %s : %s+%s\n", symbollist[i], funcname, begin_offset);
        }
        else {
        // demangling failed. Output function name as a C function with
        // no arguments.
        fprintf(out, "  %s : %s()+%s\n", symbollist[i], begin_name, begin_offset);
        fprintf(stderr, "  %s : %s()+%s\n", symbollist[i], begin_name, begin_offset);
        }
    }
    else
    {
        // couldn't parse the line? print the whole line.
        fprintf(out, "  %s\n", symbollist[i]);
        fprintf(stderr, "  %s\n", symbollist[i]);
    }
    }

    free(funcname);
    free(symbollist);
}

static const int nameSize = 200;
static char fileName[nameSize];

void crush_handler(int sig){

    fprintf(stderr, "Oops! Error: %s.\n", strsignal(sig));

    // // to file
    FILE* destFile = fopen(fileName, "w");

    if (destFile == NULL){
        fprintf(stderr, "Cannot open output file\n");

    }else{
        fprintf(stderr, "Backtrace file is open\n");
        print_stacktrace(destFile);
    }

    fclose(destFile);
    _Exit(0);
}
//========================================================================
void installHandler(const QString &logFileName){

    if(!logFileName.trimmed().isEmpty() && logFileName.length() < nameSize){

        QByteArray ba = logFileName.toLatin1();
        memcpy(&fileName[0], ba.data(), ba.size());

        signal(SIGSEGV, crush_handler);
        signal(SIGABRT, crush_handler);
        signal(SIGFPE, crush_handler);
        signal(SIGPIPE, crush_handler);
    }
}

}
