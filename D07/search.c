#include <math.h>
#include <stdio.h>

#define NMAX 30

// Читает длину и элементы массива; 1 - успех, 0 - ошибка.
int input(int *a, int *n);
// Вычисляет математическое ожидание массива.
double mean(int *a, int n);
// Вычисляет дисперсию массива.
double variance(int *a, int n);
// Проверяет, подходит ли число под все условия задачи.
int is_match(int value, double mean_value, double variance_value);
// Ищет первое число, подходящее под условия; если нет, возвращает 0.
int find_first(int *a, int n, double mean_value, double variance_value);

int main(void) {
    int n = 0;
    int data[NMAX];

    if (input(data, &n) == 1) {
        double mean_value = mean(data, n);
        double variance_value = variance(data, n);
        int found = find_first(data, n, mean_value, variance_value);
        printf("%d", found);
    } else {
        printf("n/a");
    }

    return 0;
}

int input(int *a, int *n) {
    int ok = 1;

    if (scanf("%d", n) != 1) {
        ok = 0;
    }
    if (ok == 1 && (*n < 1 || *n > NMAX)) {
        ok = 0;
    }

    for (int i = 0; i < *n && ok == 1; i++) {
        if (scanf("%d", &a[i]) != 1) {
            ok = 0;
        }
    }

    return ok;
}

double mean(int *a, int n) {
    double sum = 0.0;

    for (int i = 0; i < n; i++) {
        sum += a[i];
    }

    return sum / n;
}

double variance(int *a, int n) {
    double mean_value = mean(a, n);
    double sum_sq_diff = 0.0;

    for (int i = 0; i < n; i++) {
        double diff = a[i] - mean_value;
        sum_sq_diff += diff * diff;
    }

    return sum_sq_diff / n;
}

int is_match(int value, double mean_value, double variance_value) {
    int result = 0;

    // Граница по правилу трех сигм: mean + 3 * sqrt(variance).
    double sigma_border = mean_value + 3.0 * sqrt(variance_value);

    // Проверяем все условия из задания.
    if (value % 2 == 0 && value >= mean_value && value <= sigma_border && value != 0) {
        result = 1;
    }

    return result;
}

int find_first(int *a, int n, double mean_value, double variance_value) {
    int found = 0;
    int index = 0;
    int continue_search = 1;

    // Идем слева направо и останавливаемся на первом подходящем значении.
    while (index < n && continue_search == 1) {
        if (is_match(a[index], mean_value, variance_value) == 1) {
            found = a[index];
            continue_search = 0;
        }
        index++;
    }

    return found;
}
