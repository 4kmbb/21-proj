#include <stdio.h>
#include <stdlib.h>

#include "../data_libs/data_io.h"
#include "../data_libs/data_stat.h"
#include "../data_module/data_process.h"
#include "../yet_another_decision_module/decision.h"

/*
 * Главная программа третьего задания.
 * Она показывает все этапы обработки данных:
 * ввод -> нормализация -> сортировка -> решение.
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
            printf("LOAD DATA...\n");
            input(data, n);

            printf("RAW DATA:\n\t");
            output(data, n);

            printf("\nNORMALIZED DATA:\n\t");
            if (normalization(data, n)) {
                output(data, n);

                printf("\nSORTED NORMALIZED DATA:\n\t");
                sort(data, n);
                output(data, n);

                printf("\nFINAL DECISION:\n\t");
                if (make_decision(data, n)) {
                    printf("YES\n");
                } else {
                    printf("NO\n");
                }
            } else {
                // Если нормализовать нельзя, дальнейшие шаги не имеют смысла
                printf("ERROR\n");
            }

            free(data);
        }
    }

    return status;
}
