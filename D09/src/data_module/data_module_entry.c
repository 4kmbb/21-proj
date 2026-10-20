#include <stdio.h>
#include <stdlib.h>

#include "../data_libs/data_io.h"
#include "data_process.h"

/*
 * Точка входа модуля data_module.
 * Сначала читаем количество чисел, потом выделяем под них память,
 * затем читаем сами числа и нормализуем их.
 */
int main(void) {
    int n = 0;
    int status = 0;

    // Проверяем, что количество ввели корректно
    if (scanf("%d", &n) != 1 || n <= 0) {
        status = 1;
    } else {
        // Выделяем память под n вещественных чисел
        double* data = malloc((size_t)n * sizeof(double));

        if (data == NULL) {
            // Память не выделилась — дальше работать нельзя
            status = 1;
        } else {
            input(data, n);

            if (normalization(data, n)) {
                output(data, n);
                printf("\n");
            } else {
                printf("ERROR\n");
            }

            // Память, выделенную через malloc, обязательно освобождаем
            free(data);
        }
    }

    // Единственный выход из функции: 0 — успех, 1 — ошибка
    return status;
}
