#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*edu_cmp)(const void *, const void *);

int edu_cmp_c(const void *lhs, const void *rhs);
int edu_cmp_sc(const void *lhs, const void *rhs);
int edu_cmp_uc(const void *lhs, const void *rhs);

int edu_cmp_s(const void *lhs, const void *rhs);
int edu_cmp_us(const void *lhs, const void *rhs);

int edu_cmp_i(const void *lhs, const void *rhs);
int edu_cmp_ui(const void *lhs, const void *rhs);

int edu_cmp_l(const void *lhs, const void *rhs);
int edu_cmp_ul(const void *lhs, const void *rhs);

int edu_cmp_ll(const void *lhs, const void *rhs);
int edu_cmp_ull(const void *lhs, const void *rhs);

int edu_cmp_f(const void *lhs, const void *rhs);
int edu_cmp_d(const void *lhs, const void *rhs);
int edu_cmp_ld(const void *lhs, const void *rhs);

#ifdef __cplusplus
}
#endif
