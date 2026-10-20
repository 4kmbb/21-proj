# Учебные разборы заданий D08T05

Эта папка создана для подробного объяснения кода из папки `src/`.
Здесь разобраны все небонусные задания проекта: Quest 1–7.

## Содержание

1. [Quest 1. Динамический массив и сортировка](01_sort.md)
2. [Quest 2. Версия без утечек памяти](02_sort_no_leak.md)
3. [Quest 3. Матрица и 4 способа выделения памяти](03_matrix.md)
4. [Quest 4. Максимумы строк и минимумы столбцов](04_matrix_extended.md)
5. [Quest 5. Рисунок из массивов](05_picture.md)
6. [Quest 6. Матричная арифметика](06_matrix_arithmetic.md)
7. [Quest 7. Ключ key10.txt](07_key10.md)

Бонусные задания `det.c` и `invert.c` в этой папке не разбираются, потому что
они не входят в обязательную часть.

## Как читать разборы

В каждом файле есть:

- краткое условие задания;
- примеры ввода и вывода;
- полный код;
- разбор по блокам и по строкам;
- объяснение работы с памятью;
- частые ошибки;
- команды для компиляции и проверки.

## Обозначения

- `n` — количество строк матрицы или размер массива;
- `m` — количество столбцов матрицы;
- `a` — одномерный массив;
- `matrix` — матрица, то есть массив указателей на строки;
- `int*` — указатель на одно целое число;
- `int**` — указатель на указатель, через него удобно хранить матрицу;
- `int***` — указатель на `int**`, нужен, когда функция должна изменить
  сам указатель на матрицу.

## Быстрая компиляция

Из корня проекта:

```sh
gcc -std=c11 -Wall -Wextra -Werror -pedantic src/sort.c -o sort
gcc -std=c11 -Wall -Wextra -Werror -pedantic src/sort_no_leak.c -o sort_no_leak
gcc -std=c11 -Wall -Wextra -Werror -pedantic src/matrix.c -o matrix
gcc -std=c11 -Wall -Wextra -Werror -pedantic src/matrix_extended.c -o matrix_extended
gcc -std=c11 -Wall -Wextra -Werror -pedantic src/picture.c -o picture
gcc -std=c11 -Wall -Wextra -Werror -pedantic src/matrix_arithmetic.c -o matrix_arithmetic
```

## Быстрая проверка примеров

```sh
printf '10\n4 3 9 0 1 2 100 2 7 -1' | ./sort

printf '2\n2 2\n4 3\n9 0' | ./matrix

printf '2\n3 3\n4 3 1\n9 0 55\n-4 7 111' | ./matrix_extended

./picture

printf '2\n2 3\n4 3 1\n9 0 2\n3 1\n1\n2\n3' | ./matrix_arithmetic
```

## Важное правило

Во всех программах память, выделенная через `malloc`, должна быть освобождена
через `free`. Если этого не сделать, возникнет утечка памяти.
