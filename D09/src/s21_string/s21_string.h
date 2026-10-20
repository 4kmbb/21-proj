#ifndef S21_STRING_H
#define S21_STRING_H

#include <stdlib.h>

/*
 * Своя мини-версия библиотеки string.h.
 * Подключаем stdlib.h, потому что в нём объявлен тип size_t,
 * а другие стандартные библиотеки по условию использовать нельзя.
 */

size_t s21_strlen(const char* str);
int s21_strcmp(const char* str1, const char* str2);
char* s21_strcpy(char* dest, const char* src);
char* s21_strcat(char* dest, const char* src);
char* s21_strchr(const char* str, int c);

#endif
