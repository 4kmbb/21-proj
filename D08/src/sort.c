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

    // Если ввод прошел успешно, сортируем и печатаем массив.
    if (status == 1) {
        sort(a, n);
        output(a, n);
    } else {
        // При любой ошибке показываем "n/a" без перевода строки.
        printf("n/a");
    }

    // Освобождаем память в любом случае, даже если была ошибка.
    free_array(&a);
    return 0;
}

// Читает количество элементов, выделяет память и читает сам массив.
// Возвращает 1 при успехе и 0 при ошибке.
int input(int** a, int* n) {
    int status = 1;

    if (scanf("%d", n) != 1 || *n <= 0) {
        status = 0;
    } else {
        // Выделяем память ровно под n целых чисел.
        *a = (int*)malloc((size_t)*n * sizeof(int));
        if (*a == NULL) {
            status = 0;
        } else {
            // Читаем элементы, пока не встретим ошибку.
            for (int i = 0; i < *n && status == 1; i++) {
                if (scanf("%d", &(*a)[i]) != 1) {
                    status = 0;
                }
            }
        }
    }

    // Если что-то пошло не так, память нужно освободить здесь же.
    if (status == 0) {
        free_array(a);
    }
    return status;
}

// Сортировка выбором: на каждом шаге ставим на место i минимальный элемент.
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

// Печатаем элементы через пробел. После последнего элемента нет '\n'.
void output(const int* a, int n) {
    for (int i = 0; i < n; i++) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", a[i]);
    }
}

// Безопасно освобождаем память и обнуляем указатель.
void free_array(int** a) {
    if (a != NULL && *a != NULL) {
        free(*a);
        *a = NULL;
    }
}
