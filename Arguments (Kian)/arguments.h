#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <float.h>

long double validate_argument_long_double(char const* str);
long long validate_argument_long_long(char const* str);
unsigned long long validate_argument_unsigned_long_long(char const* str);

#endif
