#ifndef DATA_PROCESS_H
#define DATA_PROCESS_H

/*
 * EPS — маленькое число для сравнения вещественных значений.
 * Сравнивать double через == нельзя, поэтому используем такой "зазор".
 */
#define EPS 1E-6

int normalization(double* data, int n);

#endif
