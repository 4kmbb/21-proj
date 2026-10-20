# Quest 4. Максимумы строк и минимумы столбцов

Файл программы: `src/matrix_extended.c`.

## 1. Условие задания

Нужно расширить программу из Quest 3. Она должна:

1. так же принимать номер способа выделения памяти `1–4`;
2. принимать размеры матрицы `n` и `m`;
3. принимать саму матрицу;
4. выводить матрицу;
5. дополнительно выводить:
   - массив максимумов по каждой строке;
   - массив минимумов по каждому столбцу;
6. при любой ошибке выводить `n/a`;
7. не выводить `\n` после последнего числа.

Пример:

```text
Ввод:
2
3 3
4 3 1
9 0 55
-4 7 111

Вывод:
4 3 1
9 0 55
-4 7 111
4 55 111
-4 0 1
```

Разберём вывод:

- первые три строки — сама матрица;
- `4 55 111` — максимумы строк:
  - строка `4 3 1` → максимум `4`;
  - строка `9 0 55` → максимум `55`;
  - строка `-4 7 111` → максимум `111`;
- `-4 0 1` — минимумы столбцов:
  - столбец `4, 9, -4` → минимум `-4`;
  - столбец `3, 0, 7` → минимум `0`;
  - столбец `1, 55, 111` → минимум `1`.

После последнего числа `1` перевода строки нет.

## 2. Что общего с `matrix.c`

Почти весь код совпадает с Quest 3:

- те же четыре способа выделения памяти;
- те же функции `create_matrix`, `make_*`, `sizes_are_safe`;
- тот же ввод и вывод матрицы;
- то же освобождение памяти.

Подробный разбор этих частей есть в файле
[03_matrix.md](03_matrix.md). Здесь мы подробно разберём новые функции и
полный порядок работы программы.

## 3. Новые функции

В `matrix_extended.c` добавлены:

```c
void output_array(const int* a, int n);
void find_row_max(int** matrix, int n, int m, int* row_max);
void find_col_min(int** matrix, int n, int m, int* col_min);
int allocate_results(int** row_max, int** col_min, int n, int m);
void print_result(int** matrix, int n, int m, int* row_max, int* col_min);
```

Их назначение:

| Функция | Что делает |
|--------|------------|
| `output_array` | печатает одномерный массив через пробел |
| `find_row_max` | заполняет массив максимумов строк |
| `find_col_min` | заполняет массив минимумов столбцов |
| `allocate_results` | выделяет память под два результатных массива |
| `print_result` | вызывает поиск и печатает всё в нужном порядке |

## 4. Полный код новых частей

Ниже приведён полный код `main` и новых функций. Код создания матрицы
совпадает с `matrix.c`.

```c
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
```

## 5. Разбор `main`

```c
int* row_max = NULL;
int* col_min = NULL;
```

Это будущие массивы результатов.

- `row_max` будет хранить `n` чисел: по одному максимуму на каждую строку.
- `col_min` будет хранить `m` чисел: по одному минимуму на каждый столбец.

Пока указатели равны `NULL`, потому что память ещё не выделена.

```c
int status = load_matrix(&matrix, &type, &n, &m, static_data, static_rows);
```

Читаем способ, размеры, создаём и заполняем матрицу.

```c
if (status == 1) {
    status = allocate_results(&row_max, &col_min, n, m);
}
```

Если матрица успешно создана, выделяем память под результаты.

Почему только после успешного создания матрицы? Потому что до этого мы не
знаем размеры `n` и `m`. Если матрица не создалась, результаты не нужны.

```c
if (status == 1) {
    print_result(matrix, n, m, row_max, col_min);
} else {
    printf("n/a");
}
```

- Если всё хорошо, считаем максимумы и минимумы и печатаем результат.
- Иначе печатаем `n/a`.

```c
free(row_max);
free(col_min);
free_matrix(&matrix, n, type);
return 0;
```

Освобождаем всё, что могло быть выделено.

- `free(NULL)` безопасен, поэтому `free(row_max)` можно вызывать, даже если
  память не выделялась.
- `free_matrix` сама проверяет, есть ли матрица.

Порядок освобождения не важен, потому что блоки памяти независимы.

## 6. Разбор `allocate_results`

```c
int allocate_results(int** row_max, int** col_min, int n, int m) {
    int status = 1;
```

Функция получает адреса указателей `row_max` и `col_min`, чтобы записать в них
адреса новых массивов.

```c
    *row_max = (int*)malloc((size_t)n * sizeof(int));
    if (*row_max == NULL) {
        status = 0;
    }
```

Выделяем `n` целых чисел под максимумы строк.

- `(size_t)n` — приводим к беззнаковому типу;
- `sizeof(int)` — размер одного числа;
- если `malloc` вернул `NULL`, ставим ошибку.

```c
    if (status == 1) {
        *col_min = (int*)malloc((size_t)m * sizeof(int));
        if (*col_min == NULL) {
            status = 0;
        }
    }
```

Если первый `malloc` прошёл успешно, выделяем `m` чисел под минимумы столбцов.

Если второй `malloc` не сработал, `status` станет `0`. При этом `row_max`
уже выделен. В `main` будет вызван `free(row_max)`, поэтому утечки не будет.

## 7. Разбор `find_row_max`

```c
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
```

Задача: для каждой строки найти максимальный элемент.

Алгоритм:

1. Внешний цикл идёт по строкам `i = 0..n-1`.
2. В начале считаем максимумом первый элемент строки: `matrix[i][0]`.
3. Внутренний цикл идёт по остальным столбцам `j = 1..m-1`.
4. Если текущий элемент больше `maximum`, обновляем максимум.
5. После внутреннего цикла записываем результат в `row_max[i]`.

Почему начинаем с `matrix[i][0]`, а не с нуля?

Потому что числа могут быть отрицательными. Если начать с `0`, то для строки

```text
-5 -3 -7
```

максимум останется `0`, хотя это неправильно. Правильный максимум — `-3`.

Пример:

```text
Строка: 4 3 1 9 2
maximum = 4
j=1: 3 > 4? нет
j=2: 1 > 4? нет
j=3: 9 > 4? да, maximum = 9
j=4: 2 > 9? нет
row_max[i] = 9
```

## 8. Разбор `find_col_min`

```c
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
```

Задача: для каждого столбца найти минимальный элемент.

Алгоритм:

1. Внешний цикл идёт по столбцам `j = 0..m-1`.
2. В начале считаем минимумом первый элемент столбца: `matrix[0][j]`.
3. Внутренний цикл идёт по остальным строкам `i = 1..n-1`.
4. Если текущий элемент меньше `minimum`, обновляем минимум.
5. После внутреннего цикла записываем результат в `col_min[j]`.

Почему начинаем с `matrix[0][j]`, а не с большого числа?

Потому что мы не знаем диапазон значений. Первый элемент столбца — корректная
начальная точка. Если начать с `0`, а все числа положительные, минимум
останется `0`, что неправильно:

```text
Столбец: 5 3 7
minimum = 5
i=1: 3 < 5? да, minimum = 3
i=2: 7 < 3? нет
col_min[j] = 3
```

## 9. Разбор `print_result`

```c
void print_result(int** matrix, int n, int m, int* row_max, int* col_min) {
    find_row_max(matrix, n, m, row_max);
    find_col_min(matrix, n, m, col_min);

    output_matrix(matrix, n, m);
    printf("\n");
    output_array(row_max, n);
    printf("\n");
    output_array(col_min, m);
}
```

Порядок действий:

1. Считаем максимумы строк.
2. Считаем минимумы столбцов.
3. Печатаем матрицу.
4. Печатаем `\n` — переходим на новую строку после матрицы.
5. Печатаем массив максимумов.
6. Печатаем `\n`.
7. Печатаем массив минимумов.

После последнего числа `\n` не печатается.

Почему `printf("\n")` стоит отдельно, а не внутри `output_matrix`?

Потому что `output_matrix` специально не печатает `\n` после последней строки.
Если бы она это делала, то после матрицы уже был бы перевод строки, и
`printf("\n")` дал бы лишнюю пустую строку.

## 10. Разбор `output_array`

```c
void output_array(const int* a, int n) {
    for (int i = 0; i < n; i++) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", a[i]);
    }
}
```

- `const int* a` — массив только для чтения.
- Перед каждым элементом, кроме первого, печатается пробел.
- После последнего элемента ничего не печатается.
- Функция не печатает `\n`.

Её можно использовать и для `row_max`, и для `col_min`, потому что оба массива
имеют тип `int*`.

## 11. Полный порядок работы программы

```text
main
 |
 +-- load_matrix
 |    |
 |    +-- читает type, n, m
 |    +-- create_matrix
 |    |    +-- make_static_matrix    (type 1)
 |    |    +-- make_separate_rows    (type 2)
 |    |    +-- make_one_buffer       (type 3)
 |    |    +-- make_one_block        (type 4)
 |    +-- input_matrix
 |
 +-- allocate_results
 |    +-- malloc row_max
 |    +-- malloc col_min
 |
 +-- print_result
 |    +-- find_row_max
 |    +-- find_col_min
 |    +-- output_matrix
 |    +-- output_array(row_max)
 |    +-- output_array(col_min)
 |
 +-- free(row_max)
 +-- free(col_min)
 +-- free_matrix
```

## 12. Пример вычислений

Матрица:

```text
4  3   1
9  0  55
-4 7 111
```

### Максимумы строк

Строка 0:

```text
4, 3, 1 -> максимум 4
```

Строка 1:

```text
9, 0, 55 -> максимум 55
```

Строка 2:

```text
-4, 7, 111 -> максимум 111
```

Результат:

```text
4 55 111
```

### Минимумы столбцов

Столбец 0:

```text
4, 9, -4 -> минимум -4
```

Столбец 1:

```text
3, 0, 7 -> минимум 0
```

Столбец 2:

```text
1, 55, 111 -> минимум 1
```

Результат:

```text
-4 0 1
```

## 13. Ошибки и очистка памяти

Возможные ошибки:

- неверный номер способа;
- неверные размеры;
- размер больше `100 x 100` для статического способа;
- не удалось выделить матрицу;
- не удалось выделить `row_max`;
- не удалось выделить `col_min`;
- ошибка чтения элемента матрицы.

Во всех случаях:

- печатается `n/a`;
- всё, что было выделено, освобождается.

Особый случай: `row_max` выделен, а `col_min` — нет. Тогда:

```c
free(row_max);  // освобождает реальный блок
free(col_min);  // free(NULL), безопасно
free_matrix(...);
```

## 14. Компиляция и проверка

```sh
gcc -std=c11 -Wall -Wextra -Werror -pedantic src/matrix_extended.c -o matrix_extended

printf '2\n3 3\n4 3 1\n9 0 55\n-4 7 111' | ./matrix_extended
```

Ожидаемый вывод:

```text
4 3 1
9 0 55
-4 7 111
4 55 111
-4 0 1
```

Проверка всех способов:

```sh
for t in 1 2 3 4; do
    echo "Способ $t"
    printf "$t\n3 3\n4 3 1\n9 0 55\n-4 7 111" | ./matrix_extended
    echo
done
```

## 15. Частые ошибки

1. **Инициализировать максимум нулём.** Для отрицательных чисел это ошибка.
2. **Инициализировать минимум нулём.** Для положительных чисел это ошибка.
3. **Перепутать строки и столбцы.** Максимумы — по строкам, минимумы — по
   столбцам.
4. **Перепутать порядок вывода.** Сначала матрица, потом максимумы строк,
   потом минимумы столбцов.
5. **Напечатать лишний `\n` в конце.** После последнего минимума перевода
   строки быть не должно.
6. **Забыть освободить `row_max` или `col_min`.** Утечка памяти.
7. **Освобождать `row_max`, не проверив, что он выделен.** Здесь спасает
   `free(NULL)`, но лучше понимать, что происходит.
8. **Не проверить второй `malloc`.** Если `col_min` не выделился, будет
   разыменование `NULL`.

## 16. Чек-лист

- [x] Матрица вводится и выводится как в Quest 3.
- [x] Максимумы считаются по строкам.
- [x] Минимумы считаются по столбцам.
- [x] Для максимумов и минимумов выделяется память.
- [x] Результаты выводятся в правильном порядке.
- [x] После последнего числа нет `\n`.
- [x] При ошибке печатается `n/a`.
- [x] Вся динамическая память освобождается.
