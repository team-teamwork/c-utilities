#include "enum2str_test.h"
#include "queue_test.h"
#include "logger_test.h"

int main(void) {
    queue_tests();
    logger_tests();
    enum2str_tests();

    return 0;
}
