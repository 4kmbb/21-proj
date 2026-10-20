# Мануалы к проекту D09T06

В этой папке лежат подробные разборы всех **небонусных** заданий проекта.
Каждый мануал объясняет не только «что сделать», но и **что делает каждая
строка кода** и зачем она нужна.

## Список мануалов

| Файл | Задание |
|---|---|
| [01_quest1_data_module.md](01_quest1_data_module.md) | Quest 1. Модуль `data_module` |
| [02_quest2_decision.md](02_quest2_decision.md) | Quest 2. Модуль `yet_another_decision_module` |
| [03_quest3_makefile.md](03_quest3_makefile.md) | Quest 3. `main_executable_module` и Makefile |
| [06_quest6_strlen.md](06_quest6_strlen.md) | Quest 6. `s21_strlen` и первые тесты |
| [07_quest7_strcmp.md](07_quest7_strcmp.md) | Quest 7. `s21_strcmp` и его тесты |
| [08_quest8_strcpy.md](08_quest8_strcpy.md) | Quest 8. `s21_strcpy` |
| [09_quest9_strcat.md](09_quest9_strcat.md) | Quest 9. `s21_strcat` |
| [10_quest10_strchr.md](10_quest10_strchr.md) | Quest 10. `s21_strchr` |

Бонусные задания (4, 5, 11, 12, 13) в этот набор не входят, потому что они
не выполнялись.

## Как устроен проект

```text
D09/
├── README_RUS.md
├── Manuals/                 <- эта папка с мануалами
├── materials/               <- правила стиля, тестирования и Makefile
├── build/                   <- сюда попадают собранные программы
└── src/
    ├── data_libs/
    │   ├── data_io.c/.h     <- ввод и вывод массива
    │   └── data_stat.c/.h   <- min, max, mean, variance, sort
    ├── data_module/
    │   ├── data_process.c/.h<- нормализация
    │   └── data_module_entry.c
    ├── yet_another_decision_module/
    │   ├── decision.c/.h    <- решение YES/NO
    │   └── yet_another_decision_module_entry.c
    ├── main_executable_module/
    │   ├── main_executable_module.c
    │   └── Makefile
    └── s21_string/
        ├── s21_string.c/.h  <- свои strlen, strcmp, strcpy, strcat, strchr
        ├── s21_string_test.c<- тесты
        └── Makefile
```

## Общие команды

Сборка Quest 3:

```bash
make -C src/main_executable_module rebuild
./build/Quest_3
```

Сборка Quest 6–10:

```bash
make -C src/s21_string rebuild
./build/Quest_6
./build/Quest_7
./build/Quest_8
./build/Quest_9
./build/Quest_10
```

Проверка стиля:

```bash
clang-format -n src/data_libs/*.c src/data_libs/*.h \
    src/data_module/*.c src/data_module/*.h \
    src/yet_another_decision_module/*.c src/yet_another_decision_module/*.h \
    src/main_executable_module/*.c \
    src/s21_string/*.c src/s21_string/*.h
```

## Как читать разбор

- Если в мануале показан файл целиком, номера строк в таблице совпадают
  с номерами в реальном файле.
- Если показан только фрагмент (например, одна функция), нумерация в
  таблице относится к **этому фрагменту**, а не ко всему файлу.
- Если строка пустая, она обычно не разбирается отдельно: это просто
  разделитель для читаемости.
- Комментарии в коде тоже разбираются: они помогают понять замысел.
- Команды `gcc` и `make` можно выполнять из корня проекта `D09`.

## Важные правила проекта

1. Использовать `system()` и похожие вызовы запрещено.
2. В `s21_string` нельзя подключать `string.h`; разрешены только
   `stdlib.h` и `stdio.h`.
3. Результаты сборки не хранятся в репозитории — папка `build/`
   добавлена в `.gitignore`.
4. Нужно соблюдать стиль Google C++ Style Guide (файл `src/.clang-format`).
5. Нужно следить за памятью: каждый `malloc` должен иметь свой `free`.
6. В функциях желательно иметь один выход; ранние проверки аргументов —
   допустимое исключение.
7. Вложенность блоков — не больше четырёх.
