#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stddef.h>

enum dynamic_array_error {
    dynamic_array_error_none,
    dynamic_array_error_null_pointer,
    dynamic_array_error_allocation_failure,
    dynamic_array_error_out_of_range
};

void *dynamic_array_new(void);
void dynamic_array_delete(void *dynamic_array);

size_t dynamic_array_capacity(const void *dynamic_array);
size_t dynamic_array_maximum_capacity(size_t size);
enum dynamic_array_error dynamic_array_set_capacity(void **dynamic_array, size_t capacity, size_t size);

size_t dynamic_array_length(const void *dynamic_array);
enum dynamic_array_error dynamic_array_set_length(void *dynamic_array, size_t length);

enum dynamic_array_error dynamic_array_reserve(void **dynamic_array, size_t length, size_t size);

enum dynamic_array_error dynamic_array_resize(void **dynamic_array, size_t length, const void *value, size_t size);

enum dynamic_array_error dynamic_array_insert(void **dynamic_array, size_t index, size_t length, const void *values, size_t size);
enum dynamic_array_error dynamic_array_remove(void *dynamic_array, size_t index, size_t length, void *values, size_t size);

#ifdef DYNAMIC_ARRAY_IMPLEMENTATION

#include <string.h>

struct dynamic_array_private_header {
    size_t capacity, length;
};

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
    struct dynamic_array_private_type {
        struct dynamic_array_private_header header;
        max_align_t array;
    };
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    #include <stdint.h>
    #include <math.h>
    struct dynamic_array_private_type {
        struct dynamic_array_private_header header;
        union {
            intmax_t _0;
            long double _1;
            double_t _2;
            void *_3;
            void (*_4)();
        } array;
    };
#else
    struct dynamic_array_private_type {
        struct dynamic_array_private_header header;
        union {
            long _0;
            size_t _1;
            ptrdiff_t _2;
            long double _3;
            void *_4;
            void (*_5)();
        } array;
    };
#endif

#if !defined(dynamic_array_allocate) && !defined(dynamic_array_reallocate) && !defined(dynamic_array_reallocate)
    #include <stdlib.h>
    #define dynamic_array_allocate malloc
    #define dynamic_array_reallocate realloc
    #define dynamic_array_deallocate free
#elif !(defined(dynamic_array_allocate) && defined(dynamic_array_reallocate) && defined(dynamic_array_deallocate))
    #error dynamic_array_allocate, dynamic_array_reallocate and dynamic_array_deallocate must all be defined
#endif

void *dynamic_array_new(void) {
    return NULL;
}
void dynamic_array_delete(void *dynamic_array) {
    if (dynamic_array) {
        dynamic_array_deallocate((char *)dynamic_array - offsetof(struct dynamic_array_private_type, array));
    }
}

size_t dynamic_array_capacity(const void *dynamic_array) {
    return dynamic_array ? ((const struct dynamic_array_private_header *)((const char *)dynamic_array - offsetof(struct dynamic_array_private_type, array)))->capacity : 0;
}
size_t dynamic_array_maximum_capacity(size_t size) {
    return ((size_t)-1 - offsetof(struct dynamic_array_private_type, array)) / size;
}
enum dynamic_array_error dynamic_array_set_capacity(void **dynamic_array, size_t capacity, size_t size) {
    if (!dynamic_array) {
        return dynamic_array_error_null_pointer;
    }
    if (!size || capacity < dynamic_array_length(*dynamic_array) || capacity > dynamic_array_maximum_capacity(size)) {
        return dynamic_array_error_out_of_range;
    }
    if (capacity != dynamic_array_capacity(*dynamic_array)) {
        if (*dynamic_array) {
            if (capacity) {
                struct dynamic_array_private_header *new_dynamic_array = dynamic_array_reallocate((char *)*dynamic_array - offsetof(struct dynamic_array_private_type, array), offsetof(struct dynamic_array_private_type, array) + capacity * size);
                if (!new_dynamic_array) {
                    return dynamic_array_error_allocation_failure;
                }
                new_dynamic_array->capacity = capacity;
                *dynamic_array = (char *)new_dynamic_array + offsetof(struct dynamic_array_private_type, array);
            } else {
                dynamic_array_deallocate((char *)*dynamic_array - offsetof(struct dynamic_array_private_type, array));
                *dynamic_array = NULL;
            }
        } else {
            *dynamic_array = dynamic_array_allocate(offsetof(struct dynamic_array_private_type, array) + capacity * size);
            if (!*dynamic_array) {
                return dynamic_array_error_allocation_failure;
            }
            ((struct dynamic_array_private_header *)*dynamic_array)->capacity = capacity;
            ((struct dynamic_array_private_header *)*dynamic_array)->length = 0;
            *dynamic_array = (char *)*dynamic_array + offsetof(struct dynamic_array_private_type, array);
        }
    }
    return dynamic_array_error_none;
}

size_t dynamic_array_length(const void *dynamic_array) {
    return dynamic_array ? ((const struct dynamic_array_private_header *)((const char *)dynamic_array - offsetof(struct dynamic_array_private_type, array)))->length : 0;
}
enum dynamic_array_error dynamic_array_set_length(void *dynamic_array, size_t length) {
    if (length > dynamic_array_capacity(dynamic_array)) {
        return dynamic_array_error_out_of_range;
    }
    if (length != dynamic_array_length(dynamic_array)) {
        ((struct dynamic_array_private_header *)((char *)dynamic_array - offsetof(struct dynamic_array_private_type, array)))->length = length;
    }
    return dynamic_array_error_none;
}

#ifndef dynamic_array_reserve_capacity
    size_t dynamic_array_reserve_capacity(const void *dynamic_array, size_t length, size_t size) {
        size_t capacity = dynamic_array ? dynamic_array_capacity(dynamic_array) : 1;
        while (capacity < length) {
            if (capacity <= dynamic_array_maximum_capacity(size) / 2) {
                capacity *= 2;
            } else {
                capacity = dynamic_array_maximum_capacity(size);
                break;
            }
        }
        return capacity;
    }
    #define dynamic_array_reserve_capacity dynamic_array_reserve_capacity
#endif
enum dynamic_array_error dynamic_array_reserve(void **dynamic_array, size_t length, size_t size) {
    if (!dynamic_array) {
        return dynamic_array_error_null_pointer;
    }
    if (!size || length > dynamic_array_maximum_capacity(size)) {
        return dynamic_array_error_out_of_range;
    }
    if (length > dynamic_array_capacity(*dynamic_array)) {
        size_t capacity = dynamic_array_reserve_capacity(*dynamic_array, length, size);
        if (capacity < length) {
            return dynamic_array_error_out_of_range;
        }
        return dynamic_array_set_capacity(dynamic_array, capacity, size);
    }
    return dynamic_array_error_none;
}

enum dynamic_array_error dynamic_array_resize(void **dynamic_array, size_t length, const void *value, size_t size) {
    if (!dynamic_array || (!value && length > dynamic_array_length(*dynamic_array))) {
        return dynamic_array_error_null_pointer;
    }
    if (!size) {
        return dynamic_array_error_out_of_range;
    }
    if (length != dynamic_array_length(*dynamic_array)) {
        enum dynamic_array_error error = dynamic_array_reserve(dynamic_array, length, size);
        if (error != dynamic_array_error_none) {
            return error;
        }
        if (value) {
            size_t index;
            for (index = dynamic_array_length(*dynamic_array); index < length; index++) {
                memmove((char *)*dynamic_array + index * size, value, size);
            }
        }
        dynamic_array_set_length(*dynamic_array, length);
    }
    return dynamic_array_error_none;
}

enum dynamic_array_error dynamic_array_insert(void **dynamic_array, size_t index, size_t length, const void *values, size_t size) {
    enum dynamic_array_error error;
    if (!dynamic_array || (!values && length)) {
        return dynamic_array_error_null_pointer;
    }
    if (index > dynamic_array_length(*dynamic_array) || dynamic_array_length(*dynamic_array) > (size_t)-1 - length) {
        return dynamic_array_error_out_of_range;
    }
    error = dynamic_array_reserve(dynamic_array, dynamic_array_length(*dynamic_array) + length, size);
    if (error != dynamic_array_error_none) {
        return error;
    }
    memmove((char *)*dynamic_array + (index + length) * size, (char *)*dynamic_array + index * size, (dynamic_array_length(*dynamic_array) - index) * size);
    if (values) {
        memmove((char *)*dynamic_array + index * size, values, length * size);
    }
    dynamic_array_set_length(*dynamic_array, dynamic_array_length(*dynamic_array) + length);
    return dynamic_array_error_none;
}
enum dynamic_array_error dynamic_array_remove(void *dynamic_array, size_t index, size_t length, void *values, size_t size) {
    if (!size || index > dynamic_array_length(dynamic_array) || length > dynamic_array_length(dynamic_array) - index) {
        return dynamic_array_error_out_of_range;
    }
    if (values) {
        memmove(values, (char *)dynamic_array + index * size, length * size);
    }
    memmove((char *)dynamic_array + index * size, (char *)dynamic_array + (index + length) * size, (dynamic_array_length(dynamic_array) - index) * size);
    dynamic_array_set_length(dynamic_array, dynamic_array_length(dynamic_array) - length);
    return dynamic_array_error_none;
}

#endif

#endif
