#include <string.h>

#ifndef __STRING_UTILS_H__
#define __STRING_UTILS_H__

/**
 * Wrapper defines around strcmp & strncmp, their man page describes them
 * like so,
 *
 * The  strcmp()  function compares the two strings s1 and s2.  It returns
 * an integer less than, equal to, or greater than zero if  s1  is  found,
 * respectively, to be less than, to match, or be greater than s2.
 *
 * The  strncmp()  function  is similar, except it compares the only first
 * (at most) n bytes of s1 and s2.
 *
 * The macros expect the same arguments as strcmp and strncmp, their function
 * declarations are described like so,
 *
 * int strcmp(const char *s1, const char *s2);
 * int strncmp(const char *s1, const char *s2, size_t n);
 *
 * To demonstrate how to use the macros an example for STRCMP will be provided.
 *
 * Example:
 * STRCMP("hello", ==, "Bye"); -> 0
 * STRCMP("hello", !=, "Bye"); -> 1
 * STRCMP("hello", ==, "hello"); -> 1
 */

#define STRCMP(a, R, b) strcmp(a, b) R 0
#define STRNCMP(a, R, b, n) strncmp(a, b, n) R 0

#endif
