#include <stdio.h>

#define NMAX 10

// Считывает n, массив из n элементов и сдвиг c; возвращает 1 при успехе и 0 при ошибке.
int input(int *a, int *n, int *c);
// Выполняет циклический сдвиг влево на c позиций; отрицательный c означает сдвиг вправо.
void cycle_shift(int *a, int n, int c);
// Выводит массив в одну строку через пробел.
void output(int *a, int n);

// Точка входа программы Quest 7.
int main(void) {
    int n = 0;
    int c = 0;
    int data[NMAX];

    // Делаем обработку только при полностью корректном вводе.
    if (input(data, &n, &c) == 1) {
        cycle_shift(data, n, c);
        output(data, n);
    } else {
        // При ошибке ввода выводим n/a.
        printf("n/a");
    }

    return 0;
}

int input(int *a, int *n, int *c) {
    int ok = 1;

    // Считываем длину массива.
    if (scanf("%d", n) != 1) {
        ok = 0;
    }

    // Проверяем диапазон длины по условию задачи.
    if (ok == 1 && (*n < 1 || *n > NMAX)) {
        ok = 0;
    }

    // Считываем элементы массива.
    for (int i = 0; i < *n && ok == 1; i++) {
        if (scanf("%d", &a[i]) != 1) {
            ok = 0;
        }
    }

    // Считываем число сдвига c.
    if (ok == 1 && scanf("%d", c) != 1) {
        ok = 0;
    }

    return ok;
}

void cycle_shift(int *a, int n, int c) {
    // Переводим любой сдвиг в эквивалентный сдвиг влево в диапазоне 0..n-1.
    int shift = c % n;

    // Если остаток отрицательный, приводим его к положительному сдвигу влево.
    if (shift < 0) {
        shift = shift + n;
    }

    // Делаем сдвиг по одной позиции влево, повторяя это shift раз.
    for (int step = 0; step < shift; step++) {
        int first = a[0];

        // Сдвигаем все элементы на одну позицию влево.
        for (int i = 0; i < n - 1; i++) {
            a[i] = a[i + 1];
        }

        // Первый элемент переносим в конец массива.
        a[n - 1] = first;
    }
}

void output(int *a, int n) {
    // Выводим массив через пробел без лишнего пробела в конце.
    for (int i = 0; i < n; i++) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", a[i]);
    }
}
