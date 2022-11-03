#define DYNAMIC_ARRAY_IMPLEMENTATION
#include <dynamic_array.h>

#include <stdio.h>

int main(void) {
    char *a = dynamic_array_new();

    dynamic_array_reserve((void **)&a, 13, sizeof(*a));

    dynamic_array_insert((void **)&a, 0, 5, "Hello", sizeof(*a));
    dynamic_array_insert((void **)&a, dynamic_array_length(a), 8, ", World!", sizeof(*a));

    printf("%.*s\n", (int)dynamic_array_length(a), a);

    dynamic_array_delete(a);
    return 0;
}
