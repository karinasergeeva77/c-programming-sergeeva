#include <stdio.h>

#define MAX_BRIGHTNESS 255

// Инвертирование (негатив)
void image_invert(int rows, int cols, int img[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            img[i][j] = MAX_BRIGHTNESS - img[i][j];
        }
    }
}

// Увеличение яркости
void image_brighten(int rows, int cols, int img[rows][cols], int delta) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            img[i][j] += delta;
            if (img[i][j] > MAX_BRIGHTNESS) {
                img[i][j] = MAX_BRIGHTNESS;
            }
            if (img[i][j] < 0) {
                img[i][j] = 0;
            }
        }
    }
}

// Пороговая фильтрация (бинаризация)
void image_threshold(int rows, int cols, int img[rows][cols], int threshold) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            img[i][j] = (img[i][j] > threshold) ? MAX_BRIGHTNESS : 0;
        }
    }
}

// Размытие (усреднение с соседями)
void image_blur(int rows, int cols, int src[rows][cols], int dst[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int sum = 0;
            int count = 0;
            
            // Проходим по соседям (3x3)
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    int ni = i + di;
                    int nj = j + dj;
                    
                    // Проверяем границы
                    if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
                        sum += src[ni][nj];
                        count++;
                    }
                }
            }
            
            dst[i][j] = sum / count;
        }
    }
}

// Поворот на 90° по часовой стрелке
void image_rotate_90(int rows, int cols, int src[rows][cols], int dst[cols][rows]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dst[j][rows - 1 - i] = src[i][j];
        }
    }
}

// Отражение по горизонтали
void image_flip_horizontal(int rows, int cols, int img[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols / 2; j++) {
            int temp = img[i][j];
            img[i][j] = img[i][cols - 1 - j];
            img[i][cols - 1 - j] = temp;
        }
    }
}

// Отражение по вертикали 
void image_flip_vertical(int rows, int cols, int img[rows][cols]) {
    for (int i = 0; i < rows / 2; i++) {
        for (int j = 0; j < cols; j++) {
            int temp = img[i][j];
            img[i][j] = img[rows - 1 - i][j];
            img[rows - 1 - i][j] = temp;
        }
    }
}

// Вывод изображения
void image_print(int rows, int cols, int img[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d ", img[i][j]);
        }
        printf("\n");
    }
}

// Копирование изображения
void image_copy(int rows, int cols, int src[rows][cols], int dst[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dst[i][j] = src[i][j];
        }
    }
}

int main(void) {
    // Исходное изображение 4x4
    int img[4][4] = {
        {100, 150, 200, 250},
        {50, 100, 150, 200},
        {0, 50, 100, 150},
        {25, 75, 125, 175}
    };
    
    printf("Исходное \"изображение\" 4x4:\n");
    image_print(4, 4, img);
    
    // Тестирование инвертирования
    int inverted[4][4];
    image_copy(4, 4, img, inverted);
    image_invert(4, 4, inverted);
    printf("\nПосле invert:\n");
    image_print(4, 4, inverted);
    
    // Тестирование пороговой фильтрации (threshold = 100)
    int thresholded[4][4];
    image_copy(4, 4, img, thresholded);
    image_threshold(4, 4, thresholded, 100);
    printf("\nПосле threshold(100):\n");
    image_print(4, 4, thresholded);
    
    return 0;
}
