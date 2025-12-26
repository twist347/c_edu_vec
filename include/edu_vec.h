#pragma once

#include <stddef.h>
#include <stdbool.h>

#include "internal/edu_cmp.h"
#include "internal/edu_print.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct edu_vec edu_vec;

/* ---------- create/destroy ---------- */

edu_vec *edu_vec_create(size_t size, size_t elem_size);
edu_vec *edu_vec_create_cap(size_t cap, size_t elem_size);
edu_vec *edu_vec_create_from_buf(void *buf, size_t size, size_t elem_size);
void edu_vec_destroy(edu_vec *vec);

/* ---------- copy/move semantic ---------- */

edu_vec *edu_vec_copy(const edu_vec *from);
edu_vec *edu_vec_move(edu_vec *from);
bool edu_vec_copy_assign(edu_vec *to, const edu_vec *from);
void edu_vec_move_assign(edu_vec *to, edu_vec *from);

/* ---------- info ---------- */

size_t edu_vec_size(const edu_vec *vec);
bool edu_vec_empty(const edu_vec *vec);
size_t edu_vec_cap(const edu_vec *vec);
size_t edu_vec_elem_size(const edu_vec *vec);

/* ---------- access ---------- */

void *edu_vec_get(edu_vec *vec, size_t idx);
const void *edu_vec_get_const(const edu_vec *vec, size_t idx);
void edu_vec_set(edu_vec *vec, size_t idx, const void *elem);
void *edu_vec_buf(edu_vec *vec);
const void *edu_vec_buf_const(const edu_vec *vec);

/* ---------- mods ---------- */

bool edu_vec_push(edu_vec *vec, const void *elem);
bool edu_vec_pop(edu_vec *vec, void *out);
void edu_vec_clear(edu_vec *vec);
bool edu_vec_reserve(edu_vec *vec, size_t new_cap);
bool edu_vec_resize(edu_vec *vec, size_t new_size);
bool edu_vec_shrink_to_fit(edu_vec *vec);
void edu_vec_fill(edu_vec *vec, const void *elem);
void edu_vec_swap(edu_vec *a, edu_vec *b);
bool edu_vec_insert(edu_vec *vec, size_t idx, const void *elem);
bool edu_vec_erase(edu_vec *vec, size_t idx, void *out);

/* ---------- relations ---------- */

bool edu_vec_eq(const edu_vec *a, const edu_vec *b, edu_cmp cmp);
bool edu_vec_not_eq(const edu_vec *a, const edu_vec *b, edu_cmp cmp);

/* ---------- algs ---------- */

void edu_vec_sort(edu_vec *vec, edu_cmp cmp);
ptrdiff_t edu_vec_find(const edu_vec *vec, const void *key, edu_cmp cmp);
bool edu_vec_contains(const edu_vec *vec, const void *key, edu_cmp cmp);

/* ---------- print ---------- */

void edu_vec_print(const edu_vec *vec, edu_print_func f);

/* ---------- macros ---------- */

#define EDU_VEC_CREATE(T, size) \
    edu_vec_create((size), sizeof(T))

#define EDU_VEC_CREATE_CAP(T, cap) \
    edu_vec_create_cap((cap), sizeof(T))

#define EDU_VEC_CREATE_FROM_BUF(T, buf, size) \
    edu_vec_create_from_buf((buf), (size), sizeof(T))

#define EDU_VEC_GET(vec, T, idx) \
    ((T *) edu_vec_get((vec), (idx)))

#define EDU_VEC_GET_CONST(vec, T, idx) \
    ((const T *) edu_vec_get_const((vec), (idx)))

#define EDU_VEC_SET(vec, T, idx, val) \
    do { T _tmp = (val); edu_vec_set((vec), (idx), &_tmp); } while (0)

#define EDU_VEC_PUSH(vec, T, val) \
    do { T _tmp = (val); edu_vec_push((vec), &_tmp); } while (0)

#define EDU_VEC_BUF(vec, T) \
    ((T *) edu_vec_buf((vec)))

#define EDU_VEC_BUF_CONST(vec, T) \
    ((const T *) edu_vec_buf_const((vec)))

#ifdef __cplusplus
}
#endif
