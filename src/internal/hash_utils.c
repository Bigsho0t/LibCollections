#include "hash_utils.h"
#include <stdbool.h>

static bool is_prime(size_t n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;

    for (size_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }

    return true;
}

size_t next_prime(size_t n) {
    while (!is_prime(n))
        n++;

    return n;
}