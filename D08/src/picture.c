#include <stdio.h>

#define N 15
#define M 13

void transform(int* buf, int** matr, int n, int m);
void make_picture(int** picture, int n, int m);
void reset_picture(int** picture, int n, int m);
void draw_frame(int** picture, int n, int m, int* frame_w, int* frame_h);
void draw_tree(int** picture, int* tree_trunk, int* tree_foliage);
void draw_sun(int** picture, int sun_data[6][5]);
void output_picture(int** picture, int n, int m);

int main(void) {
    int picture_data[N][M];
    int* picture[N];

    // Превращаем обычный двумерный массив в массив указателей на строки.
    transform(&picture_data[0][0], picture, N, M);
    make_picture(picture, N, M);
    output_picture(picture, N, M);
    return 0;
}

// Заполняем картину с помощью готовых массивов из задания.
void make_picture(int** picture, int n, int m) {
    int frame_w[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int frame_h[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int tree_trunk[] = {7, 7, 7, 7};
    int tree_foliage[] = {3, 3, 3, 3};
    int sun_data[6][5] = {{0, 6, 6, 6, 6}, {0, 0, 6, 6, 6}, {0, 0, 6, 6, 6},
                          {0, 6, 0, 0, 6}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};

    reset_picture(picture, n, m);
    draw_frame(picture, n, m, frame_w, frame_h);
    draw_tree(picture, tree_trunk, tree_foliage);
    draw_sun(picture, sun_data);
}

// Рисуем внешнюю рамку, среднюю горизонтальную линию и разделитель.
void draw_frame(int** picture, int n, int m, int* frame_w, int* frame_h) {
    // Горизонтальные линии находятся в строках 0, 7 и 14.
    for (int i = 0; i < n; i += 7) {
        for (int j = 0; j < m; j++) {
            picture[i][j] = frame_w[j];
        }
    }

    // Левая граница, правая граница и вертикальная линия посередине.
    for (int i = 0; i < n; i++) {
        picture[i][0] = frame_h[i];
        picture[i][m - 1] = frame_h[i];
        picture[i][6] = frame_h[i];
    }
}

// Рисуем крону дерева тройками и ствол семерками.
void draw_tree(int** picture, int* tree_trunk, int* tree_foliage) {
    // Верхняя часть кроны: две тройки в ряд.
    picture[2][3] = tree_foliage[0];
    picture[2][4] = tree_foliage[1];

    // Средняя часть кроны: по четыре тройки в двух строках.
    picture[3][2] = tree_foliage[0];
    picture[3][3] = tree_foliage[1];
    picture[3][4] = tree_foliage[2];
    picture[3][5] = tree_foliage[3];
    picture[4][2] = tree_foliage[0];
    picture[4][3] = tree_foliage[1];
    picture[4][4] = tree_foliage[2];
    picture[4][5] = tree_foliage[3];

    // Нижняя часть кроны: снова две тройки.
    picture[5][3] = tree_foliage[0];
    picture[5][4] = tree_foliage[1];

    // Ствол дерева состоит из семерок.
    picture[6][3] = tree_trunk[0];
    picture[6][4] = tree_trunk[1];
    picture[8][3] = tree_trunk[2];
    picture[8][4] = tree_trunk[3];
    picture[9][3] = tree_trunk[0];
    picture[9][4] = tree_trunk[1];
    picture[10][2] = tree_trunk[0];
    picture[10][3] = tree_trunk[1];
    picture[10][4] = tree_trunk[2];
    picture[10][5] = tree_trunk[3];
}

// Копируем готовую матрицу солнца в правую верхнюю часть картины.
void draw_sun(int** picture, int sun_data[6][5]) {
    // Солнце занимает строки 1-6 и столбцы 7-11.
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 5; j++) {
            picture[1 + i][7 + j] = sun_data[i][j];
        }
    }
}

// Сначала заполняем всю картину нулями.
void reset_picture(int** picture, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            picture[i][j] = 0;
        }
    }
}

// Печатаем картину: числа через пробел, строки через '\n'.
void output_picture(int** picture, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (j > 0) {
                printf(" ");
            }
            printf("%d", picture[i][j]);
        }
        printf("\n");
    }
}

// Настраиваем указатели так, чтобы picture[i] смотрел на i-ю строку buf.
void transform(int* buf, int** matr, int n, int m) {
    for (int i = 0; i < n; i++) {
        matr[i] = buf + i * m;
    }
}
