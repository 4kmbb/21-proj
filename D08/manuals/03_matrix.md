# Quest 3. Матрица и четыре способа выделения памяти

Файл программы: `src/matrix.c`.

## 1. Условие задания

Нужно написать программу, которая:

1. читает номер способа выделения памяти (`1`–`4`);
2. читает размеры матрицы: сначала количество строк `n`, потом количество
   столбцов `m`;
3. читает `n * m` целых чисел;
4. выводит матрицу;
5. при любой ошибке выводит `n/a`;
6. не оставляет лишних пробелов в конце строк;
7. не выводит `\n` после последней строки;
8. освобождает всю динамически выделенную память.

Способы выделения:

| Номер | Способ |
|------|--------|
| 1 | статический массив `100 x 100` |
| 2 | массив указателей + отдельный `malloc` на каждую строку |
| 3 | массив указателей + один общий буфер для чисел |
| 4 | один блок памяти: сначала указатели, потом числа |

Пример:

```text
Ввод:
2
2 2
4 3
9 0

Вывод:
4 3
9 0
```

После `9 0` символа `\n` нет.

## 2. Главная идея: матрица как `int**`

В C нет «настоящей» динамической двумерной матрицы. Обычно её делают так:

```c
int** matrix;
```

`matrix` — это указатель на массив указателей. Каждый элемент `matrix[i]` —
это указатель на первую ячейку `i`-й строки.

Схема:

```text
matrix
  |
  v
+------+      +---+---+---+
| ptr0 | ---> | 4 | 3 | 1 |
+------+      +---+---+---+
| ptr1 | ---> | 9 | 0 | 5 |
+------+      +---+---+---+
| ptr2 | ---> | 7 | 8 | 2 |
+------+      +---+---+---+
```

Тогда:

- `matrix[i]` — адрес начала строки `i`;
- `matrix[i][j]` — элемент в строке `i` и столбце `j`;
- `matrix[i][j]` можно записать как `*(*(matrix + i) + j)`.

Все четыре способа отличаются только тем, как именно устроены эти указатели
и где лежат числа. Функции ввода и вывода работают с `int**` и не знают,
какой способ был выбран.

## 3. Почему в функциях встречается `int***`

В `main` есть переменная:

```c
int** matrix = NULL;
```

Функция `create_matrix` должна изменить именно эту переменную, то есть
записать в неё адрес новой матрицы. В C параметры передаются по значению.
Если передать `int**`, функция получит копию указателя и не сможет изменить
оригинал.

Поэтому передают адрес указателя:

```c
create_matrix(&matrix, ...);
```

Тогда параметр имеет тип `int***` — «указатель на указатель на указатель».
Внутри функции:

```c
*matrix = ...;
```

`*matrix` — это та самая переменная `matrix` из `main`.

## 4. Полный код

```c
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
```

## 5. Разбор `main`

```c
int type = 0;
int n = 0;
int m = 0;
int** matrix = NULL;
```

- `type` — номер способа выделения памяти;
- `n` — количество строк;
- `m` — количество столбцов;
- `matrix` — указатель на матрицу, пока `NULL`.

```c
int static_data[MAX_STATIC_SIZE][MAX_STATIC_SIZE];
int* static_rows[MAX_STATIC_SIZE];
```

Это память для статического способа.

- `static_data` — обычный двумерный массив `100 x 100`.
- `static_rows` — массив из 100 указателей. Каждый указатель будет смотреть
  на строку `static_data`.

Память под эти массивы выделяется на стеке автоматически. `free` для них
вызывать нельзя.

```c
int status = load_matrix(&matrix, &type, &n, &m, static_data, static_rows);
```

`load_matrix`:

- читает `type`, `n`, `m`;
- создаёт матрицу нужным способом;
- заполняет её числами.

Возвращает `1` при успехе и `0` при ошибке.

```c
if (status == 1) {
    output_matrix(matrix, n, m);
} else {
    printf("n/a");
}
```

Если всё хорошо, печатаем матрицу. Иначе печатаем `n/a`.

```c
free_matrix(&matrix, n, type);
```

Освобождаем память. Для статического способа `free_matrix` ничего не делает.
Для динамических — освобождает в зависимости от `type`.

## 6. Разбор `load_matrix`

```c
int status = 1;
```

Считаем, что пока всё хорошо.

```c
if (scanf("%d", type) != 1 || *type < 1 || *type > 4) {
    status = 0;
}
```

- Читаем номер способа.
- Если это не число или число вне диапазона `1..4`, ставим ошибку.

```c
if (status == 1 && (scanf("%d %d", n, m) != 2 || *n <= 0 || *m <= 0)) {
    status = 0;
}
```

- Читаем `n` и `m`.
- `scanf` должен вернуть `2`.
- Размеры должны быть положительными.
- Проверка `status == 1` нужна, чтобы не читать размеры, если способ уже
  оказался неверным.

```c
if (status == 1 && create_matrix(matrix, *n, *m, *type, static_data, static_rows) == 0) {
    status = 0;
}
if (status == 1 && input_matrix(*matrix, *n, *m) == 0) {
    status = 0;
}
```

- Создаём матрицу.
- Если создать не удалось, ставим ошибку.
- Если матрица создана, читаем её элементы.
- `*matrix` — указатель на матрицу, который лежит в `main`.

## 7. Способ 1: статическая матрица

```c
int static_data[MAX_STATIC_SIZE][MAX_STATIC_SIZE];
int* static_rows[MAX_STATIC_SIZE];
```

`static_data` — это непрерывный блок памяти:

```text
static_data:
+------+------+------+ ...
| [0][0] [0][1] [0][2] ...
+------+------+------+ ...
| [1][0] [1][1] [1][2] ...
+------+------+------+ ...
```

В C имя `static_data[i]` автоматически превращается в указатель на первый
элемент строки `i`, то есть на `static_data[i][0]`.

```c
for (int i = 0; i < n; i++) {
    static_rows[i] = static_data[i];
}
*matrix = static_rows;
```

После этого:

```text
static_rows[0] -> static_data[0] -> [0][0]
static_rows[1] -> static_data[1] -> [1][0]
...
matrix -> static_rows
```

Функции ввода и вывода видят обычную `int**`-матрицу и работают с ней.

Ограничение: `n <= 100`, `m <= 100`. Если размер больше, способ 1 вернёт
ошибку.

Память освобождать не нужно: `static_data` и `static_rows` лежат на стеке
и исчезнут сами при выходе из `main`.

## 8. Способ 2: отдельный `malloc` на каждую строку

```c
*matrix = (int**)malloc((size_t)n * sizeof(int*));
```

Сначала выделяем массив из `n` указателей.

```text
matrix -> [ ptr0 ][ ptr1 ][ ptr2 ] ...
```

Затем каждый указатель должен получить свою строку:

```c
for (int i = 0; i < n; i++) {
    (*matrix)[i] = NULL;
}
for (int i = 0; i < n && status == 1; i++) {
    (*matrix)[i] = (int*)malloc((size_t)m * sizeof(int));
    if ((*matrix)[i] == NULL) {
        status = 0;
    }
}
```

Первый цикл обнуляет все указатели. Это важно: если `malloc` для одной из
строк не сработает, остальные указатели будут `NULL`, и `free(NULL)` можно
вызывать безопасно.

После успешного выделения:

```text
matrix -> [ ptr0 ][ ptr1 ][ ptr2 ]
             |       |       |
             v       v       v
          [m чисел] [m чисел] [m чисел]
```

В памяти это выглядит так:

```text
отдельный блок указателей
+------+------+------+
| ptr0 | ptr1 | ptr2 |
+------+------+------+
   |      |      |
   v      v      v
+---+  +---+  +---+
|...|  |...|  |...|
+---+  +---+  +---+
```

Плюс: строки можно выделять и освобождать по отдельности.

Минус: много вызовов `malloc` и `free`, память может быть разбросана по куче.

Освобождение:

```c
for (int i = 0; i < n; i++) {
    free((*matrix)[i]);
}
free(*matrix);
```

Сначала освобождаем каждую строку, потом массив указателей.

## 9. Способ 3: указатели + один общий буфер

```c
*matrix = (int**)malloc((size_t)n * sizeof(int*));
```

Выделяем массив указателей.

```c
int* values = (int*)malloc((size_t)n * (size_t)m * sizeof(int));
```

Выделяем один непрерывный блок под все `n * m` чисел.

```c
for (int i = 0; i < n; i++) {
    (*matrix)[i] = values + (size_t)i * (size_t)m;
}
```

Настраиваем указатели:

```text
values -> [0][0] [0][1] ... [0][m-1] [1][0] ... [n-1][m-1]

matrix[0] = values + 0 * m
matrix[1] = values + 1 * m
matrix[2] = values + 2 * m
```

Схема:

```text
matrix -> [ ptr0 ][ ptr1 ][ ptr2 ]
             |       |       |
             v       v       v
values ---> +-------+-------+-------+
            | строка0| строка1| строка2|
            +-------+-------+-------+
```

Все числа лежат рядом. Это лучше для кэша процессора, чем способ 2.

Освобождение:

```c
free((*matrix)[0]);  // это тот же адрес, что values
free(*matrix);
```

Почему `(*matrix)[0]`? Потому что `matrix[0]` указывает на начало `values`.
Можно было бы хранить `values` отдельно, но тогда пришлось бы передавать его
в `free_matrix`. Мы этого не делаем: `matrix[0]` и есть `values`.

## 10. Способ 4: всё в одном блоке

```c
size_t pointers_size = (size_t)n * sizeof(int*);
size_t values_size = (size_t)n * (size_t)m * sizeof(int);
```

Считаем, сколько байт нужно под указатели и сколько под числа.

```c
if (pointers_size > SIZE_MAX - values_size) {
    status = 0;
}
```

Проверяем, что сумма не переполнит `size_t`.

```c
int** row_pointers = (int**)malloc(pointers_size + values_size);
```

Выделяем один блок, в котором сначала идут указатели, а потом числа.

```c
int* values = (int*)(row_pointers + n);
```

`row_pointers + n` — адрес сразу после `n` указателей. Приводим его к `int*`.

```c
for (int i = 0; i < n; i++) {
    row_pointers[i] = values + (size_t)i * (size_t)m;
}
*matrix = row_pointers;
```

Схема:

```text
один блок malloc:
+--------+--------+--------+-------+-------+-------+-----+
| ptr0   | ptr1   | ptr2   | число | число | число | ... |
+--------+--------+--------+-------+-------+-------+-----+
   |        |        |
   |        |        +----> значения строки 2
   |        +-------------> значения строки 1
   +----------------------> значения строки 0
```

Плюс: всего один `malloc` и один `free`.

Минус: код сложнее, нужно вручную считать смещения.

Освобождение:

```c
free(*matrix);
```

Достаточно одного `free`, потому что весь блок был выделен одним `malloc`.

## 11. Почему `free_matrix` принимает `type`

Разные способы требуют разного освобождения:

| `type` | Что освобождать |
|-------|------------------|
| 1 | ничего, память статическая |
| 2 | каждую строку, затем массив указателей |
| 3 | общий буфер чисел, затем массив указателей |
| 4 | один общий блок |

Поэтому в `free_matrix` есть ветвления по `type`.

```c
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
```

После освобождения:

```c
*matrix = NULL;
```

Это защищает от повторного `free`.

## 12. `sizes_are_safe`

```c
if (n > 0 && m > 0 && (size_t)n <= SIZE_MAX / sizeof(int*) &&
    (size_t)n <= SIZE_MAX / sizeof(int) / (size_t)m) {
    safe = 1;
}
```

Зачем это нужно?

При умножении `n * m * sizeof(int)` может произойти переполнение `size_t`.
Например, если `n` и `m` огромные, произведение может стать маленьким из-за
переполнения, и `malloc` выделит меньше памяти, чем нужно. Тогда программа
выйдет за границы массива.

Проверка:

- `n > 0`, `m > 0` — размеры положительные;
- `n <= SIZE_MAX / sizeof(int*)` — массив указателей помещается в `size_t`;
- `n <= SIZE_MAX / sizeof(int) / m` — `n * m * sizeof(int)` не переполнится.

`SIZE_MAX` — максимальное значение `size_t`. Он объявлен в `<stdint.h>`.

## 13. Ввод и вывод

```c
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
```

- Два вложенных цикла обходят все элементы.
- `&matrix[i][j]` — адрес элемента.
- Если `scanf` не смог прочитать число, ставим ошибку и выходим из циклов.

```c
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
```

- Перед каждым элементом, кроме первого в строке, печатается пробел.
- После каждой строки, кроме последней, печатается `\n`.
- После последней строки `\n` нет.

## 14. Ошибки и `n/a`

Программа выводит `n/a`, если:

- номер способа не число;
- номер способа не в диапазоне `1..4`;
- размеры не числа;
- `n <= 0` или `m <= 0`;
- для способа 1 размер больше `100 x 100`;
- `malloc` не смог выделить память;
- хотя бы один элемент матрицы не число;
- размеры вызывают переполнение.

При ошибке `free_matrix` всё равно вызывается в `main`, но если матрица
не создана, `*matrix == NULL`, и функция ничего не делает.

## 15. Проверка всех четырёх способов

```sh
gcc -std=c11 -Wall -Wextra -Werror -pedantic src/matrix.c -o matrix

for t in 1 2 3 4; do
    echo "Способ $t"
    printf "$t\n2 2\n4 3\n9 0" | ./matrix
    echo
done
```

Ожидаемый вывод для каждого способа:

```text
4 3
9 0
```

## 16. Частые ошибки

1. **Путать `int**` и `int***`.** В `main` матрица — `int**`, в функцию
   создания передаётся `&matrix`, поэтому параметр — `int***`.
2. **Забыть `free` для способа 2.** Нужно освободить каждую строку.
3. **Для способа 3 освобождать каждую строку.** Нельзя: строки не выделялись
   отдельно, это части одного буфера.
4. **Для способа 4 вызывать `free` для каждой строки.** Нельзя: всё выделено
   одним блоком.
5. **Для способа 1 вызывать `free`.** Нельзя: память на стеке.
6. **Не проверять `malloc`.** Если памяти нет, будет `NULL` и падение.
7. **Не проверять `scanf`.** В матрице окажется мусор.
8. **Печатать `\n` после последней строки.** Автотест не примет.
9. **Печатать пробел после последнего числа в строке.** Тоже лишний символ.
10. **Не освобождать память при ошибке ввода.** Утечка.

## 17. Чек-лист

- [x] Меню способов `1–4`.
- [x] Статический способ ограничен `100 x 100`.
- [x] Три динамических способа реализованы.
- [x] Ввод и вывод не зависят от способа.
- [x] Все `malloc` проверяются.
- [x] Все динамические блоки освобождаются.
- [x] Нет лишних пробелов в конце строк.
- [x] Нет `\n` после последней строки.
- [x] При ошибке печатается `n/a`.
