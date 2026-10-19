#include <stdio.h>

// Функция вычисляет максимум и минимум из трех чисел и записывает результат по указателям.
void maxmin(int prob1, int prob2, int prob3, int *max, int *min);

// Точка входа: читаем 3 числа, проверяем ввод и печатаем max/min.
int main(void) {
    int x = 0;
    int y = 0;
    int z = 0;
    int read_result = scanf("%d %d %d", &x, &y, &z);

    if (read_result == 3) {
        int max_value = 0;
        int min_value = 0;

        maxmin(x, y, z, &max_value, &min_value);
        printf("%d %d", max_value, min_value);
    } else {
        // Если входные данные некорректны, выводим сообщение об ошибке.
        printf("n/a");
    }

    return 0;
}

// Эту функцию сохраняем по структуре задачи, но исправляем ошибки работы с указателями.
void maxmin(int prob1, int prob2, int prob3, int *max, int *min) {
    // Берем первое число как стартовое значение и для максимума, и для минимума.
    *max = prob1;
    *min = prob1;

    // Сравниваем второе число с текущими экстремумами.
    if (prob2 > *max) {
        *max = prob2;
    }
    if (prob2 < *min) {
        *min = prob2;
    }

    // Сравниваем третье число с текущими экстремумами.
    if (prob3 > *max) {
        *max = prob3;
    }
    if (prob3 < *min) {
        *min = prob3;
    }
}
