# Quest 6. Матричная арифметика

Файл программы: `src/matrix_arithmetic.c`.

## 1. Условие задания

Программа должна выполнять одну из трёх операций над целочисленными матрицами:

| Код | Операция |
|-----|----------|
| 1 | сложение двух матриц |
| 2 | умножение двух матриц |
| 3 | транспонирование одной матрицы |

Порядок ввода:

1. код операции;
2. размеры и элементы первой матрицы;
3. для операций 1 и 2 — размеры и элементы второй матрицы.

При любой ошибке нужно вывести `n/a`.

После последней строки результата не должно быть символа `\n`.

## 2. Примеры из задания

### Сложение

```text
Ввод:
1
2 2
4 3
9 0
2 2
1 1
2 2

Вывод:
5 4
11 2
```

Проверка:

```text
4 + 1 = 5     3 + 1 = 4
9 + 2 = 11    0 + 2 = 2
```

### Умножение

```text
Ввод:
2
2 3
4 3 1
9 0 2
3 1
1
2
3

Вывод:
13
15
```

Проверка:

```text
4*1 + 3*2 + 1*3 = 4 + 6 + 3 = 13
9*1 + 0*2 + 2*3 = 9 + 0 + 6 = 15
```

### Транспонирование

```text
Ввод:
3
2 2
4 3
9 0

Вывод:
4 9
3 0
```

Проверка: строки и столбцы поменялись местами.

## 3. Полный код

```c
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
```

## 4. Разбор `main`

```c
int operation = 0;
int status = 1;
```

- `operation` — код операции;
- `status` — признак успеха.

```c
if (scanf("%d", &operation) != 1 || operation < 1 || operation > 3) {
    status = 0;
}
```

Читаем код операции. Если это не число или число не `1`, `2`, `3`, ставим
ошибку.

```c
if (status == 1) {
    if (operation == 1) {
        status = run_sum();
    } else if (operation == 2) {
        status = run_multiply();
    } else {
        status = run_transpose();
    }
}
```

Запускаем нужную функцию.

- `run_sum` — сложение;
- `run_multiply` — умножение;
- `run_transpose` — транспонирование.

Каждая функция возвращает `1` при успехе и `0` при ошибке.

```c
if (status == 0) {
    printf("n/a");
}
return 0;
```

Если что-то пошло не так, печатаем `n/a`.

Если всё хорошо, функции `run_*` уже сами напечатали результат, поэтому
`main` больше ничего не печатает.

## 5. `read_size`

```c
int read_size(int* n, int* m) {
    int status = 0;

    if (scanf("%d %d", n, m) == 2 && *n > 0 && *m > 0) {
        status = 1;
    }
    return status;
}
```

- Читает два числа.
- `scanf` должен вернуть `2`.
- Оба числа должны быть положительными.
- Если всё хорошо, возвращает `1`, иначе `0`.

## 6. `create_matrix`

```c
int create_matrix(int*** matrix, int n, int m) {
    int status = 1;
    *matrix = NULL;
```

Сначала указатель на матрицу равен `NULL`.

```c
    if (sizes_are_safe(n, m) == 0) {
        status = 0;
    }
```

Проверяем размеры на переполнение.

```c
    } else {
        *matrix = (int**)malloc((size_t)n * sizeof(int*));
```

Выделяем массив из `n` указателей.

```c
        if (*matrix == NULL) {
            status = 0;
        } else {
            for (int i = 0; i < n; i++) {
                (*matrix)[i] = NULL;
            }
```

Обнуляем все указатели. Это нужно, чтобы при ошибке можно было безопасно
вызвать `free` для каждой строки.

```c
            for (int i = 0; i < n && status == 1; i++) {
                (*matrix)[i] = (int*)malloc((size_t)m * sizeof(int));
                if ((*matrix)[i] == NULL) {
                    status = 0;
                }
            }
```

Выделяем отдельную память под каждую строку.

Если `malloc` для какой-то строки не сработал:

```c
            if (status == 0) {
                free_matrix(matrix, n);
            }
```

Освобождаем всё, что уже было выделено. `free_matrix` пройдёт по всем `n`
указателям; для ещё не выделенных строк там `NULL`, а `free(NULL)` безопасен.

## 7. `sizes_are_safe`

```c
if (n > 0 && m > 0 && (size_t)n <= SIZE_MAX / sizeof(int*) &&
    (size_t)n <= SIZE_MAX / sizeof(int) / (size_t)m) {
    safe = 1;
}
```

Проверяет:

- размеры положительные;
- `n * sizeof(int*)` не переполнит `size_t`;
- `n * m * sizeof(int)` не переполнит `size_t`.

`SIZE_MAX` берётся из `<stdint.h>`.

## 8. `run_sum`: сложение

```c
int run_sum(void) {
    int n_first = 0;
    int m_first = 0;
    int n_second = 0;
    int m_second = 0;
    int** matrix_first = NULL;
    int** matrix_second = NULL;
    int** matrix_result = NULL;
    int status = read_size(&n_first, &m_first);
```

Сначала читаем размеры первой матрицы.

```c
    if (status == 1) {
        status = create_matrix(&matrix_first, n_first, m_first);
    }
    if (status == 1) {
        status = input(matrix_first, n_first, m_first);
    }
```

Создаём и заполняем первую матрицу.

```c
    if (status == 1) {
        status = read_size(&n_second, &m_second);
    }
    if (status == 1) {
        status = create_matrix(&matrix_second, n_second, m_second);
    }
    if (status == 1) {
        status = input(matrix_second, n_second, m_second);
    }
```

То же самое для второй матрицы.

```c
    if (status == 1 && (n_first != n_second || m_first != m_second)) {
        status = 0;
    }
```

Для сложения размеры должны совпадать. Если нет — ошибка.

```c
    if (status == 1) {
        status = create_matrix(&matrix_result, n_first, m_first);
    }
    if (status == 1) {
        sum(matrix_first, matrix_second, matrix_result, n_first, m_first);
        output(matrix_result, n_first, m_first);
    }
```

Создаём результат того же размера, складываем и печатаем.

```c
    free_matrix(&matrix_result, n_first);
    free_matrix(&matrix_second, n_second);
    free_matrix(&matrix_first, n_first);
    return status;
}
```

Освобождаем все три матрицы. Даже если какая-то из них не была создана,
`free_matrix` увидит `NULL` и ничего не сделает.

### Формула сложения

```text
result[i][j] = first[i][j] + second[i][j]
```

Пример:

```text
4 3     1 1     5 4
9 0  +  2 2  =  11 2
```

## 9. `run_multiply`: умножение

Код почти такой же, но есть важное отличие в проверке размеров:

```c
    if (status == 1 && m_first != n_second) {
        status = 0;
    }
```

Для умножения нужно, чтобы число столбцов первой матрицы было равно числу
строк второй.

Размер результата:

```c
    status = create_matrix(&matrix_result, n_first, m_second);
```

То есть:

```text
(n_first x m_first) * (n_second x m_second)
```

при условии `m_first == n_second` даёт:

```text
n_first x m_second
```

### Формула умножения

```text
result[i][j] = sum(first[i][k] * second[k][j]) по k
```

Три цикла:

```c
for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
        matrix_result[i][j] = 0;
        for (int k = 0; k < common; k++) {
            matrix_result[i][j] += matrix_first[i][k] * matrix_second[k][j];
        }
    }
}
```

- `i` — строка результата;
- `j` — столбец результата;
- `k` — общий индекс.

Перед суммированием результат обнуляется: `matrix_result[i][j] = 0;`.

Пример:

```text
A = 4 3 1     B = 1
    9 0 2         2
                  3
```

Результат:

```text
C[0][0] = 4*1 + 3*2 + 1*3 = 13
C[1][0] = 9*1 + 0*2 + 2*3 = 15
```

## 10. `run_transpose`: транспонирование

```c
int run_transpose(void) {
    int n = 0;
    int m = 0;
    int** matrix = NULL;
    int** matrix_result = NULL;
    int status = read_size(&n, &m);
```

Читаем размеры исходной матрицы.

```c
    if (status == 1) {
        status = create_matrix(&matrix, n, m);
    }
    if (status == 1) {
        status = input(matrix, n, m);
    }
```

Создаём и заполняем матрицу.

```c
    if (status == 1) {
        status = create_matrix(&matrix_result, m, n);
    }
```

Результат имеет размер `m x n`: строки и столбцы меняются местами.

```c
    if (status == 1) {
        transpose(matrix, matrix_result, n, m);
        output(matrix_result, m, n);
    }
```

Транспонируем и печатаем.

```c
    free_matrix(&matrix_result, m);
    free_matrix(&matrix, n);
```

Освобождаем память. У результата `m` строк, у исходной матрицы `n` строк.

### Формула транспонирования

```text
result[j][i] = matrix[i][j]
```

Пример:

```text
4 3     4 9
9 0  -> 3 0
```

## 11. Почему `free_matrix` принимает `int***`

```c
void free_matrix(int*** matrix, int n) {
    if (*matrix != NULL) {
        for (int i = 0; i < n; i++) {
            free((*matrix)[i]);
        }
        free(*matrix);
        *matrix = NULL;
    }
}
```

- `matrix` — адрес указателя на матрицу;
- `*matrix` — сама матрица;
- сначала освобождаются строки;
- потом массив указателей;
- в конце указатель обнуляется.

Если передать просто `int**`, функция не сможет записать `NULL` в указатель
из `main`.

## 12. Ошибки и `n/a`

Программа выводит `n/a`, если:

- код операции не число;
- код операции не `1`, `2` или `3`;
- размеры не числа;
- размеры не положительные;
- не удалось выделить память;
- не удалось прочитать элемент матрицы;
- для сложения размеры матриц не совпадают;
- для умножения `m_first != n_second`.

Во всех случаях освобождается всё, что было выделено.

## 13. Компиляция и проверка

```sh
gcc -std=c11 -Wall -Wextra -Werror -pedantic src/matrix_arithmetic.c -o matrix_arithmetic
```

Сложение:

```sh
printf '1\n2 2\n4 3\n9 0\n2 2\n1 1\n2 2' | ./matrix_arithmetic
```

Ожидаемый вывод:

```text
5 4
11 2
```

Умножение:

```sh
printf '2\n2 3\n4 3 1\n9 0 2\n3 1\n1\n2\n3' | ./matrix_arithmetic
```

Ожидаемый вывод:

```text
13
15
```

Транспонирование:

```sh
printf '3\n2 2\n4 3\n9 0' | ./matrix_arithmetic
```

Ожидаемый вывод:

```text
4 9
3 0
```

Ошибка:

```sh
printf '4' | ./matrix_arithmetic
```

Ожидаемый вывод:

```text
n/a
```

## 14. Частые ошибки

1. **Перепутать условие умножения.** Нужно `m_first == n_second`, а не
   `n_first == m_second`.
2. **Неверный размер результата умножения.** Нужно `n_first x m_second`.
3. **Забыть обнулить `matrix_result[i][j]`.** Тогда к результату прибавится
   старое значение.
4. **Перепутать индексы в транспонировании.** Нужно
   `result[j][i] = matrix[i][j]`.
5. **Не проверить размеры для сложения.** Матрицы должны быть одинаковыми.
6. **Не освободить промежуточные матрицы при ошибке.** Утечка.
7. **Печатать `\n` после последней строки.** Автотест не примет.
8. **Печатать пробел после последнего числа в строке.** Лишний символ.
9. **Использовать `free_matrix` с неправильным числом строк.** Для результата
   умножения число строк равно `n_first`, для транспонирования — `m`.

## 15. Чек-лист

- [x] Код операции читается первым.
- [x] Сложение проверяет одинаковые размеры.
- [x] Умножение проверяет `m_first == n_second`.
- [x] Транспонирование меняет размеры на `m x n`.
- [x] Результат умножения имеет размер `n_first x m_second`.
- [x] Все матрицы создаются динамически.
- [x] Все `malloc` проверяются.
- [x] Вся память освобождается.
- [x] При ошибке печатается `n/a`.
- [x] После последней строки нет `\n`.
