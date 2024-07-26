#include <stdio.h>
#include <stdlib.h>

#define WIDTH 80
#define HEIGHT 25
#define NUM_FILES 5

// Функция для инициализации поля из файлов
void initializeFieldFromFile(int field[HEIGHT][WIDTH], const char* filenames[]) {
    for (int i = 0; i < NUM_FILES; i++) {
        FILE *file = fopen(filenames[i], "r");
        if (!file) {
            perror("Ошибка открытия файла");
            continue;
        }
        int y, x;
        while (fscanf(file, "%d %d", &y, &x) != EOF) {
            if (y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH) {
                field[y][x] = 1;
            }
        }
        fclose(file);
    }
}

// Функция для отображения поля
void displayField(int field[HEIGHT][WIDTH]) {
    printf("\033[H\033[J"); // Очистка экрана с использованием ANSI escape кодов
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (field[y][x] == 1) {
                printf("O"); // Живая клетка
            } else {
                printf(" "); // Пустая клетка
            }
        }
        printf("\n");
    }
}

// Функция для подсчета соседей клетки
int countNeighbors(int field[HEIGHT][WIDTH], int y, int x) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int ny = y + i;
            int nx = x + j;
            if (!(i == 0 && j == 0) && ny >= 0 && ny < HEIGHT && nx >= 0 && nx < WIDTH) {
                count += field[ny][nx];
            }
        }
    }
    return count;
}

// Функция для обновления состояния поля
void updateField(int field[HEIGHT][WIDTH]) {
    int newField[HEIGHT][WIDTH];
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int neighbors = countNeighbors(field, y, x);
            if (field[y][x] == 1) {
                if (neighbors < 2 || neighbors > 3) {
                    newField[y][x] = 0;
                } else {
                    newField[y][x] = 1;
                }
            } else {
                if (neighbors == 3) {
                    newField[y][x] = 1;
                } else {
                    newField[y][x] = 0;
                }
            }
        }
    }
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            field[y][x] = newField[y][x];
        }
    }
}

// Функция для задержки (примитивный цикл задержки)
void delay(int milliseconds) {
    volatile long long count;
    for (count = 0; count < milliseconds * 100000; count++) {
        // Ничего не делать
    }
}

int main(int argc, char *argv[]) {
    int field[HEIGHT][WIDTH] = {0};
    const char* filenames[NUM_FILES] = {
        "pattern1.txt",
        "pattern2.txt",
        "pattern3.txt",
        "pattern4.txt",
        "pattern5.txt"
    };

    // Проверка наличия аргумента для задания скорости
    int speed = 200; // Задержка по умолчанию в миллисекундах
    if (argc > 1) {
        speed = atoi(argv[1]);
    }

    initializeFieldFromFile(field, filenames);

    while (1) {
        displayField(field);
        updateField(field);
        delay(speed); // Задержка в миллисекундах
    }

    return 0;
}

