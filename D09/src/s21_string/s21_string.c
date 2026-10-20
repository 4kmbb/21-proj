#include "s21_string.h"

/*
 * Считаем длину строки без завершающего символа '\0'.
 * Просто идём по строке, пока не встретим конец.
 */
size_t s21_strlen(const char* str) {
    size_t length = 0;

    while (str[length] != '\0') {
        length++;
    }

    return length;
}

/*
 * Сравниваем две строки посимвольно.
 * Возвращаем:
 *   0  — строки одинаковые;
 *   <0 — первая строка "меньше";
 *   >0 — первая строка "больше".
 */
int s21_strcmp(const char* str1, const char* str2) {
    int result = 0;
    int i = 0;

    // Идём, пока строки совпадают и хотя бы одна ещё не закончилась
    while (result == 0 && (str1[i] != '\0' || str2[i] != '\0')) {
        result = (unsigned char)str1[i] - (unsigned char)str2[i];
        i++;
    }

    return result;
}

/*
 * Копируем строку src в dest вместе с завершающим '\0'.
 * Возвращаем указатель на начало dest — так делает стандартная strcpy.
 * Вызывающая сторона должна сама позаботиться о размере dest.
 */
char* s21_strcpy(char* dest, const char* src) {
    size_t i = 0;

    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';

    return dest;
}

/*
 * Добавляем строку src в конец строки dest.
 * Сначала находим конец dest, потом копируем туда src.
 */
char* s21_strcat(char* dest, const char* src) {
    size_t dest_length = s21_strlen(dest);
    size_t i = 0;

    while (src[i] != '\0') {
        dest[dest_length + i] = src[i];
        i++;
    }
    dest[dest_length + i] = '\0';

    return dest;
}

/*
 * Ищем символ c в строке str.
 * Если нашли — возвращаем указатель на первое вхождение.
 * Если не нашли — возвращаем NULL.
 * Отдельно обрабатываем c == '\0': тогда результатом будет указатель
 * на завершающий ноль в конце строки.
 */
char* s21_strchr(const char* str, int c) {
    char* result = NULL;
    size_t i = 0;

    while (result == NULL && str[i] != '\0') {
        if (str[i] == (char)c) {
            // Убираем const только потому, что прототип функции этого требует
            result = (char*)(str + i);
        }
        i++;
    }

    if (result == NULL && (char)c == '\0') {
        result = (char*)(str + i);
    }

    return result;
}
