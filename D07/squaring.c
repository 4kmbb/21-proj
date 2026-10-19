#include <stdio.h>

#define NMAX 10

// Считывает размер и элементы массива; возвращает 1 при успехе и 0 при ошибке.
int input(int *a, int *n);
// Печатает массив в одну строку через пробел.
void output(int *a, int n);
// Возводит каждый элемент массива в квадрат.
void squaring(int *a, int n);

int main(void) {
    int n = 0;
    int data[NMAX];

    // Выполняем чтение, затем обработку и вывод только при корректном вводе.
    if (input(data, &n) == 1) {
        squaring(data, n);
        output(data, n);
    } else {
        printf("n/a");
    }

    return 0;
}

int input(int *a, int *n) {
    int ok = 1;

    // Сначала читаем длину массива.
    if (scanf("%d", n) != 1) {
        ok = 0;
    }

    // Проверяем допустимый диапазон длины.
    if (ok == 1 && (*n < 1 || *n > NMAX)) {
        ok = 0;
    }

    // Считываем элементы массива.
    for (int i = 0; i < *n && ok == 1; i++) {
        if (scanf("%d", &a[i]) != 1) {
            ok = 0;
        }
    }

    return ok;
}

void output(int *a, int n) {
    // Печатаем элементы аккуратно: без лишнего пробела после последнего числа.
    for (int i = 0; i < n; i++) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", a[i]);
    }
}

void squaring(int *a, int n) {
    // Проходим по массиву и заменяем каждый элемент его квадратом.
    for (int i = 0; i < n; i++) {
        a[i] = a[i] * a[i];
    }
}
