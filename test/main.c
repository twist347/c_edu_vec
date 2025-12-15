#include "edu_vec.h"
#include "criterion/criterion.h"

Test(vec, push_pop) {
    edu_vec *v = edu_vec_create(0, sizeof(int));
    cr_assert_not_null(v);

    int x = 42;
    cr_assert(edu_vec_push(v, &x));

    cr_assert_eq(edu_vec_size(v), 1);

    int out = 0;
    cr_assert(edu_vec_pop(v, &out));
    cr_assert_eq(out, 42);
    cr_assert(edu_vec_empty(v));

    edu_vec_destroy(v);
}

Test(vec, insert_erase) {
    edu_vec *v = edu_vec_create(0, sizeof(int));

    int a = 1, b = 3, c = 2;
    edu_vec_push(v, &a);
    edu_vec_push(v, &b);

    cr_assert(edu_vec_insert(v, 1, &c)); // [1,2,3]
    cr_assert_eq(*(int*)edu_vec_get(v, 0), 1);
    cr_assert_eq(*(int*)edu_vec_get(v, 1), 2);
    cr_assert_eq(*(int*)edu_vec_get(v, 2), 3);

    int erased = 0;
    cr_assert(edu_vec_erase(v, 1, &erased));
    cr_assert_eq(erased, 2);
    cr_assert_eq(edu_vec_size(v), 2);

    edu_vec_destroy(v);
}

Test(vec, sort_find_contains) {
    edu_vec *v = edu_vec_create(0, sizeof(int));
    int a = 3, b = 1, c = 2;
    edu_vec_push(v, &a);
    edu_vec_push(v, &b);
    edu_vec_push(v, &c);

    edu_vec_sort(v, edu_cmp_i);

    cr_assert_eq(*(int*)edu_vec_get(v, 0), 1);
    cr_assert_eq(*(int*)edu_vec_get(v, 1), 2);
    cr_assert_eq(*(int*)edu_vec_get(v, 2), 3);

    int key2 = 2, key9 = 9;
    cr_assert_eq(edu_vec_find(v, &key2, edu_cmp_i), 1);
    cr_assert_eq(edu_vec_find(v, &key9, edu_cmp_i), -1);
    cr_assert(edu_vec_contains(v, &key2, edu_cmp_i));
    cr_assert_not(edu_vec_contains(v, &key9, edu_cmp_i));

    edu_vec_destroy(v);
}
