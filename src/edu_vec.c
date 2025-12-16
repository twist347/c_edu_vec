#include "edu_vec.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

struct edu_vec {
    size_t elem_size;
    size_t size;
    size_t cap;
    void *buf;
};

// internals decls

static edu_vec *create(size_t size, size_t cap, size_t elem_size);
static bool grow_if_needed(edu_vec *vec);
static void set_fields(edu_vec *vec, size_t elem_size, size_t size, size_t cap, void *buf);
static void reset_fields(edu_vec *vec);
static char *ptr_at(edu_vec *vec, size_t idx);
static const char *ptr_at_c(const edu_vec *vec, size_t idx);
static void shift_left(edu_vec *vec, size_t idx);
static void shift_right(edu_vec *vec, size_t idx);
static void *alloc_and_copy_buf(const edu_vec *from);

// create/destroy

edu_vec *edu_vec_create(size_t size, size_t elem_size) {
    return create(size, size, elem_size);
}

edu_vec *edu_vec_create_cap(size_t cap, size_t elem_size) {
    return create(0, cap, elem_size);
}

edu_vec *edu_vec_create_from_buf(void *buf, size_t size, size_t elem_size) {
    if (elem_size == 0) {
        return NULL;
    }
    if (size != 0 && buf == NULL) {
        return NULL;
    }

    edu_vec *vec = malloc(sizeof(*vec));
    if (!vec) {
        return NULL;
    }

    set_fields(vec, elem_size, size, size, buf);

    return vec;
}

void edu_vec_destroy(edu_vec *vec) {
    if (!vec) {
        return;
    }

    free(vec->buf);
    free(vec);
}

// "copy and move semantic"

edu_vec *edu_vec_copy(const edu_vec *from) {
    assert(from);

    edu_vec *to = malloc(sizeof(*to));
    if (!to) {
        return NULL;
    }

    set_fields(to, from->elem_size, from->size, from->cap, NULL);

    if (from->cap == 0) {
        return to;
    }

    to->buf = alloc_and_copy_buf(from);
    if (!to->buf) {
        free(to);
        return NULL;
    }

    return to;
}

edu_vec *edu_vec_move(edu_vec *from) {
    assert(from);

    edu_vec *to = malloc(sizeof(*to));
    if (!to) {
        return NULL;
    }

    *to = *from;

    reset_fields(from);

    return to;
}

bool edu_vec_copy_assign(edu_vec *to, const edu_vec *from) {
    assert(to);
    assert(from);

    if (to == from) {
        return true;
    }

    void *new_buf = alloc_and_copy_buf(from);
    if (from->cap != 0 && !new_buf) {
        return false;
    }

    free(to->buf);

    set_fields(to, from->elem_size, from->size, from->cap, new_buf);

    return true;
}

void edu_vec_move_assign(edu_vec *to, edu_vec *from) {
    assert(to);
    assert(from);

    if (to == from) {
        return;
    }

    free(to->buf);

    *to = *from;

    reset_fields(from);
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

    return ptr_at(vec, idx);
}

const void *edu_vec_get_const(const edu_vec *vec, size_t idx) {
    assert(vec);
    assert(idx < vec->size);

    return ptr_at_c(vec, idx);
}

void edu_vec_set(edu_vec *vec, size_t idx, const void *elem) {
    assert(vec);
    assert(idx < vec->size);
    assert(elem);

    memcpy(ptr_at(vec, idx), elem, vec->elem_size);
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

bool edu_vec_insert(edu_vec *vec, size_t idx, const void *elem) {
    assert(vec);
    assert(elem);
    assert(idx <= vec->size);

    if (!grow_if_needed(vec)) {
        return false;
    }

    shift_right(vec, idx);
    edu_vec_set(vec, idx, elem);
    ++vec->size;

    return true;
}

bool edu_vec_erase(edu_vec *vec, size_t idx, void *out) {
    assert(vec);
    assert(idx < vec->size);

    if (out) {
        memcpy(out, ptr_at(vec, idx), vec->elem_size);
    }

    shift_left(vec, idx);

    --vec->size;
    return true;
}

// relations

bool edu_vec_eq(const edu_vec *a, const edu_vec *b, edu_cmp cmp) {
    assert(a);
    assert(b);
    assert(cmp);

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
    assert(cmp);

    return !edu_vec_eq(a, b, cmp);
}

// algs

void edu_vec_sort(edu_vec *vec, edu_cmp cmp) {
    assert(vec);
    assert(cmp);

    qsort(vec->buf, vec->size, vec->elem_size, cmp);
}

ptrdiff_t edu_vec_find(const edu_vec *vec, const void *key, edu_cmp cmp) {
    assert(vec);
    assert(key);
    assert(cmp);

    for (size_t i = 0; i < vec->size; ++i) {
        if (cmp(edu_vec_get_const(vec, i), key) == 0) {
            return (ptrdiff_t) i;
        }
    }
    return -1;
}

bool edu_vec_contains(const edu_vec *vec, const void *key, edu_cmp cmp) {
    assert(vec);
    assert(key);
    assert(cmp);

    return edu_vec_find(vec, key, cmp) != -1;
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

// internals defs

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

    set_fields(vec, elem_size, size, cap, NULL);

    if (cap == 0) {
        return vec;
    }

    void *buf = calloc(vec->cap, vec->elem_size);
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

static void set_fields(edu_vec *vec, size_t elem_size, size_t size, size_t cap, void *buf) {
    assert(vec);

    vec->elem_size = elem_size;
    vec->size = size;
    vec->cap = cap;
    vec->buf = buf;
}

static void reset_fields(edu_vec *vec) {
    assert(vec);

    vec->size = 0;
    vec->cap = 0;
    vec->buf = NULL;
}

static char *ptr_at(edu_vec *vec, size_t idx) {
    assert(vec);

    return (char *) vec->buf + idx * vec->elem_size;
}

static const char *ptr_at_c(const edu_vec *vec, size_t idx) {
    assert(vec);

    return (const char *) vec->buf + idx * vec->elem_size;
}

static void shift_left(edu_vec *vec, size_t idx) {
    assert(vec);

    const size_t es = vec->elem_size;
    memmove(ptr_at(vec, idx), ptr_at(vec, idx + 1), (vec->size - idx - 1) * es);
}

static void shift_right(edu_vec *vec, size_t idx) {
    assert(vec);

    const size_t es = vec->elem_size;
    memmove(ptr_at(vec, idx + 1), ptr_at(vec, idx), (vec->size - idx) * es);
}

static void *alloc_and_copy_buf(const edu_vec *from) {
    if (from->cap == 0) {
        return NULL;
    }

    void *buf = malloc(from->cap * from->elem_size);
    if (!buf) {
        return NULL;
    }

    memcpy(buf, from->buf, from->size * from->elem_size);
    return buf;
}
