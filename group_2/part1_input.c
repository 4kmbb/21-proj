#include <stdio.h>

#define WIDTH 80
#define HEIGHT 25

void read_field(int field[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            char c = getchar();
            
            if (c == '\n') {
                c = getchar();
            }
            
            if (c == '*' || c == '1') {
                field[i][j] = 1;
            } else {
                field[i][j] = 0;
            }
        }
    }
}

/*
 * ДЛЯ САМОПРОВЕРКИ:
 */
#ifdef TEST_PART1
int main(void) {
    int field[HEIGHT][WIDTH];
    read_field(field);
    printf("Проверка считывания поля (первые 5 строк):\n");
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", field[i][j] ? '*' : '.');
        }
        printf("\n");
    }
    return 0;
}
#endif
