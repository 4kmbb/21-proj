# Компилятор
CC = gcc

# Флаги сборки: предупреждения = ошибки, стандарт C11
CFLAGS = -Wall -Wextra -Werror -std=c11

# Папка build лежит в корне проекта D09.
# Makefile находится в src/s21_string, поэтому путь такой: ../../build
BUILD_DIR = ../../build

# Исходники тестовой программы
SRC = s21_string.c s21_string_test.c
HEADERS = s21_string.h

# По умолчанию собираем все пять заданий
all: strlen_tests strcmp_tests strcpy strcat strchr

# Quest 6
strlen_tests: $(SRC) $(HEADERS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(BUILD_DIR)/Quest_6

# Quest 7
strcmp_tests: $(SRC) $(HEADERS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(BUILD_DIR)/Quest_7

# Quest 8
strcpy: $(SRC) $(HEADERS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(BUILD_DIR)/Quest_8

# Quest 9
strcat: $(SRC) $(HEADERS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(BUILD_DIR)/Quest_9

# Quest 10
strchr: $(SRC) $(HEADERS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(BUILD_DIR)/Quest_10

# Удаляем только наши исполняемые файлы
clean:
	rm -f $(BUILD_DIR)/Quest_6 $(BUILD_DIR)/Quest_7 $(BUILD_DIR)/Quest_8 \
	      $(BUILD_DIR)/Quest_9 $(BUILD_DIR)/Quest_10

# Пересборка с нуля
rebuild: clean all

# Эти имена не являются файлами
.PHONY: all strlen_tests strcmp_tests strcpy strcat strchr clean rebuild
