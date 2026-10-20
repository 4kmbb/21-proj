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
void output_array(const int* a, int n);
void find_row_max(int** matrix, int n, int m, int* row_max);
void find_col_min(int** matrix, int n, int m, int* col_min);
void free_matrix(int*** matrix, int n, int type);
int load_matrix(int*** matrix, int* type, int* n, int* m, int static_data[MAX_STATIC_SIZE][MAX_STATIC_SIZE],
                int* static_rows[MAX_STATIC_SIZE]);
int allocate_results(int** row_max, int** col_min, int n, int m);
void print_result(int** matrix, int n, int m, int* row_max, int* col_min);

int main(void) {
    int type = 0;
    int n = 0;
    int m = 0;
    int** matrix = NULL;
    int* row_max = NULL;
    int* col_min = NULL;
    int static_data[MAX_STATIC_SIZE][MAX_STATIC_SIZE];
    int* static_rows[MAX_STATIC_SIZE];
    int status = load_matrix(&matrix, &type, &n, &m, static_data, static_rows);

    // Массивы для максимумов строк и минимумов столбцов.
    if (status == 1) {
        status = allocate_results(&row_max, &col_min, n, m);
    }

    if (status == 1) {
        print_result(matrix, n, m, row_max, col_min);
    } else {
        printf("n/a");
    }

    free(row_max);
    free(col_min);
    free_matrix(&matrix, n, type);
    return 0;
}

// Читает способ выделения, размеры, создает и заполняет матрицу.
int load_matrix(int*** matrix, int* type, int* n, int* m, int static_data[MAX_STATIC_SIZE][MAX_STATIC_SIZE],
                int* static_rows[MAX_STATIC_SIZE]) {
    int status = 1;

    // Способ выделения памяти: 1-4.
    if (scanf("%d", type) != 1 || *type < 1 || *type > 4) {
        status = 0;
    }

    // Размер матрицы: сначала строки, потом столбцы.
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

// Выделяет память под массивы максимумов и минимумов.
int allocate_results(int** row_max, int** col_min, int n, int m) {
    int status = 1;

    *row_max = (int*)malloc((size_t)n * sizeof(int));
    if (*row_max == NULL) {
        status = 0;
    }
    if (status == 1) {
        *col_min = (int*)malloc((size_t)m * sizeof(int));
        if (*col_min == NULL) {
            status = 0;
        }
    }
    return status;
}

// Выводит матрицу, максимумы строк и минимумы столбцов.
void print_result(int** matrix, int n, int m, int* row_max, int* col_min) {
    find_row_max(matrix, n, m, row_max);
    find_col_min(matrix, n, m, col_min);

    output_matrix(matrix, n, m);
    printf("\n");
    output_array(row_max, n);
    printf("\n");
    output_array(col_min, m);
}

// Создает матрицу выбранным способом.
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

// Способ 1: статическая матрица до 100 x 100.
int make_static_matrix(int*** matrix, int n, int m, int static_data[MAX_STATIC_SIZE][MAX_STATIC_SIZE],
                       int* static_rows[MAX_STATIC_SIZE]) {
    int status = 1;

    if (n <= 0 || m <= 0 || n > MAX_STATIC_SIZE || m > MAX_STATIC_SIZE) {
        status = 0;
    } else {
        for (int i = 0; i < n; i++) {
            static_rows[i] = static_data[i];
        }
        *matrix = static_rows;
    }

    return status;
}

// Способ 2: каждая строка выделена отдельным malloc.
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

// Способ 3: массив указателей и один общий буфер с числами.
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
                for (int i = 0; i < n; i++) {
                    (*matrix)[i] = values + (size_t)i * (size_t)m;
                }
            }
        }
    }

    return status;
}

// Способ 4: указатели и числа в одном блоке памяти.
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

// Проверка размеров перед динамическим выделением памяти.
int sizes_are_safe(int n, int m) {
    int safe = 0;

    if (n > 0 && m > 0 && (size_t)n <= SIZE_MAX / sizeof(int*) &&
        (size_t)n <= SIZE_MAX / sizeof(int) / (size_t)m) {
        safe = 1;
    }

    return safe;
}

// Ввод матрицы работает с int ** независимо от способа выделения.
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

// Вывод матрицы без лишних пробелов и без '\n' после последней строки.
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

// Вывод одномерного массива через пробел.
void output_array(const int* a, int n) {
    for (int i = 0; i < n; i++) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", a[i]);
    }
}

// Максимум в каждой строке.
void find_row_max(int** matrix, int n, int m, int* row_max) {
    for (int i = 0; i < n; i++) {
        int maximum = matrix[i][0];
        for (int j = 1; j < m; j++) {
            if (matrix[i][j] > maximum) {
                maximum = matrix[i][j];
            }
        }
        row_max[i] = maximum;
    }
}

// Минимум в каждом столбце.
void find_col_min(int** matrix, int n, int m, int* col_min) {
    for (int j = 0; j < m; j++) {
        int minimum = matrix[0][j];
        for (int i = 1; i < n; i++) {
            if (matrix[i][j] < minimum) {
                minimum = matrix[i][j];
            }
        }
        col_min[j] = minimum;
    }
}

// Освобождение памяти по типу выделения.
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
        *matrix = NULL;
    }
}
