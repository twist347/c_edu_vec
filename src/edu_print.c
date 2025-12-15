#include "../include/internal/edu_print.h"

#include <stdio.h>

#define EDU_PRINT_DEF(NAME, TYPE, FMT)                 \
    void edu_print_##NAME(const void *data) {          \
        printf(FMT, *(const TYPE *) data);             \
    }

EDU_PRINT_DEF(c,   char,               "%c")
EDU_PRINT_DEF(uc,  unsigned char,      "%hhu")
EDU_PRINT_DEF(sc,  signed char,        "%hhd")

EDU_PRINT_DEF(s,   short,              "%hd")
EDU_PRINT_DEF(us,  unsigned short,     "%hu")

EDU_PRINT_DEF(i,   int,                "%d")
EDU_PRINT_DEF(ui,  unsigned int,       "%u")

EDU_PRINT_DEF(l,   long,               "%ld")
EDU_PRINT_DEF(ul,  unsigned long,      "%lu")

EDU_PRINT_DEF(ll,  long long,          "%lld")
EDU_PRINT_DEF(ull, unsigned long long, "%llu")

EDU_PRINT_DEF(f,   float,              "%f")
EDU_PRINT_DEF(d,   double,             "%lf")
EDU_PRINT_DEF(ld,  long double,        "%Lf")
