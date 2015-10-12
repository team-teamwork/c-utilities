#include "logger.h"
#include "logger_test.h"

void logger_tests(void) {
    log_error("Something horrible happened");
    log_warn("We can recover from this");
    log_info("I think you should know this");
    log_debug("This is only during development");
    log_info("All logging macros works with %s", "variables");
    errno = EAGAIN;
    log_error("Operation would block!");
    log_info("Notice that errno is reset to 0");
}
