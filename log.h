#ifndef DESKTOP_UTILS_LOG_H
#define DESKTOP_UTILS_LOG_H

#include <stdarg.h>

#ifdef DEBUG
#   define debug(fmt, ...)      log_message("[DEBUG] " fmt, ## __VA_ARGS__)
#else
#   define debug(fmt, ...)
#endif
#define error(fmt, ...)         log_message("[ERROR] " fmt, ## __VA_ARGS__)   

#define log_message(fmt, ...)   __log_message(__FILE__, __LINE__, fmt, ## __VA_ARGS__)


void __log_message(const char *file, int line, const char *format, ...) __attribute__((format(printf, 3, 4)));


#endif // DESKTOP_UTILS_LOG_H
