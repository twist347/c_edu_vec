#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "edu_vec.h"

#include <stdlib.h>
#include <stdio.h>

static edu_vec *make_int_vec(const int *a, size_t n) {
    edu_vec *v = edu_vec_create(0, sizeof(int));
    cr_assert_not_null(v);

    for (size_t i = 0; i < n; ++i) {
        cr_assert(edu_vec_push(v, &a[i]));
    }
    return v;
}

/* ---------- create/destroy ---------- */

Test(vec_api, edu_vec_create) {
    edu_vec *v = edu_vec_create(3, sizeof(int));
    cr_assert_not_null(v);

    cr_assert_eq(edu_vec_size(v), 3);
    cr_assert_eq(edu_vec_cap(v), 3);
    cr_assert_eq(edu_vec_elem_size(v), sizeof(int));

    for (size_t i = 0; i < 3; ++i) {
        cr_assert_eq(*(const int *)edu_vec_get_const(v, i), 0);
    }

    edu_vec_destroy(v);
}

Test(vec_api, edu_vec_create_cap) {
    edu_vec *v = edu_vec_create_cap(5, sizeof(int));
    cr_assert_not_null(v);

    cr_assert_eq(edu_vec_size(v), 0);
    cr_assert_eq(edu_vec_cap(v), 5);
    cr_assert_eq(edu_vec_elem_size(v), sizeof(int));

    edu_vec_destroy(v);
}

Test(vec_api, edu_vec_create_from_buf) {
    int *buf = malloc(3 * sizeof(int));
    cr_assert_not_null(buf);

    buf[0] = 10;
    buf[1] = 20;
    buf[2] = 30;

    edu_vec *v = edu_vec_create_from_buf(buf, 3, sizeof(int));
    cr_assert_not_null(v);

    cr_assert_eq(edu_vec_size(v), 3);
    cr_assert_eq(edu_vec_cap(v), 3);
    cr_assert_eq(edu_vec_buf(v), (void *)buf);

    cr_assert_eq(*(int *)edu_vec_get(v, 1), 20);

    int x = 99;
    edu_vec_set(v, 1, &x);
    cr_assert_eq(buf[1], 99);

    edu_vec_destroy(v);
}

Test(vec_api, edu_vec_destroy) {
    edu_vec *v = edu_vec_create(0, sizeof(int));
    cr_assert_not_null(v);

    edu_vec_destroy(v);
    edu_vec_destroy(NULL);
}

/* ---------- copy/move semantic ---------- */

Test(vec_api, edu_vec_copy) {
    const int a[] = {1, 2, 3};
    edu_vec *src = make_int_vec(a, 3);

    edu_vec *cpy = edu_vec_copy(src);
    cr_assert_not_null(cpy);

    cr_assert_eq(edu_vec_size(cpy), edu_vec_size(src));
    cr_assert_eq(edu_vec_elem_size(cpy), edu_vec_elem_size(src));

    cr_assert_neq(edu_vec_buf(cpy), edu_vec_buf(src));

    for (size_t i = 0; i < 3; ++i) {
        cr_assert_eq(*(int *)edu_vec_get(cpy, i), a[i]);
    }

    edu_vec_destroy(src);
    edu_vec_destroy(cpy);
}

Test(vec_api, edu_vec_move) {
    int a[] = {7, 8};
    edu_vec *src = make_int_vec(a, 2);
    void *src_buf = edu_vec_buf(src);

    edu_vec *m = edu_vec_move(src);
    cr_assert_not_null(m);

    cr_assert_eq(edu_vec_buf(m), src_buf);
    cr_assert_eq(edu_vec_size(m), 2);
    cr_assert_eq(*(int *)edu_vec_get(m, 0), 7);
    cr_assert_eq(*(int *)edu_vec_get(m, 1), 8);

    cr_assert_eq(edu_vec_size(src), 0);
    cr_assert_eq(edu_vec_cap(src), 0);
    cr_assert_null(edu_vec_buf(src));

    edu_vec_destroy(src);
    edu_vec_destroy(m);
}

Test(vec_api, edu_vec_copy_assign) {
    const int a[] = {1, 2, 3};
    edu_vec *src = make_int_vec(a, 3);

    const int b[] = {9};
    edu_vec *dst = make_int_vec(b, 1);

    cr_assert(edu_vec_copy_assign(dst, src));

    cr_assert_eq(edu_vec_size(dst), 3);
    cr_assert_neq(edu_vec_buf(dst), edu_vec_buf(src)); /* deep */

    const int x = 111;
    edu_vec_set(src, 0, &x);

    cr_assert_eq(*(int *)edu_vec_get(dst, 0), 1);
    cr_assert_eq(*(int *)edu_vec_get(src, 0), 111);

    edu_vec_destroy(src);
    edu_vec_destroy(dst);
}

Test(vec_api, edu_vec_move_assign) {
    int a[] = {4, 5};
    edu_vec *src = make_int_vec(a, 2);
    void *src_buf = edu_vec_buf(src);

    int b[] = {99, 98, 97};
    edu_vec *dst = make_int_vec(b, 3);

    edu_vec_move_assign(dst, src);

    cr_assert_eq(edu_vec_buf(dst), src_buf);
    cr_assert_eq(edu_vec_size(dst), 2);
    cr_assert_eq(*(int *)edu_vec_get(dst, 0), 4);
    cr_assert_eq(*(int *)edu_vec_get(dst, 1), 5);

    cr_assert_eq(edu_vec_size(src), 0);
    cr_assert_eq(edu_vec_cap(src), 0);
    cr_assert_null(edu_vec_buf(src));

    edu_vec_destroy(src);
    edu_vec_destroy(dst);
}

/* ---------- info ---------- */

Test(vec_api, edu_vec_size) {
    edu_vec *v = edu_vec_create(0, sizeof(int));
    const int x = 1, y = 2;
    edu_vec_push(v, &x);
    edu_vec_push(v, &y);

    cr_assert_eq(edu_vec_size(v), 2);
    edu_vec_destroy(v);
}

Test(vec_api, edu_vec_empty) {
    edu_vec *v = edu_vec_create(0, sizeof(int));
    cr_assert(edu_vec_empty(v));

    const int x = 1;
    edu_vec_push(v, &x);
    cr_assert_not(edu_vec_empty(v));

    edu_vec_destroy(v);
}

Test(vec_api, edu_vec_cap) {
    edu_vec *v = edu_vec_create_cap(10, sizeof(int));
    cr_assert_eq(edu_vec_cap(v), 10);
    edu_vec_destroy(v);
}

Test(vec_api, edu_vec_elem_size) {
    edu_vec *v = edu_vec_create(0, sizeof(long long));
    cr_assert_eq(edu_vec_elem_size(v), sizeof(long long));
    edu_vec_destroy(v);
}

/* ---------- access ---------- */

Test(vec_api, edu_vec_get) {
    edu_vec *v = edu_vec_create(0, sizeof(int));
    const int x = 42;
    edu_vec_push(v, &x);

    cr_assert_eq(*(int *)edu_vec_get(v, 0), 42);
    edu_vec_destroy(v);
}

Test(vec_api, edu_vec_get_const) {
    edu_vec *v = edu_vec_create(0, sizeof(int));
    const int x = 5;
    edu_vec_push(v, &x);

    const int *p = (const int *) edu_vec_get_const(v, 0);
    cr_assert_eq(*p, 5);

    edu_vec_destroy(v);
}

Test(vec_api, edu_vec_set) {
    edu_vec *v = edu_vec_create(1, sizeof(int));
    const int x = 77;
    edu_vec_set(v, 0, &x);

    cr_assert_eq(*(int *)edu_vec_get(v, 0), 77);
    edu_vec_destroy(v);
}

Test(vec_api, edu_vec_buf) {
    edu_vec *v = edu_vec_create(0, sizeof(int));
    const int x = 1;
    edu_vec_push(v, &x);

    const void *b = edu_vec_buf(v);
    cr_assert_not_null(b);
    cr_assert_eq(edu_vec_get(v, 0), b);

    edu_vec_destroy(v);
}

Test(vec_api, edu_vec_buf_const) {
    edu_vec *v = edu_vec_create(0, sizeof(int));
    const int x = 2;
    edu_vec_push(v, &x);

    const void *b = edu_vec_buf_const(v);
    cr_assert_not_null(b);
    cr_assert_eq(edu_vec_get_const(v, 0), b);

    edu_vec_destroy(v);
}

/* ---------- mods ---------- */

Test(vec_api, edu_vec_push) {
    edu_vec *v = edu_vec_create(0, sizeof(int));
    const int x = 123;

    cr_assert(edu_vec_push(v, &x));
    cr_assert_eq(edu_vec_size(v), 1);
    cr_assert_eq(*(int *)edu_vec_get(v, 0), 123);

    edu_vec_destroy(v);
}

Test(vec_api, edu_vec_pop) {
    edu_vec *v = edu_vec_create(0, sizeof(int));
    const int x = 9;
    edu_vec_push(v, &x);

    int out = 0;
    cr_assert(edu_vec_pop(v, &out));
    cr_assert_eq(out, 9);
    cr_assert_eq(edu_vec_size(v), 0);

    edu_vec_destroy(v);
}

Test(vec_api, edu_vec_clear) {
    edu_vec *v = edu_vec_create_cap(8, sizeof(int));
    const int x = 1, y = 2;
    edu_vec_push(v, &x);
    edu_vec_push(v, &y);

    size_t cap_before = edu_vec_cap(v);
    edu_vec_clear(v);

    cr_assert_eq(edu_vec_size(v), 0);
    cr_assert_eq(edu_vec_cap(v), cap_before);

    edu_vec_destroy(v);
}

Test(vec_api, edu_vec_reserve) {
    edu_vec *v = edu_vec_create(0, sizeof(int));

    cr_assert(edu_vec_reserve(v, 16));
    cr_assert_geq(edu_vec_cap(v), 16);
    cr_assert_eq(edu_vec_size(v), 0);

    edu_vec_destroy(v);
}

Test(vec_api, edu_vec_resize) {
    edu_vec *v = edu_vec_create(0, sizeof(int));

    cr_assert(edu_vec_resize(v, 3));
    cr_assert_eq(edu_vec_size(v), 3);

    cr_assert_eq(*(int *)edu_vec_get(v, 0), 0);
    cr_assert_eq(*(int *)edu_vec_get(v, 1), 0);
    cr_assert_eq(*(int *)edu_vec_get(v, 2), 0);

    edu_vec_destroy(v);
}

Test(vec_api, edu_vec_shrink_to_fit) {
    edu_vec *v = edu_vec_create(0, sizeof(int));
    cr_assert(edu_vec_reserve(v, 20));

    const int x = 1, y = 2;
    edu_vec_push(v, &x);
    edu_vec_push(v, &y);

    cr_assert(edu_vec_shrink_to_fit(v));
    cr_assert_eq(edu_vec_cap(v), edu_vec_size(v));
    cr_assert_eq(edu_vec_cap(v), 2);

    edu_vec_destroy(v);
}

Test(vec_api, edu_vec_fill) {
    edu_vec *v = edu_vec_create(4, sizeof(int));
    const int x = 7;

    edu_vec_fill(v, &x);

    for (size_t i = 0; i < 4; ++i) {
        cr_assert_eq(*(int *)edu_vec_get(v, i), 7);
    }

    edu_vec_destroy(v);
}

Test(vec_api, edu_vec_swap) {
    const int a[] = {1, 2};
    const int b[] = {9};

    edu_vec *va = make_int_vec(a, 2);
    edu_vec *vb = make_int_vec(b, 1);

    edu_vec_swap(va, vb);

    cr_assert_eq(edu_vec_size(va), 1);
    cr_assert_eq(*(int *)edu_vec_get(va, 0), 9);

    cr_assert_eq(edu_vec_size(vb), 2);
    cr_assert_eq(*(int *)edu_vec_get(vb, 0), 1);
    cr_assert_eq(*(int *)edu_vec_get(vb, 1), 2);

    edu_vec_destroy(va);
    edu_vec_destroy(vb);
}

Test(vec_api, edu_vec_insert) {
    const int a[] = {1, 3};
    edu_vec *v = make_int_vec(a, 2);

    const int x = 2;
    cr_assert(edu_vec_insert(v, 1, &x)); /* [1,2,3] */

    cr_assert_eq(edu_vec_size(v), 3);
    cr_assert_eq(*(int *)edu_vec_get(v, 0), 1);
    cr_assert_eq(*(int *)edu_vec_get(v, 1), 2);
    cr_assert_eq(*(int *)edu_vec_get(v, 2), 3);

    edu_vec_destroy(v);
}

Test(vec_api, edu_vec_erase) {
    const int a[] = {1, 2, 3};
    edu_vec *v = make_int_vec(a, 3);

    int out = 0;
    cr_assert(edu_vec_erase(v, 1, &out));

    cr_assert_eq(out, 2);
    cr_assert_eq(edu_vec_size(v), 2);
    cr_assert_eq(*(int *)edu_vec_get(v, 0), 1);
    cr_assert_eq(*(int *)edu_vec_get(v, 1), 3);

    edu_vec_destroy(v);
}

/* ---------- relations ---------- */

Test(vec_api, edu_vec_eq) {
    const int a[] = {1, 2, 3};
    edu_vec *x = make_int_vec(a, 3);
    edu_vec *y = make_int_vec(a, 3);

    cr_assert(edu_vec_eq(x, y, edu_cmp_i));

    edu_vec_destroy(x);
    edu_vec_destroy(y);
}

Test(vec_api, edu_vec_not_eq) {
    const int a[] = {1, 2, 3};
    const int b[] = {1, 2, 4};
    edu_vec *x = make_int_vec(a, 3);
    edu_vec *y = make_int_vec(b, 3);

    cr_assert(edu_vec_not_eq(x, y, edu_cmp_i));

    edu_vec_destroy(x);
    edu_vec_destroy(y);
}

/* ---------- algs ---------- */

Test(vec_api, edu_vec_sort) {
    const int a[] = {3, 1, 2};
    edu_vec *v = make_int_vec(a, 3);

    edu_vec_sort(v, edu_cmp_i);

    cr_assert_eq(*(int *)edu_vec_get(v, 0), 1);
    cr_assert_eq(*(int *)edu_vec_get(v, 1), 2);
    cr_assert_eq(*(int *)edu_vec_get(v, 2), 3);

    edu_vec_destroy(v);
}

Test(vec_api, edu_vec_find) {
    const int a[] = {1, 2, 3};
    edu_vec *v = make_int_vec(a, 3);

    const int key2 = 2;
    const int key9 = 9;

    cr_assert_eq(edu_vec_find(v, &key2, edu_cmp_i), (ptrdiff_t)1);
    cr_assert_eq(edu_vec_find(v, &key9, edu_cmp_i), (ptrdiff_t)-1);

    edu_vec_destroy(v);
}

Test(vec_api, edu_vec_contains) {
    const int a[] = {1, 2, 3};
    edu_vec *v = make_int_vec(a, 3);

    const int key2 = 2;
    const int key9 = 9;

    cr_assert(edu_vec_contains(v, &key2, edu_cmp_i));
    cr_assert_not(edu_vec_contains(v, &key9, edu_cmp_i));

    edu_vec_destroy(v);
}

/* ---------- print ---------- */

Test(vec_api, edu_vec_print) {
    cr_redirect_stdout();

    const int a[] = {1, 2, 3};
    edu_vec *v = make_int_vec(a, 3);

    edu_vec_print(v, edu_print_i);
    fflush(stdout);

    cr_assert_stdout_eq_str("[1, 2, 3]\n");

    edu_vec_destroy(v);
}

/* ---------- macros ---------- */

Test(vec_macros, EDU_VEC_CREATE_creates_typed_vec) {
    edu_vec *v = EDU_VEC_CREATE(int, 3);
    cr_assert_not_null(v);

    cr_assert_eq(edu_vec_size(v), 3);
    cr_assert_eq(edu_vec_cap(v), 3);
    cr_assert_eq(edu_vec_elem_size(v), sizeof(int));

    for (size_t i = 0; i < 3; ++i) {
        cr_assert_eq(*EDU_VEC_GET_CONST(v, int, i), 0);
    }

    edu_vec_destroy(v);
}

Test(vec_macros, EDU_VEC_CREATE_CAP_creates_empty_with_capacity) {
    edu_vec *v = EDU_VEC_CREATE_CAP(int, 5);
    cr_assert_not_null(v);

    cr_assert_eq(edu_vec_size(v), 0);
    cr_assert_eq(edu_vec_cap(v), 5);
    cr_assert_eq(edu_vec_elem_size(v), sizeof(int));

    edu_vec_destroy(v);
}

Test(vec_macros, EDU_VEC_CREATE_FROM_BUF_wraps_external_buffer) {
    int *buf = malloc(3 * sizeof(int));
    cr_assert_not_null(buf);

    buf[0] = 10;
    buf[1] = 20;
    buf[2] = 30;

    edu_vec *v = EDU_VEC_CREATE_FROM_BUF(int, buf, 3);
    cr_assert_not_null(v);

    cr_assert_eq(edu_vec_size(v), 3);
    cr_assert_eq(edu_vec_cap(v), 3);
    cr_assert_eq(edu_vec_elem_size(v), sizeof(int));
    cr_assert_eq(edu_vec_buf(v), (void *)buf);

    cr_assert_eq(*EDU_VEC_GET(v, int, 1), 20);

    EDU_VEC_SET(v, int, 1, 99);
    cr_assert_eq(buf[1], 99);

    edu_vec_destroy(v);
}

Test(vec_macros, EDU_VEC_PUSH_and_GET_work) {
    edu_vec *v = EDU_VEC_CREATE_CAP(int, 8);
    cr_assert_not_null(v);

    EDU_VEC_PUSH(v, int, 1);
    EDU_VEC_PUSH(v, int, 2);
    EDU_VEC_PUSH(v, int, 3);

    cr_assert_eq(edu_vec_size(v), 3);
    cr_assert_eq(*EDU_VEC_GET(v, int, 0), 1);
    cr_assert_eq(*EDU_VEC_GET(v, int, 1), 2);
    cr_assert_eq(*EDU_VEC_GET(v, int, 2), 3);

    const int *p1 = EDU_VEC_GET_CONST(v, int, 1);
    cr_assert_eq(*p1, 2);

    edu_vec_destroy(v);
}

Test(vec_macros, EDU_VEC_BUF_and_GET_point_to_same_memory) {
    edu_vec *v = EDU_VEC_CREATE_CAP(int, 4);
    cr_assert_not_null(v);

    EDU_VEC_PUSH(v, int, 42);
    EDU_VEC_PUSH(v, int, 43);

    int *b = EDU_VEC_BUF(v, int);
    const int *bc = EDU_VEC_BUF_CONST(v, int);

    cr_assert_not_null(b);
    cr_assert_not_null(bc);

    cr_assert_eq((void *)b, edu_vec_buf(v));
    cr_assert_eq((const void *)bc, edu_vec_buf_const(v));

    cr_assert_eq((void *)EDU_VEC_GET(v, int, 0), (void *)b);
    cr_assert_eq((const void *)EDU_VEC_GET_CONST(v, int, 0), (const void *)bc);

    cr_assert_eq(b[0], 42);
    cr_assert_eq(b[1], 43);

    edu_vec_destroy(v);
}

Test(vec_macros, EDU_VEC_SET_sets_value) {
    edu_vec *v = EDU_VEC_CREATE(int, 2);
    cr_assert_not_null(v);

    EDU_VEC_SET(v, int, 0, 111);
    EDU_VEC_SET(v, int, 1, 222);

    cr_assert_eq(*EDU_VEC_GET(v, int, 0), 111);
    cr_assert_eq(*EDU_VEC_GET(v, int, 1), 222);

    edu_vec_destroy(v);
}
