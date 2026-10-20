#include <stdio.h>
#include <stdlib.h>

#include "../data_libs/data_io.h"
#include "decision.h"

/*
 * Точка входа модуля yet_another_decision_module.
 * Читаем данные, принимаем решение и печатаем YES или NO.
 */
int main(void) {
    int n = 0;
    int status = 0;

    if (scanf("%d", &n) != 1 || n <= 0) {
        status = 1;
    } else {
        double* data = malloc((size_t)n * sizeof(double));

        if (data == NULL) {
            status = 1;
        } else {
            input(data, n);

            if (make_decision(data, n)) {
                printf("YES\n");
            } else {
                printf("NO\n");
            }

            free(data);
        }
    }

    return status;
}
