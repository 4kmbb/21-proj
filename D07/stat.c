#include <stdio.h>

#define NMAX 10

int input(int *a, int *n);
void output(int *a, int n);
int max(int *a, int n);
int min(int *a, int n);
double mean(int *a, int n);
double variance(int *a, int n);
void output_result(int max_v, int min_v, double mean_v, double variance_v);

int main(void) {
    int n = 0;
    int data[NMAX];

    if (input(data, &n) == 1) {
        output(data, n);
        printf("\n");
        output_result(max(data, n), min(data, n), mean(data, n), variance(data, n));
    } else {
        printf("n/a");
    }

    return 0;
}

// Чтение длины и элементов массива с проверкой корректности.
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

// Печать массива в формате: x1 x2 x3 ...
void output(int *a, int n) {
    for (int i = 0; i < n; i++) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", a[i]);
    }
}

// Поиск максимального элемента.
int max(int *a, int n) {
    int max_value = a[0];

    for (int i = 1; i < n; i++) {
        if (a[i] > max_value) {
            max_value = a[i];
        }
    }

    return max_value;
}

// Поиск минимального элемента.
int min(int *a, int n) {
    int min_value = a[0];

    for (int i = 1; i < n; i++) {
        if (a[i] < min_value) {
            min_value = a[i];
        }
    }

    return min_value;
}

// Математическое ожидание для дискретного равномерного распределения: сумма / n.
double mean(int *a, int n) {
    double sum = 0.0;

    for (int i = 0; i < n; i++) {
        sum += a[i];
    }

    return sum / n;
}

// Дисперсия: среднее значение квадратов отклонений от математического ожидания.
double variance(int *a, int n) {
    double mean_value = mean(a, n);
    double sum_sq_diff = 0.0;

    for (int i = 0; i < n; i++) {
        double diff = a[i] - mean_value;
        sum_sq_diff += diff * diff;
    }

    return sum_sq_diff / n;
}

// Вывод вычисленных метрик в требуемом формате.
void output_result(int max_v, int min_v, double mean_v, double variance_v) {
    printf("%d %d %.6lf %.6lf", max_v, min_v, mean_v, variance_v);
}
