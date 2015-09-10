#include <errno.h>
#include "logger.h"

int main(void) {
    log_error("Something horrible happened");
    log_warn("We can recover from this");
    log_info("I think you should know this");
    log_debug("This is only during development");

    log_info("All logging macros works with %s", "variables");
    errno = EAGAIN;
    log_error("Operation would block!");

    return 0;
}
