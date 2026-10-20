#include "data_stat.h"

/*
 * Ищем максимальный элемент.
 * Если массив пустой (n <= 0), просто возвращаем 0,
 * чтобы не обращаться к несуществующему data[0].
 */
double max(double* data, int n) {
    double result = 0.0;
    if (n > 0) {
        result = data[0];
        for (int i = 1; i < n; i++) {
            if (data[i] > result) {
                result = data[i];
            }
        }
    }
    return result;
}

/*
 * Ищем минимальный элемент.
 * Логика такая же, как в max, только знак сравнения другой.
 */
double min(double* data, int n) {
    double result = 0.0;
    if (n > 0) {
        result = data[0];
        for (int i = 1; i < n; i++) {
            if (data[i] < result) {
                result = data[i];
            }
        }
    }
    return result;
}

/*
 * Среднее арифметическое: сумма всех элементов / количество.
 */
double mean(double* data, int n) {
    double result = 0.0;
    if (n > 0) {
        double sum = 0.0;
        for (int i = 0; i < n; i++) {
            sum += data[i];
        }
        result = sum / n;
    }
    return result;
}

/*
 * Дисперсия: среднее значение квадратов отклонений от среднего.
 * Сначала находим среднее, потом складываем (data[i] - mean)^2
 * и делим на количество элементов.
 */
double variance(double* data, int n) {
    double result = 0.0;
    if (n > 0) {
        double average = mean(data, n);
        for (int i = 0; i < n; i++) {
            double difference = data[i] - average;
            result += difference * difference;
        }
        result /= n;
    }
    return result;
}

/*
 * Сортировка пузырьком по возрастанию.
 * На каждом проходе "большие" числа уходят в конец массива.
 * Для учебной задачи этого достаточно, а код получается понятным.
 */
void sort(double* data, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (data[j] > data[j + 1]) {
                double temporary = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temporary;
            }
        }
    }
}
