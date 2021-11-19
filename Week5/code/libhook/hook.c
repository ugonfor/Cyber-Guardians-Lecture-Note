#define _GNU_SOURCE
#include <stdarg.h>
#include <stdio.h>
#include <dlfcn.h>

extern char * __progname;

static int (*hook_printf)(const char *__restrict __fmt, ...) = NULL;

int printf(const char *__restrict __fmt, ...){
    va_list arglist;

    hook_printf("[Hook!] ");
    va_start(arglist, __fmt);
    int ret = vprintf(__fmt, arglist);
    va_end(arglist);
    
    return ret;
}


void __attribute__ ((constructor)) before_load(void)
{
    if (hook_printf == NULL) hook_printf    = dlsym(RTLD_NEXT, "printf");
}