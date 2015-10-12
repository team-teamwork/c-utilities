#include "logger.h"
#include "enum2str_test.h"

enum opcode {
    MESSAGE = 10,
};

void enum2str_tests(void) {
    log_info("%s", enum2str(MESSAGE));
}
