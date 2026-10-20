# Компилятор, которым собираем программу
CC = gcc

# Флаги: показываем предупреждения и считаем их ошибками,
# а также используем стандарт C11
CFLAGS = -Wall -Wextra -Werror -std=c11

# Папка build находится в корне проекта D09.
# Makefile лежит в src/main_executable_module, поэтому поднимаемся на два уровня вверх.
BUILD_DIR = ../../build
TARGET = $(BUILD_DIR)/Quest_3

# Все исходники, из которых состоит программа
SRC = main_executable_module.c \
      ../data_libs/data_io.c \
      ../data_libs/data_stat.c \
      ../data_module/data_process.c \
      ../yet_another_decision_module/decision.c

# Заголовки, от которых зависит сборка
HEADERS = ../data_libs/data_io.h \
          ../data_libs/data_stat.h \
          ../data_module/data_process.h \
          ../yet_another_decision_module/decision.h

# Цель по умолчанию: собрать исполняемый файл
all: $(TARGET)

# Собираем все исходники сразу в один исполняемый файл.
# -lm подключает библиотеку математики (sqrt, fabs).
$(TARGET): $(SRC) $(HEADERS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) -lm

# Удаляем только результат нашей сборки
clean:
	rm -f $(TARGET)

# Полная пересборка: сначала очистка, потом сборка
rebuild: clean all

# Эти цели не являются файлами, поэтому объявляем их phony
.PHONY: all clean rebuild
