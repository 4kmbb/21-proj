#include <ncurses.h>
#include <stdio.h>

#define WIDTH 80
#define HEIGHT 25

extern void read_field(int field[HEIGHT][WIDTH]);
extern void update_field(int field[HEIGHT][WIDTH]);

// Отрисовка поля в терминале
void render(int field[HEIGHT][WIDTH]) {
    clear();
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (field[i][j] == 1) {
                printw("*");
            } else {
                printw(" ");
            }
        }
        printw("\n");
    }
    refresh();
}

int main(void) {
    int field[HEIGHT][WIDTH];
    int delay = 100;
    int running = 1;

    // Шаг 1: считываем начальное поле
    read_field(field);

    // Шаг 2: переключаем stdin обратно на физическую клавиатуру терминала
    freopen("/dev/tty", "r", stdin);

    // Шаг 3: настройка ncurses
    initscr();
    noecho();
    cbreak();
    curs_set(0);

    // Шаг 4: игровой цикл
    while (running) {
        render(field);

        timeout(delay);
        int ch = getch();

        if (ch == 'a' || ch == 'A') {
            if (delay > 20) {
                delay -= 20; // Увеличение скорости
            }
        } else if (ch == 'z' || ch == 'Z') {
            if (delay < 400) {
                delay += 20; // Уменьшение скорости
            }
        } else if (ch == ' ') {
            running = 0;     // Выход по пробелу
        }

        // Шаг 5: пересчёт поля
        update_field(field);
    }

    // Шаг 6: освобождаем ресурсы терминала
    endwin();
    return 0;
}
