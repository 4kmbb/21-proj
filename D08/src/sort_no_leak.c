#include <stdio.h>
#include <stdlib.h>

int input(int** a, int* n);
void sort(int* a, int n);
void output(const int* a, int n);
void free_array(int** a);

int main(void) {
    int n = 0;
    int* a = NULL;
    int status = input(&a, &n);

    if (status == 1) {
        sort(a, n);
        output(a, n);
    } else {
        printf("n/a");
    }

    // Главное отличие версии без утечек: память освобождается
    // и при успешной работе, и при любой ошибке ввода.
    free_array(&a);
    return 0;
}

// Читает размер, выделяет память под массив и вводит его элементы.
int input(int** a, int* n) {
    int status = 1;

    if (scanf("%d", n) != 1 || *n <= 0) {
        status = 0;
    } else {
        *a = (int*)malloc((size_t)*n * sizeof(int));
        if (*a == NULL) {
            status = 0;
        } else {
            for (int i = 0; i < *n && status == 1; i++) {
                if (scanf("%d", &(*a)[i]) != 1) {
                    status = 0;
                }
            }
        }
    }

    // Если выделенная память больше не нужна из-за ошибки, освобождаем ее.
    if (status == 0) {
        free_array(a);
    }
    return status;
}

// Сортировка выбором.
void sort(int* a, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[i]) {
                int temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            }
        }
    }
}

// Вывод массива через пробел, без перевода строки в конце.
void output(const int* a, int n) {
    for (int i = 0; i < n; i++) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", a[i]);
    }
}

// Освобождение памяти и обнуление указателя.
void free_array(int** a) {
    if (a != NULL && *a != NULL) {
        free(*a);
        *a = NULL;
    }
}
