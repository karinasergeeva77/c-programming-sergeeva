#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Заполнение матрицы случайными числами
void matrix_random(int rows, int cols, int m[rows][cols], int min, int max) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m[i][j] = min + rand() % (max - min + 1);
        }
    }
}

// Вывод матрицы
void matrix_print(int rows, int cols, int m[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d ", m[i][j]);
        }
        printf("\n");
    }
}

// Транспонирование матрицы
void matrix_transpose(int rows, int cols, int src[rows][cols], int dst[cols][rows]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dst[j][i] = src[i][j];
        }
    }
}

// Умножение матриц
int matrix_multiply(int r1, int c1, int a[r1][c1], 
                    int r2, int c2, int b[r2][c2], 
                    int result[r1][c2]) {
    // Проверка совместимости размеров
    if (c1 != r2) {
        return -1;  // Несовместимые размеры
    }
    
    // Обнуляем результат
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            result[i][j] = 0;
        }
    }
    
    // Умножение
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            for (int k = 0; k < c1; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return 0;
}

// Сумма элементов главной диагонали
int matrix_trace(int n, int m[n][n]) {
    int trace = 0;
    for (int i = 0; i < n; i++) {
        trace += m[i][i];
    }
    return trace;
}

int main(void) {
    srand(time(NULL));
    
    // Матрица A (2x3)
    int A[2][3] = {{1, 2, 3}, {4, 5, 6}};
    // Матрица B (3x2)
    int B[3][2] = {{7, 8}, {9, 10}, {11, 12}};
    // Результат (2x2)
    int C[2][2];
    
    printf("Матрица A (2x3):\n");
    matrix_print(2, 3, A);
    
    printf("\nМатрица B (3x2):\n");
    matrix_print(3, 2, B);
    
    matrix_multiply(2, 3, A, 3, 2, B, C);
    
    printf("\nA × B (2x2):\n");
    matrix_print(2, 2, C);
    
}
