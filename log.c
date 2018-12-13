#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include "log.h"

void __log_message(const char *file, int line, const char *format, ...) {
    va_list args;
    char datetime[32];
    char new_format[256];
    struct tm tm;
    time_t now = time(NULL);

    va_start(args, format);
    localtime_r(&now, &tm);
    strftime(datetime, sizeof(datetime), "%F %T", &tm);
    snprintf(new_format, sizeof(new_format) - 1, "[%s] [%s:%d] %s\n", datetime, file, line, format);
    vfprintf(stderr, new_format, args);
    va_end(args);
}

