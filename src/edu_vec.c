#include "edu_vec.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

struct edu_vec {
    size_t size;
    size_t elem_size;
    size_t cap;
    void *buf;
};

static edu_vec *create(size_t size, size_t cap, size_t elem_size);

static bool grow_if_needed(edu_vec *vec);

edu_vec *edu_vec_create(size_t size, size_t elem_size) {
    return create(size, size, elem_size);
}

edu_vec *edu_vec_create_cap(size_t cap, size_t elem_size) {
    return create(cap, 0, elem_size);
}

void edu_vec_destroy(edu_vec *vec) {
    if (!vec) {
        return;
    }

    free(vec->buf);
    free(vec);
}

// info

size_t edu_vec_size(const edu_vec *vec) {
    assert(vec);

    return vec->size;
}

bool edu_vec_empty(const edu_vec *vec) {
    assert(vec);

    return vec->size == 0;
}

size_t edu_vec_cap(const edu_vec *vec) {
    assert(vec);

    return vec->cap;
}

size_t edu_vec_elem_size(const edu_vec *vec) {
    assert(vec);

    return vec->elem_size;
}

// access

void *edu_vec_get(edu_vec *vec, size_t idx) {
    assert(vec);
    assert(idx < vec->size);

    return (char *) vec->buf + idx * vec->elem_size;
}

const void *edu_vec_get_const(const edu_vec *vec, size_t idx) {
    assert(vec);
    assert(idx < vec->size);

    return (const char *) vec->buf + idx * vec->elem_size;
}

void edu_vec_set(edu_vec *vec, size_t idx, const void *elem) {
    assert(vec);
    assert(idx < vec->size);
    assert(elem);

    memcpy(edu_vec_get(vec, idx), elem, vec->elem_size);
}

void *edu_vec_buf(edu_vec *vec) {
    assert(vec);

    return vec->buf;
}

const void *edu_vec_buf_const(const edu_vec *vec) {
    assert(vec);

    return vec->buf;
}

// mods

bool edu_vec_push(edu_vec *vec, const void *elem) {
    assert(vec);
    assert(elem);

    if (!grow_if_needed(vec)) {
        return false;
    }
    ++vec->size;
    edu_vec_set(vec, vec->size - 1, elem);
    return true;
}

bool edu_vec_pop(edu_vec *vec, void *out) {
    assert(vec);

    if (vec->size == 0) {
        return false;
    }

    if (out) {
        memcpy(out, edu_vec_get(vec, vec->size - 1), vec->elem_size);
    }
    --vec->size;
    return true;
}

void edu_vec_clear(edu_vec *vec) {
    assert(vec);

    vec->size = 0;
}

bool edu_vec_reserve(edu_vec *vec, size_t new_cap) {
    assert(vec);

    if (new_cap <= vec->cap) {
        return true;
    }

    void *new_buf = realloc(vec->buf, new_cap * vec->elem_size);
    if (!new_buf) {
        return false;
    }

    vec->buf = new_buf;
    vec->cap = new_cap;

    return true;
}

bool edu_vec_resize(edu_vec *vec, size_t new_size) {
    assert(vec);

    if (new_size <= vec->size) {
        vec->size = new_size;
        return true;
    }

    if (new_size > vec->cap) {
        if (!edu_vec_reserve(vec, new_size)) {
            return false;
        }
    }

    const size_t old_size = vec->size;
    memset((char *) vec->buf + old_size * vec->elem_size, 0, (new_size - old_size) * vec->elem_size);
    vec->size = new_size;

    return true;
}

bool edu_vec_shrink_to_fit(edu_vec *vec) {
    assert(vec);

    if (vec->cap == vec->size) {
        return true;
    }

    if (vec->size == 0) {
        free(vec->buf);
        vec->buf = NULL;
        vec->cap = 0;
        return true;
    }

    void *new_buf = realloc(vec->buf, vec->size * vec->elem_size);
    if (!new_buf) {
        return false;
    }

    vec->buf = new_buf;
    vec->cap = vec->size;

    return true;
}

void edu_vec_fill(edu_vec *vec, const void *elem) {
    assert(vec);
    assert(elem);

    if (vec->size == 0) {
        return;
    }

    for (size_t i = 0; i < vec->size; ++i) {
        memcpy(edu_vec_get(vec, i), elem, vec->elem_size);
    }
}

void edu_vec_swap(edu_vec *a, edu_vec *b) {
    assert(a);
    assert(b);

    const edu_vec tmp = *a;
    *a = *b;
    *b = tmp;
}

// relations

bool edu_vec_eq(const edu_vec *a, const edu_vec *b, edu_cmp cmp) {
    assert(a);
    assert(b);

    if (a->size != b->size) {
        return false;
    }
    for (size_t i = 0; i < a->size; ++i) {
        if (cmp(edu_vec_get_const(a, i), edu_vec_get_const(b, i)) != 0) {
            return false;
        }
    }
    return true;
}

bool edu_vec_not_eq(const edu_vec *a, const edu_vec *b, edu_cmp cmp) {
    assert(a);
    assert(b);

    return !edu_vec_eq(a, b, cmp);
}

// print

void edu_vec_print(const edu_vec *vec, edu_print_func f) {
    assert(vec);
    assert(f);

    printf("[");
    for (size_t i = 0; i < vec->size; ++i) {
        f(edu_vec_get_const(vec, i));
        if (i != vec->size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

// internals

static edu_vec *create(size_t size, size_t cap, size_t elem_size) {
    if (elem_size == 0) {
        return NULL;
    }
    if (size > cap) {
        return NULL;
    }

    edu_vec *vec = malloc(sizeof(*vec));
    if (!vec) {
        return NULL;
    }

    vec->size = size;
    vec->elem_size = elem_size;
    vec->cap = cap;
    vec->buf = NULL;

    if (cap == 0) {
        return vec;
    }

    void *buf = malloc(vec->cap * vec->elem_size);
    if (!buf) {
        free(vec);
        return NULL;
    }
    vec->buf = buf;
    return vec;
}

static bool grow_if_needed(edu_vec *vec) {
    assert(vec);

    if (vec->size < vec->cap) {
        return true;
    }

    const size_t new_cap = vec->cap == 0 ? 1 : vec->cap * 2;
    return edu_vec_reserve(vec, new_cap);
}
