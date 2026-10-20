#include "data_process.h"

#include <math.h>
#include <stdlib.h>

#include "../data_libs/data_stat.h"

/*
 * Приводим все числа к диапазону [0, 1].
 * Формула: (x - min) / (max - min).
 * Возвращаем 1, если нормализация получилась, и 0, если нет.
 */
int normalization(double* data, int n) {
    int result = 1;

    if (data == NULL || n <= 0) {
        // Некорректные входные данные — нормализовать нечего
        result = 0;
    } else {
        double max_value = max(data, n);
        double min_value = min(data, n);
        double size = max_value - min_value;

        /*
         * Если max и min совпадают, то size == 0 и делить на него нельзя.
         * Поэтому нормализуем только когда разница заметна.
         */
        if (fabs(size) > EPS) {
            for (int i = 0; i < n; i++) {
                data[i] = (data[i] - min_value) / size;
            }
        } else {
            result = 0;
        }
    }

    return result;
}
