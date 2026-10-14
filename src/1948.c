#include <stdio.h>

int is_divisible(long long number, long long divisor) {
    long long remainder = number;
    while (remainder > 0) {
        remainder = remainder - divisor;
    }
    return remainder == 0;
}

int is_prime(long long number) {
    int result = 1;
    long long divisor = 2;

    if (number < 2) {
        result = 0;
    }

    while (divisor < number && result == 1) {
        if (is_divisible(number, divisor)) {
            result = 0;
        }
        divisor = divisor + 1;
    }

    return result;
}

long long largest_prime_factor(long long number) {
    long long result = -1;
    long long candidate = 2;

    if (number < 0) {
        number = -number;
    }

    while (candidate <= number) {
        if (is_prime(candidate) && is_divisible(number, candidate)) {
            result = candidate;
        }
        candidate = candidate + 1;
    }

    return result;
}

int main(void) {
    long long number = 0;
    char end = '\0';

    if (scanf("%lld%c", &number, &end) == 2 && end == '\n') {
        long long result = largest_prime_factor(number);
        if (result == -1) {
            printf("n/a");
        } else {
            printf("%lld", result);
        }
    } else {
        printf("n/a");
    }

    return 0;
}
