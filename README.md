# c-utilities

Collection of reusable C utility functions, data structures and macros.

# Datastructures

## Queue

A thread-safe queue implementation.

# Utilities

## logger.h

Compile your program with the following flag: `-DEBUGMSG` to enable
the `log_debug` macro.

Sample program (named `logger_test.c`, located in the `test` folder):

```c
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
}
```

If the program is compiled without the `-DEBUGMSG` flag the resulting output of calling the function is:
(can be compiled using `make tests`)
```
test/logger_test.c:5 in function: "logger_tests" errno: "None" [ERROR] Something horrible happened
test/logger_test.c:6 in function: "logger_tests" errno: "None" [WARN] We can recover from this
test/logger_test.c:7 in function: "logger_tests" errno: "None" [INFO] I think you should know this
test/logger_test.c:9 in function: "logger_tests" errno: "None" [INFO] All logging macros works with variables
test/logger_test.c:11 in function: "logger_tests" errno: "Resource temporarily unavailable" [ERROR] Operation would block!
test/logger_test.c:12 in function: "logger_tests" errno: "None" [INFO] Notice that errno is reset to 0
```

Consequently, if the function is compiled with the `-DEBUGMSG` flag calling the function yields the following output:
(`make CFLAGS+=-DEBUGMSG tests`)
```
test/logger_test.c:5 in function: "logger_tests" errno: "None" [ERROR] Something horrible happened
test/logger_test.c:6 in function: "logger_tests" errno: "None" [WARN] We can recover from this
test/logger_test.c:7 in function: "logger_tests" errno: "None" [INFO] I think you should know this
test/logger_test.c:8 in function: "logger_tests" errno: "None" [DEBUG] This is only during development
test/logger_test.c:9 in function: "logger_tests" errno: "None" [INFO] All logging macros works with variables
test/logger_test.c:11 in function: "logger_tests" errno: "Resource temporarily unavailable" [ERROR] Operation would block!
test/logger_test.c:12 in function: "logger_tests" errno: "None" [INFO] Notice that errno is reset to 0
```

Lastly the header file also supplies a nice utility method for stringifying enums, 
Sample program (`test/enum2str_test.c`):

```c
#include "logger.h"
#include "enum2str_test.h"

enum opcode {
    MESSAGE = 10,
};

void enum2str_tests(void) {
    log_info("%s", enum2str(MESSAGE));
}
```
When executed the function outputs the following,
```
MESSAGE
```
