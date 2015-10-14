#include "string_utils_test.h"
#include "queue_test.h"
#include "logger_test.h"

int main(void) {
    queue_tests();
    logger_tests();
    string_utils_tests();

    return 0;
}
