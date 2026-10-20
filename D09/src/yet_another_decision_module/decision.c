#include "decision.h"

#include <math.h>
#include <stdlib.h>

#include "../data_libs/data_stat.h"

/*
 * Принимаем решение по массиву.
 * Возвращаем 1 (YES), если:
 *  1) все значения попадают в правило трёх сигм;
 *  2) среднее значение не меньше GOLDEN_RATIO.
 * Иначе возвращаем 0 (NO).
 */
int make_decision(double* data, int n) {
    int decision = 0;

    if (data != NULL && n > 0) {
        double m = mean(data, n);
        double sigma = sqrt(variance(data, n));
        double max_value = max(data, n);
        double min_value = min(data, n);

        // Проверяем, что все числа лежат в интервале [m - 3*sigma; m + 3*sigma]
        int three_sigma_rule = (max_value <= m + 3 * sigma) && (min_value >= m - 3 * sigma);

        // Среднее должно быть достаточно большим
        int mean_is_good = (m >= GOLDEN_RATIO);

        decision = three_sigma_rule && mean_is_good;
    }

    return decision;
}
