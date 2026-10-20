#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_STATIC_SIZE 100

int create_matrix(int*** matrix, int n, int m, int type, int static_data[MAX_STATIC_SIZE][MAX_STATIC_SIZE],
                  int* static_rows[MAX_STATIC_SIZE]);
int make_static_matrix(int*** matrix, int n, int m, int static_data[MAX_STATIC_SIZE][MAX_STATIC_SIZE],
                       int* static_rows[MAX_STATIC_SIZE]);
int make_separate_rows(int*** matrix, int n, int m);
int make_one_buffer(int*** matrix, int n, int m);
int make_one_block(int*** matrix, int n, int m);
int sizes_are_safe(int n, int m);
int input_matrix(int** matrix, int n, int m);
void output_matrix(int** matrix, int n, int m);
void free_matrix(int*** matrix, int n, int type);
int load_matrix(int*** matrix, int* type, int* n, int* m, int static_data[MAX_STATIC_SIZE][MAX_STATIC_SIZE],
                int* static_rows[MAX_STATIC_SIZE]);

int main(void) {
    int type = 0;
    int n = 0;
    int m = 0;
    int** matrix = NULL;

    // Для статического способа заранее держим память на стеке.
    int static_data[MAX_STATIC_SIZE][MAX_STATIC_SIZE];
    int* static_rows[MAX_STATIC_SIZE];

    int status = load_matrix(&matrix, &type, &n, &m, static_data, static_rows);

    // Выводим матрицу или сообщение об ошибке.
    if (status == 1) {
        output_matrix(matrix, n, m);
    } else {
        printf("n/a");
    }

    // Освобождаем память, если она была выделена динамически.
    free_matrix(&matrix, n, type);
    return 0;
}

// Читает способ выделения, размеры и элементы матрицы.
int load_matrix(int*** matrix, int* type, int* n, int* m, int static_data[MAX_STATIC_SIZE][MAX_STATIC_SIZE],
                int* static_rows[MAX_STATIC_SIZE]) {
    int status = 1;

    // Сначала выбираем способ выделения памяти (1-4).
    if (scanf("%d", type) != 1 || *type < 1 || *type > 4) {
        status = 0;
    }

    // Затем читаем количество строк и столбцов.
    if (status == 1 && (scanf("%d %d", n, m) != 2 || *n <= 0 || *m <= 0)) {
        status = 0;
    }

    if (status == 1 && create_matrix(matrix, *n, *m, *type, static_data, static_rows) == 0) {
        status = 0;
    }
    if (status == 1 && input_matrix(*matrix, *n, *m) == 0) {
        status = 0;
    }
    return status;
}

// Выбирает нужный способ создания матрицы по номеру из меню.
int create_matrix(int*** matrix, int n, int m, int type, int static_data[MAX_STATIC_SIZE][MAX_STATIC_SIZE],
                  int* static_rows[MAX_STATIC_SIZE]) {
    int status = 0;

    if (type == 1) {
        status = make_static_matrix(matrix, n, m, static_data, static_rows);
    } else if (type == 2) {
        status = make_separate_rows(matrix, n, m);
    } else if (type == 3) {
        status = make_one_buffer(matrix, n, m);
    } else if (type == 4) {
        status = make_one_block(matrix, n, m);
    }

    return status;
}

// Способ 1: статическая матрица, не больше 100 x 100.
int make_static_matrix(int*** matrix, int n, int m, int static_data[MAX_STATIC_SIZE][MAX_STATIC_SIZE],
                       int* static_rows[MAX_STATIC_SIZE]) {
    int status = 1;

    if (n <= 0 || m <= 0 || n > MAX_STATIC_SIZE || m > MAX_STATIC_SIZE) {
        status = 0;
    } else {
        // Указатели на строки смотрят прямо в статический массив.
        for (int i = 0; i < n; i++) {
            static_rows[i] = static_data[i];
        }
        *matrix = static_rows;
    }

    return status;
}

// Способ 2: массив указателей, каждая строка выделена отдельно.
int make_separate_rows(int*** matrix, int n, int m) {
    int status = 1;
    *matrix = NULL;

    if (sizes_are_safe(n, m) == 0) {
        status = 0;
    } else {
        *matrix = (int**)malloc((size_t)n * sizeof(int*));
        if (*matrix == NULL) {
            status = 0;
        } else {
            // Сначала обнуляем указатели, чтобы free() был безопасным.
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
                free_matrix(matrix, n, 2);
            }
        }
    }

    return status;
}

// Способ 3: массив указателей + один общий буфер для всех чисел.
int make_one_buffer(int*** matrix, int n, int m) {
    int status = 1;
    *matrix = NULL;

    if (sizes_are_safe(n, m) == 0) {
        status = 0;
    } else {
        *matrix = (int**)malloc((size_t)n * sizeof(int*));
        if (*matrix == NULL) {
            status = 0;
        } else {
            int* values = (int*)malloc((size_t)n * (size_t)m * sizeof(int));
            if (values == NULL) {
                free(*matrix);
                *matrix = NULL;
                status = 0;
            } else {
                // Строки идут подряд внутри одного буфера values.
                for (int i = 0; i < n; i++) {
                    (*matrix)[i] = values + (size_t)i * (size_t)m;
                }
            }
        }
    }

    return status;
}

// Способ 4: указатели и числа лежат в одном большом блоке памяти.
int make_one_block(int*** matrix, int n, int m) {
    int status = 1;
    *matrix = NULL;

    if (sizes_are_safe(n, m) == 0) {
        status = 0;
    } else {
        size_t pointers_size = (size_t)n * sizeof(int*);
        size_t values_size = (size_t)n * (size_t)m * sizeof(int);

        if (pointers_size > SIZE_MAX - values_size) {
            status = 0;
        } else {
            int** row_pointers = (int**)malloc(pointers_size + values_size);
            if (row_pointers == NULL) {
                status = 0;
            } else {
                // Числа начинаются сразу после массива указателей.
                int* values = (int*)(row_pointers + n);
                for (int i = 0; i < n; i++) {
                    row_pointers[i] = values + (size_t)i * (size_t)m;
                }
                *matrix = row_pointers;
            }
        }
    }

    return status;
}

// Проверяем, что размеры положительные и не вызовут переполнение.
int sizes_are_safe(int n, int m) {
    int safe = 0;

    if (n > 0 && m > 0 && (size_t)n <= SIZE_MAX / sizeof(int*) &&
        (size_t)n <= SIZE_MAX / sizeof(int) / (size_t)m) {
        safe = 1;
    }

    return safe;
}

// Читаем элементы матрицы. Функция не знает, как была выделена память.
int input_matrix(int** matrix, int n, int m) {
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

// Печатаем строки через '\n', а числа через пробел. В конце '\n' нет.
void output_matrix(int** matrix, int n, int m) {
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

// Освобождаем память в зависимости от способа выделения.
void free_matrix(int*** matrix, int n, int type) {
    if (*matrix != NULL) {
        if (type == 2) {
            for (int i = 0; i < n; i++) {
                free((*matrix)[i]);
            }
            free(*matrix);
        } else if (type == 3) {
            free((*matrix)[0]);
            free(*matrix);
        } else if (type == 4) {
            free(*matrix);
        }
        // Для способа 1 память статическая, ее освобождать не нужно.
        *matrix = NULL;
    }
}
