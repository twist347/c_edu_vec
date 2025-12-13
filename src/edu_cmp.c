#include "edu_cmp.h"

#include <math.h>

#define EDU_CMP_INT_DEF(NAME, TYPE)                                 \
    int edu_cmp_##NAME(const void *lhs, const void *rhs) {          \
        const TYPE a = *(const TYPE *) lhs;                         \
        const TYPE b = *(const TYPE *) rhs;                         \
        return (a < b) ? -1 : (a > b) ? 1 : 0;                      \
    }

#define EDU_CMP_FP_DEF(NAME, TYPE)                                   \
    int edu_cmp_##NAME(const void *lhs, const void *rhs) {           \
        const TYPE a = *(const TYPE *) lhs;                          \
        const TYPE b = *(const TYPE *) rhs;                          \
        const int a_nan = isnan((long double) a);                    \
        const int b_nan = isnan((long double) b);                    \
        if (a_nan | b_nan) {                                         \
            if (a_nan && b_nan) {                                    \
                return 0;                                            \
            }                                                        \
            return a_nan ? 1 : -1;                                   \
        }                                                            \
        return (a < b) ? -1 : (a > b) ? 1 : 0;                       \
    }

EDU_CMP_INT_DEF(c,   char)
EDU_CMP_INT_DEF(sc,  signed char)
EDU_CMP_INT_DEF(uc,  unsigned char)

EDU_CMP_INT_DEF(s,   short)
EDU_CMP_INT_DEF(us,  unsigned short)

EDU_CMP_INT_DEF(i,   int)
EDU_CMP_INT_DEF(ui,  unsigned int)

EDU_CMP_INT_DEF(l,   long)
EDU_CMP_INT_DEF(ul,  unsigned long)

EDU_CMP_INT_DEF(ll,  long long)
EDU_CMP_INT_DEF(ull, unsigned long long)

EDU_CMP_FP_DEF(f,    float)
EDU_CMP_FP_DEF(d,    double)
EDU_CMP_FP_DEF(ld,   long double)
