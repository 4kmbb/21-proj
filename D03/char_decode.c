#include <stdio.h>

int encode_symbol(char symbol) {
    /* Перевод символа в ASCII-код. */
    return (unsigned char)symbol;
}

char decode_symbol(int value) {
    /* Перевод ASCII-кода обратно в символ. */
    return (char)value;
}

int hex_digit_to_int(char symbol) {
    /* Перевод одного HEX-символа в число 0..15. */
    int result = -1;

    if (symbol >= '0' && symbol <= '9') {
        result = symbol - '0';
    } else if (symbol >= 'A' && symbol <= 'F') {
        result = symbol - 'A' + 10;
    } else if (symbol >= 'a' && symbol <= 'f') {
        result = symbol - 'a' + 10;
    }

    return result;
}

int decode_hex_pair(char first, char second, int *ok) {
    /* Перевод пары HEX-символов в один байт. */
    int value1 = hex_digit_to_int(first);
    int value2 = hex_digit_to_int(second);
    int result = 0;

    if (value1 == -1 || value2 == -1) {
        *ok = 0;
    } else {
        result = value1 * 16 + value2;
    }

    return result;
}

int process_encode_mode(void) {
    /* Режим 0: из 'A B C' делаем '41 42 43'. */
    int ok = 1;
    int printed = 0;
    int token_length = 0;
    char token_char = '\0';
    int ch = getchar();

    while (ch != '\n' && ch != EOF && ok == 1) {
        if (ch == ' ') {
            if (token_length != 1) {
                ok = 0;
            } else {
                if (printed == 1) {
                    printf(" ");
                }
                printf("%02X", encode_symbol(token_char));
                printed = 1;
                token_length = 0;
            }
        } else {
            token_length = token_length + 1;
            if (token_length == 1) {
                token_char = (char)ch;
            } else {
                ok = 0;
            }
        }
        if (ok == 1) {
            ch = getchar();
        }
    }

    if (ok == 1) {
        if (token_length == 1) {
            if (printed == 1) {
                printf(" ");
            }
            printf("%02X", encode_symbol(token_char));
        } else {
            ok = 0;
        }
    }

    return ok;
}

int process_decode_mode(void) {
    /* Режим 1: из '41 42 43' делаем 'A B C'. */
    int ok = 1;
    int printed = 0;
    int token_length = 0;
    char first = '\0';
    char second = '\0';
    int ch = getchar();

    while (ch != '\n' && ch != EOF && ok == 1) {
        if (ch == ' ') {
            if (token_length != 2) {
                ok = 0;
            } else {
                int value = decode_hex_pair(first, second, &ok);
                if (ok == 1) {
                    if (printed == 1) {
                        printf(" ");
                    }
                    printf("%c", decode_symbol(value));
                    printed = 1;
                    token_length = 0;
                }
            }
        } else {
            token_length = token_length + 1;
            if (token_length == 1) {
                first = (char)ch;
            } else if (token_length == 2) {
                second = (char)ch;
            } else {
                ok = 0;
            }
        }
        if (ok == 1) {
            ch = getchar();
        }
    }

    if (ok == 1) {
        if (token_length == 2) {
            int value = decode_hex_pair(first, second, &ok);
            if (ok == 1) {
                if (printed == 1) {
                    printf(" ");
                }
                printf("%c", decode_symbol(value));
            }
        } else {
            ok = 0;
        }
    }

    return ok;
}

int main(int argc, char *argv[]) {
    int ok = 1;

    /* Выбор режима работы по аргументу командной строки. */
    if (argc != 2) {
        ok = 0;
    } else if (argv[1][0] == '0' && argv[1][1] == '\0') {
        ok = process_encode_mode();
    } else if (argv[1][0] == '1' && argv[1][1] == '\0') {
        ok = process_decode_mode();
    } else {
        ok = 0;
    }

    if (ok == 0) {
        printf("n/a");
    }

    return 0;
}
