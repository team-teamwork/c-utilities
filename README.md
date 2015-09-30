# c-utilities

Collection of reusable C utility functions, data structures and macros.

# Datastructures

## Queue

A thread-safe queue implementation.

# Utilities

## logger.h

Compile your program with the following flag: `-DEBUGMSG` to enable
the `log_debug` macro.

Sample program:

```c
int main(void) {
    log_error("Something horrible happened");
    log_warn("We can recover from this");
    log_info("I think you should know this");
    log_debug("This is only during development");

    log_info("All logging macros works with %s", "variables");
    errno = EAGAIN;
    log_error("Operation would block!");
}
```

Sample output:

```
logger.c:5 in function: "main" errno: "None" [ERROR] Something horrible happened
logger.c:6 in function: "main" errno: "None" [WARN] We can recover from this
logger.c:7 in function: "main" errno: "None" [INFO] I think you should know this
logger.c:10 in function: "main" errno: "None" [INFO] All logging macros works with variables
logger.c:12 in function: "main" errno: "Resource temporarily unavailable" [ERROR] Operation would block!
```