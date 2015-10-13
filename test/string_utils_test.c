#include <assert.h>
#include <stdio.h>
#include "string_utils.h"
#include "string_utils_test.h"

enum opcode {
    MESSAGE = 10,
};

void string_utils_tests(void) {
    assert(STRCMP(enum2str(MESSAGE), ==, "MESSAGE"));
    assert(!STRCMP("hello", ==, "Bye"));
    assert(STRCMP("hello", !=, "Bye"));
    assert(STRCMP("hello", ==, "hello"));
}
