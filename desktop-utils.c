#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "desktop-utils.h"
#include "log.h"
#include "macros.h"


static int daemon_handle = -1;
static char lock_filename[256];
static int in_son = false;
static int has_lock = false;

void sighandler(int signum);

void __exit_daemon_uniq(void) __attribute__((destructor));

inline bool daemonize(void) {
    return daemonize_uniq(NULL);
}

bool daemonize_uniq(const char *filename) {
#ifdef DEBUG
    UNUSED(filename);
    return true;
#else
    pid_t son;

    if ( filename ) {
        strncpy(lock_filename, filename, sizeof(lock_filename));
        CHK_NEG(signal(SIGTERM, sighandler));
        CHK_FALSE(has_lock = lockfile(lock_filename, &daemon_handle));
    }

    CHK_NEG(son = fork());
    if ( son == 0 ) {
        CHK_NEG(close(STDIN_FILENO));
        CHK_NEG(close(STDOUT_FILENO));
        CHK_NEG(close(STDERR_FILENO));
        in_son = true;
        return true;
    } else {
        exit(EXIT_SUCCESS);
    }

    fail:
    return false;
#endif
}

void __exit_daemon_uniq(void) {
    if ( in_son && has_lock ) {
        unlockfile(lock_filename, &daemon_handle);
        unlink(lock_filename);
    }
}

bool spawn(char *const cmd[], char *const envp[]) {
    pid_t son = -1;

    CHK_NEG(son = fork());
    if ( son == 0 ) {
        debug("Executing %s", cmd[0]);
        if ( execve(cmd[0], cmd, envp) < 0 ) {
            exit(EXIT_FAILURE);
        } else {
            exit(EXIT_SUCCESS);
        }
    } else {
        CHK_NEG(waitpid(son, NULL, 0));
    }

    return true;

    fail:
    return false;
}

bool lockfile(const char *filename, int *handle) {
    int fd;

    CHK_NEG(fd = open(filename, O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR));
    *handle = fd;
    return true;

    fail:
    return false;
}

bool unlockfile(const char *filename, int *handle) {
    close(*handle);
    unlink(filename);
    *handle = -1;

    return true;
}

void sighandler(int signum) {
    debug("Caught signal %d", signum);
    UNUSED(signum);

    if ( in_son && has_lock ) {
        unlockfile(lock_filename, &daemon_handle);
        unlink(lock_filename);
    }
    exit(EXIT_SUCCESS);
}

#ifdef WITH_LOCK
static int handle;

void __init(void) {
    CHK_FALSE(lockfile(LOCK_FILENAME, &handle));
    return;

    fail:
    exit(EXIT_FAILURE);
}

void __exit(void) {
    CHK_NEG(unlockfile(LOCK_FILENAME, &handle));
    fail:
    return;
}
#endif
