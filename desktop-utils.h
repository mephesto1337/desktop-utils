#ifndef DESKTOP_UTILS_H
#define DESKTOP_UTILS_H

#include <stdbool.h>

bool daemonize(void);
bool daemonize_uniq(const char *filename);
bool spawn(char *const cmd[], char *const envp[]);
bool lockfile(const char *filename, int *handle);
bool unlockfile(const char *filename, int *handle);

#ifdef WITH_LOCK
#ifndef LOCK_FILENAME
#error "Please define also LOCK_FILENAME with WITH_LOCK"
#endif
void __init(void) __attribute__((constructor));
void __exit(void) __attribute__((destructor));
#endif



#endif // DESKTOP_UTILS_H
