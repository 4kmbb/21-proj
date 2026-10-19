#include <stdio.h>

long long fibonacci_recursive(int n) {
    /* Классическая рекурсивная формула Фибоначчи. */
    long long result = 0;

    if (n == 0) {
        result = 0;
    } else if (n == 1) {
        result = 1;
    } else {
        result = fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2);
    }

    return result;
}

int main(void) {
    int n = 0;
    char end = '\0';

    /* Проверяем корректность ввода: одно неотрицательное число. */
    if (scanf("%d%c", &n, &end) == 2 && end == '\n' && n >= 0) {
        printf("%lld", fibonacci_recursive(n));
    } else {
        printf("n/a");
    }

    return 0;
}
