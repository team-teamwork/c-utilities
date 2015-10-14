#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifndef __LOGGER_H__
#define __LOGGER_H__

#define strerrno() errno == 0 ? "None" : strerror(errno)

#define file_info __FILE__, __LINE__

#define log(LABEL, MESSAGE, ...)                                        \
    do {                                                                \
        fprintf(stderr, "%s:%d in function: \"%s\" errno: \"%s\" "                \
                "[" LABEL "] " MESSAGE "\n",                           \
                file_info, __func__, strerrno(), ##__VA_ARGS__);        \
    } while (0)

#define log_error(MESSAGE, ...) do { \
    log("ERROR", MESSAGE, ##__VA_ARGS__);       \
    errno = 0;                                  \
 } while(0)

#define log_info(MESSAGE, ...) log("INFO", MESSAGE, ##__VA_ARGS__)
#define log_warn(MESSAGE, ...) log("WARN", MESSAGE, ##__VA_ARGS__)


/**
 * A macro for logging debug messages.
 * Compile with -DEBUGMSG to enable.
 */
#ifdef EBUGMSG
#define log_debug(MESSAGE, ...) log("DEBUG", MESSAGE, ##__VA_ARGS__)
#else
#define log_debug(message)			\
  do {                                          \
    ;                                           \
  } while (0)
#endif

#endif
