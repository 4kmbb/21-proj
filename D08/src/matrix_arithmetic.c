#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int read_size(int* n, int* m);
int create_matrix(int*** matrix, int n, int m);
int sizes_are_safe(int n, int m);
int input(int** matrix, int n, int m);
void output(int** matrix, int n, int m);
void sum(int** matrix_first, int** matrix_second, int** matrix_result, int n, int m);
void multiply(int** matrix_first, int** matrix_second, int** matrix_result, int n, int m, int common);
void transpose(int** matrix, int** matrix_result, int n, int m);
void free_matrix(int*** matrix, int n);
int run_sum(void);
int run_multiply(void);
int run_transpose(void);

int main(void) {
    int operation = 0;
    int status = 1;

    // 1 - сумма, 2 - умножение, 3 - транспонирование.
    if (scanf("%d", &operation) != 1 || operation < 1 || operation > 3) {
        status = 0;
    }

    if (status == 1) {
        if (operation == 1) {
            status = run_sum();
        } else if (operation == 2) {
            status = run_multiply();
        } else {
            status = run_transpose();
        }
    }

    // Любая ошибка на любом этапе выводится одинаково.
    if (status == 0) {
        printf("n/a");
    }
    return 0;
}

// Сложение двух матриц одинакового размера.
int run_sum(void) {
    int n_first = 0;
    int m_first = 0;
    int n_second = 0;
    int m_second = 0;
    int** matrix_first = NULL;
    int** matrix_second = NULL;
    int** matrix_result = NULL;
    int status = read_size(&n_first, &m_first);

    if (status == 1) {
        status = create_matrix(&matrix_first, n_first, m_first);
    }
    if (status == 1) {
        status = input(matrix_first, n_first, m_first);
    }
    if (status == 1) {
        status = read_size(&n_second, &m_second);
    }
    if (status == 1) {
        status = create_matrix(&matrix_second, n_second, m_second);
    }
    if (status == 1) {
        status = input(matrix_second, n_second, m_second);
    }
    if (status == 1 && (n_first != n_second || m_first != m_second)) {
        status = 0;
    }
    if (status == 1) {
        status = create_matrix(&matrix_result, n_first, m_first);
    }
    if (status == 1) {
        sum(matrix_first, matrix_second, matrix_result, n_first, m_first);
        output(matrix_result, n_first, m_first);
    }

    free_matrix(&matrix_result, n_first);
    free_matrix(&matrix_second, n_second);
    free_matrix(&matrix_first, n_first);
    return status;
}

// Умножение матриц: число столбцов первой равно числу строк второй.
int run_multiply(void) {
    int n_first = 0;
    int m_first = 0;
    int n_second = 0;
    int m_second = 0;
    int** matrix_first = NULL;
    int** matrix_second = NULL;
    int** matrix_result = NULL;
    int status = read_size(&n_first, &m_first);

    if (status == 1) {
        status = create_matrix(&matrix_first, n_first, m_first);
    }
    if (status == 1) {
        status = input(matrix_first, n_first, m_first);
    }
    if (status == 1) {
        status = read_size(&n_second, &m_second);
    }
    if (status == 1) {
        status = create_matrix(&matrix_second, n_second, m_second);
    }
    if (status == 1) {
        status = input(matrix_second, n_second, m_second);
    }
    if (status == 1 && m_first != n_second) {
        status = 0;
    }
    if (status == 1) {
        status = create_matrix(&matrix_result, n_first, m_second);
    }
    if (status == 1) {
        multiply(matrix_first, matrix_second, matrix_result, n_first, m_second, m_first);
        output(matrix_result, n_first, m_second);
    }

    free_matrix(&matrix_result, n_first);
    free_matrix(&matrix_second, n_second);
    free_matrix(&matrix_first, n_first);
    return status;
}

// Транспонирование одной матрицы.
int run_transpose(void) {
    int n = 0;
    int m = 0;
    int** matrix = NULL;
    int** matrix_result = NULL;
    int status = read_size(&n, &m);

    if (status == 1) {
        status = create_matrix(&matrix, n, m);
    }
    if (status == 1) {
        status = input(matrix, n, m);
    }
    if (status == 1) {
        status = create_matrix(&matrix_result, m, n);
    }
    if (status == 1) {
        transpose(matrix, matrix_result, n, m);
        output(matrix_result, m, n);
    }

    free_matrix(&matrix_result, m);
    free_matrix(&matrix, n);
    return status;
}

// Читаем два положительных числа: количество строк и столбцов.
int read_size(int* n, int* m) {
    int status = 0;

    if (scanf("%d %d", n, m) == 2 && *n > 0 && *m > 0) {
        status = 1;
    }
    return status;
}

// Выделяем массив указателей, а затем отдельную память под каждую строку.
int create_matrix(int*** matrix, int n, int m) {
    int status = 1;
    *matrix = NULL;

    if (sizes_are_safe(n, m) == 0) {
        status = 0;
    } else {
        *matrix = (int**)malloc((size_t)n * sizeof(int*));
        if (*matrix == NULL) {
            status = 0;
        } else {
            // Обнуляем указатели, чтобы при ошибке free(NULL) был безопасен.
            for (int i = 0; i < n; i++) {
                (*matrix)[i] = NULL;
            }
            for (int i = 0; i < n && status == 1; i++) {
                (*matrix)[i] = (int*)malloc((size_t)m * sizeof(int));
                if ((*matrix)[i] == NULL) {
                    status = 0;
                }
            }
            if (status == 0) {
                free_matrix(matrix, n);
            }
        }
    }
    return status;
}

// Проверка на переполнение при вычислении размера памяти.
int sizes_are_safe(int n, int m) {
    int safe = 0;

    if (n > 0 && m > 0 && (size_t)n <= SIZE_MAX / sizeof(int*) &&
        (size_t)n <= SIZE_MAX / sizeof(int) / (size_t)m) {
        safe = 1;
    }
    return safe;
}

// Ввод элементов матрицы.
int input(int** matrix, int n, int m) {
    int status = 1;

    for (int i = 0; i < n && status == 1; i++) {
        for (int j = 0; j < m && status == 1; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                status = 0;
            }
        }
    }
    return status;
}

// Вывод матрицы без лишних пробелов и без '\n' после последней строки.
void output(int** matrix, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (j > 0) {
                printf(" ");
            }
            printf("%d", matrix[i][j]);
        }
        if (i < n - 1) {
            printf("\n");
        }
    }
}

// Поэлементная сумма двух матриц.
void sum(int** matrix_first, int** matrix_second, int** matrix_result, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrix_result[i][j] = matrix_first[i][j] + matrix_second[i][j];
        }
    }
}

// Классическое умножение матриц.
void multiply(int** matrix_first, int** matrix_second, int** matrix_result, int n, int m, int common) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrix_result[i][j] = 0;
            for (int k = 0; k < common; k++) {
                matrix_result[i][j] += matrix_first[i][k] * matrix_second[k][j];
            }
        }
    }
}

// Меняем строки и столбцы местами.
void transpose(int** matrix, int** matrix_result, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrix_result[j][i] = matrix[i][j];
        }
    }
}

// Освобождаем каждую строку, а затем массив указателей.
void free_matrix(int*** matrix, int n) {
    if (*matrix != NULL) {
        for (int i = 0; i < n; i++) {
            free((*matrix)[i]);
        }
        free(*matrix);
        *matrix = NULL;
    }
}
