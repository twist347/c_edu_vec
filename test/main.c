#include "edu_vec.h"

#include <stdio.h>

int main(void) {
    const int n = 20;
    edu_vec *vec = edu_vec_create(0, sizeof(int));

    for (int i = 0; i < n; ++i) {
        edu_vec_push(vec, &i);
    }

    printf("%d\n", *(int *) edu_vec_get(vec, n - 1));

    edu_vec_print(vec, edu_print_i);

    edu_vec_destroy(vec);
}
