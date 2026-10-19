#include <stdio.h>
#include <string.h>

#define LEN 100
#define LINE_SIZE 512

void sum(int *buff1, int len1, int *buff2, int len2, int *result, int *result_length);
void sub(int *buff1, int len1, int *buff2, int len2, int *result, int *result_length);
int parse_number_line(char *line, int *buffer, int *length);
int compare_numbers(int *buff1, int len1, int *buff2, int len2);
void normalize_number(int *buffer, int *length);
void print_number(int *buffer, int length);

/*
    Беззнаковая длинная арифметика в массивах цифр.
    На вход идут две строки цифр (каждая цифра отдельным числом 0..9).
    На выходе печатаем сумму и разность (если второе число больше первого, печатаем n/a).
*/
int main(void) {
    char line1[LINE_SIZE];
    char line2[LINE_SIZE];
    int ok = 1;

    // Читаем две строки целиком, чтобы точно разделить два больших числа.
    if (fgets(line1, LINE_SIZE, stdin) == NULL) {
        ok = 0;
    }
    if (ok == 1 && fgets(line2, LINE_SIZE, stdin) == NULL) {
        ok = 0;
    }

    if (ok == 1) {
        int buff1[LEN];
        int buff2[LEN];
        int len1 = 0;
        int len2 = 0;

        ok = parse_number_line(line1, buff1, &len1);
        if (ok == 1) {
            ok = parse_number_line(line2, buff2, &len2);
        }

        if (ok == 1) {
            int sum_result[LEN + 1];
            int sum_len = 0;
            int sub_result[LEN];
            int sub_len = 0;

            sum(buff1, len1, buff2, len2, sum_result, &sum_len);
            print_number(sum_result, sum_len);
            printf("\n");

            if (compare_numbers(buff1, len1, buff2, len2) >= 0) {
                sub(buff1, len1, buff2, len2, sub_result, &sub_len);
                print_number(sub_result, sub_len);
            } else {
                printf("n/a");
            }
        } else {
            printf("n/a");
        }
    } else {
        printf("n/a");
    }

    return 0;
}

// Разбираем одну строку в массив цифр, проверяя, что каждый токен это число 0..9.
int parse_number_line(char *line, int *buffer, int *length) {
    int ok = 1;
    int count = 0;
    char *token = strtok(line, " \t\n\r");

    while (token != NULL && ok == 1) {
        int digit = -1;
        char tail = '\0';

        // Формат "%d%c" помогает отсеять мусор вроде "3a".
        if (sscanf(token, "%d%c", &digit, &tail) != 1) {
            ok = 0;
        }

        if (ok == 1 && (digit < 0 || digit > 9)) {
            ok = 0;
        }

        if (ok == 1 && count >= LEN) {
            ok = 0;
        }

        if (ok == 1) {
            buffer[count] = digit;
            count++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    if (count == 0) {
        ok = 0;
    }

    *length = count;
    return ok;
}

// Сравнение двух чисел в формате массивов цифр: 1 если первое больше, -1 если меньше, 0 если равно.
int compare_numbers(int *buff1, int len1, int *buff2, int len2) {
    int result = 0;

    if (len1 > len2) {
        result = 1;
    } else if (len1 < len2) {
        result = -1;
    } else {
        int index = 0;

        while (index < len1 && result == 0) {
            if (buff1[index] > buff2[index]) {
                result = 1;
            } else if (buff1[index] < buff2[index]) {
                result = -1;
            }
            index++;
        }
    }

    return result;
}

// Удаляем ведущие нули, но оставляем минимум одну цифру.
void normalize_number(int *buffer, int *length) {
    int first_non_zero = 0;

    while (first_non_zero < *length - 1 && buffer[first_non_zero] == 0) {
        first_non_zero++;
    }

    if (first_non_zero > 0) {
        int new_len = *length - first_non_zero;

        for (int i = 0; i < new_len; i++) {
            buffer[i] = buffer[i + first_non_zero];
        }

        *length = new_len;
    }
}

// Складываем числа справа налево, учитывая перенос в следующий разряд.
void sum(int *buff1, int len1, int *buff2, int len2, int *result, int *result_length) {
    int i = len1 - 1;
    int j = len2 - 1;
    int k = LEN;
    int carry = 0;

    while (i >= 0 || j >= 0 || carry > 0) {
        int digit1 = 0;
        int digit2 = 0;

        if (i >= 0) {
            digit1 = buff1[i];
            i--;
        }
        if (j >= 0) {
            digit2 = buff2[j];
            j--;
        }

        int digit_sum = digit1 + digit2 + carry;
        result[k] = digit_sum % 10;
        carry = digit_sum / 10;
        k--;
    }

    // Копируем результат в начало буфера в нормальном порядке.
    int start = k + 1;
    int length = LEN - start + 1;

    for (int idx = 0; idx < length; idx++) {
        result[idx] = result[start + idx];
    }

    *result_length = length;
    normalize_number(result, result_length);
}

// Вычитаем второе число из первого (предполагается, что первое >= второму).
void sub(int *buff1, int len1, int *buff2, int len2, int *result, int *result_length) {
    int i = len1 - 1;
    int j = len2 - 1;
    int k = LEN - 1;
    int borrow = 0;

    while (i >= 0) {
        int digit1 = buff1[i] - borrow;
        int digit2 = 0;

        if (j >= 0) {
            digit2 = buff2[j];
            j--;
        }

        if (digit1 < digit2) {
            digit1 += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        result[k] = digit1 - digit2;
        i--;
        k--;
    }

    // Переносим число в начало массива.
    int start = k + 1;
    int length = LEN - start;

    for (int idx = 0; idx < length; idx++) {
        result[idx] = result[start + idx];
    }

    *result_length = length;
    normalize_number(result, result_length);
}

// Печатаем число как последовательность цифр через пробел.
void print_number(int *buffer, int length) {
    for (int i = 0; i < length; i++) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", buffer[i]);
    }
}
