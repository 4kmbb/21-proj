#include <stdio.h>

int is_divisible(int number, int divisor) {
    int remainder = number;
    while (remainder > 0) {
        remainder = remainder - divisor;
    }
    return remainder == 0;
}

int is_prime(int number) {
    int result = 1;
    int divisor = 2;

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

int largest_prime_factor(int number) {
    int result = -1;
    int candidate = 2;

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
    int number = 0;
    char end = '\0';

    if (scanf("%d%c", &number, &end) == 2 && end == '\n') {
        int result = largest_prime_factor(number);
        if (result == -1) {
            printf("n/a");
        } else {
            printf("%d", result);
        }
    } else {
        printf("n/a");
    }

    return 0;
}
