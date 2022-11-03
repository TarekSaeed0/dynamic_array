#define DYNAMIC_ARRAY_IMPLEMENTATION
#include <dynamic_array.h>

#include <stdio.h>

int main_string(void) {
    char *string = dynamic_array_new();

    dynamic_array_reserve((void **)&string, 13, sizeof(*string));

    dynamic_array_insert((void **)&string, 0, 5, "Hello", sizeof(*string));
    dynamic_array_insert((void **)&string, dynamic_array_length(string), 8, ", World!", sizeof(*string));

    printf("%.*s\n", (int)dynamic_array_length(string), string);

    dynamic_array_delete(string);
    return 0;
}

int main_primes(void) {
    unsigned long limit = 1000;
    unsigned long *primes = dynamic_array_new();
    unsigned long number;
    size_t index;

    if (limit >= 2) {
        number = 2;
        dynamic_array_insert((void **)&primes, dynamic_array_length(primes), 1, &number, sizeof(*primes));
    }
    for (number = 3; number < limit; number += 2) {
        int is_prime = 1;
        for (index = 0; index < dynamic_array_length(primes); index++) {
            if (!(number % primes[index])) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            dynamic_array_insert((void **)&primes, dynamic_array_length(primes), 1, &number, sizeof(*primes));
        }
    }

    for (index = 0; index < dynamic_array_length(primes); index++) {
        printf("%lu, ", primes[index]);
    }

    dynamic_array_delete(primes);
    return 0;
}

int main(void) {
    return main_primes();
}
