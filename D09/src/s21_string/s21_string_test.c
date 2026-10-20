#include "s21_string.h"

#include <stdio.h>

/*
 * Печатаем результат одного теста.
 * SUCCESS — функция вернула то, что мы ожидали, FAIL — не вернула.
 */
static void print_result(int success) {
    if (success) {
        printf("SUCCESS\n");
    } else {
        printf("FAIL\n");
    }
}

/*
 * Один тест для s21_strlen.
 * input — строка, expected — ожидаемая длина.
 */
static void s21_strlen_case(const char* input, size_t expected) {
    size_t actual = s21_strlen(input);

    // Показываем вход, выход и ожидаемое значение
    printf("Input: \"%s\" | Output: %zu | Expected: %zu | ", input, actual, expected);
    print_result(actual == expected);
}

/*
 * Тесты для s21_strlen:
 * обычная строка, пустая строка (ненормальный случай),
 * строка с пробелами и строка подлиннее (краевые случаи).
 */
void s21_strlen_test(void) {
    printf("=== Testing s21_strlen ===\n");
    s21_strlen_case("hello", 5);
    s21_strlen_case("", 0);
    s21_strlen_case("a b c", 5);
    s21_strlen_case("0123456789abcdef", 16);
    printf("\n");
}

/*
 * Один тест для s21_strcmp.
 * expected_sign: -1 (первая меньше), 0 (равны), 1 (первая больше).
 */
static void s21_strcmp_case(const char* left, const char* right, int expected_sign) {
    int actual = s21_strcmp(left, right);

    // Приводим результат к -1/0/1, чтобы проверять только знак
    int actual_sign = (actual > 0) - (actual < 0);

    printf("Input: \"%s\" vs \"%s\" | Output: %d | Expected sign: %d | ", left, right, actual, expected_sign);
    print_result(actual_sign == expected_sign);
}

/*
 * Тесты для s21_strcmp:
 * одинаковые строки, разные строки в обе стороны,
 * пустая строка слева и справа.
 */
void s21_strcmp_test(void) {
    printf("=== Testing s21_strcmp ===\n");
    s21_strcmp_case("abc", "abc", 0);
    s21_strcmp_case("abc", "abd", -1);
    s21_strcmp_case("abd", "abc", 1);
    s21_strcmp_case("", "abc", -1);
    s21_strcmp_case("abc", "", 1);
    printf("\n");
}

/*
 * В main запускаем тесты.
 * Сейчас здесь strlen и strcmp — те функции, для которых тесты
 * требуются по заданиям 6 и 7.
 */
int main(void) {
    s21_strlen_test();
    s21_strcmp_test();
    return 0;
}
