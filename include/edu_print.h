#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*edu_print_func)(const void *);

void edu_print_c(const void *data);
void edu_print_uc(const void *data);
void edu_print_sc(const void *data);

void edu_print_s(const void *data);
void edu_print_us(const void *data);

void edu_print_i(const void *data);
void edu_print_ui(const void *data);

void edu_print_l(const void *data);
void edu_print_ul(const void *data);

void edu_print_ll(const void *data);
void edu_print_ull(const void *data);

void edu_print_f(const void *data);
void edu_print_d(const void *data);
void edu_print_ld(const void *data);

#ifdef __cplusplus
}
#endif
