#ifndef DATA_STAT_H
#define DATA_STAT_H

/*
 * Функции для работы с массивом чисел:
 * максимум, минимум, среднее, дисперсия и сортировка.
 */

double max(double* data, int n);
double min(double* data, int n);
double mean(double* data, int n);
double variance(double* data, int n);
void sort(double* data, int n);

#endif
