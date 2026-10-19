#include <stdio.h>

#define NMAX 10

void input(int *buffer, int *length, int *ok);
void output(int *buffer, int length);
int sum_numbers(int *buffer, int length);
int find_numbers(int *buffer, int length, int number, int *numbers);

/*
    Функция получает массив данных через stdin.
    Выдает в stdout особую сумму и массив элементов,
    на которые эта сумма делится без остатка.
*/
int main(void) {
    int buffer[NMAX];
    int length = 0;
    int ok = 1;

    input(buffer, &length, &ok);

    if (ok == 1) {
        int sum = sum_numbers(buffer, length);

        // По условию при отсутствии подходящих (ненулевых четных) элементов выводим n/a.
        if (sum != 0) {
            int numbers[NMAX];
            int numbers_count = find_numbers(buffer, length, sum, numbers);
            printf("%d\n", sum);
            output(numbers, numbers_count);
        } else {
            printf("n/a");
        }
    } else {
        printf("n/a");
    }

    return 0;
}

// Считываем длину и массив; при любой ошибке выставляем флаг ok = 0.
void input(int *buffer, int *length, int *ok) {
    if (scanf("%d", length) != 1) {
        *ok = 0;
    }

    if (*ok == 1 && (*length < 1 || *length > NMAX)) {
        *ok = 0;
    }

    for (int i = 0; i < *length && *ok == 1; i++) {
        if (scanf("%d", &buffer[i]) != 1) {
            *ok = 0;
        }
    }
}

// Вывод массива в одну строку через пробел.
void output(int *buffer, int length) {
    for (int i = 0; i < length; i++) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", buffer[i]);
    }
}

/*
    Суммируем элементы на четность.
    Дополнительно учитываем условие задачи: число 0 считаем нечетным,
    поэтому в сумму его не включаем.
*/
int sum_numbers(int *buffer, int length) {
    int sum = 0;

    for (int i = 0; i < length; i++) {
        if (buffer[i] % 2 == 0 && buffer[i] != 0) {
            sum += buffer[i];
        }
    }

    return sum;
}

/*
    Отбираем элементы исходного массива, на которые сумма делится без остатка.
    Нулевые элементы пропускаем, чтобы не делить на ноль.
*/
int find_numbers(int *buffer, int length, int number, int *numbers) {
    int count = 0;

    for (int i = 0; i < length; i++) {
        if (buffer[i] != 0 && number % buffer[i] == 0) {
            numbers[count] = buffer[i];
            count++;
        }
    }

    return count;
}
