include/mathlib.h:
```c
#ifndef MATHLIB_H
#define MATHLIB_H

#define MATHLIB_PI 3.14159265358979323846
#define MATHLIB_E  2.71828182845904523536

double ml_abs(double x);
double ml_pow(double base, int exp);
double ml_sqrt(double x);
long long ml_factorial(int n);
long long ml_gcd(long long a, long long b);
double ml_sin(double x);
double ml_cos(double x);

#endif
```
include/vector.h:
```c
#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

typedef struct {
    double *data;
    size_t size;
} Vector;

// Создание и уничтожение
Vector* vec_create(size_t size);
Vector* vec_from_array(const double *arr, size_t size);
void vec_free(Vector *v);
Vector* vec_copy(const Vector *v);

// Базовые операции
void vec_add(Vector *result, const Vector *a, const Vector *b);
void vec_sub(Vector *result, const Vector *a, const Vector *b);
void vec_scale(Vector *v, double scalar);
double vec_dot(const Vector *a, const Vector *b);

// Вывод
void vec_print(const Vector *v);

#endif // VECTOR_H
```
include/matrix.h:
```c
#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

typedef struct {
    double *data;
    size_t rows;
    size_t cols;
} Matrix;

// Создание и уничтожение
Matrix* mat_create(size_t rows, size_t cols);
Matrix* mat_identity(size_t n);
void mat_free(Matrix *m);
Matrix* mat_copy(const Matrix *m);

// Доступ к элементам
double mat_get(const Matrix *m, size_t row, size_t col);
void mat_set(Matrix *m, size_t row, size_t col, double value);

// Операции
void mat_add(Matrix *result, const Matrix *a, const Matrix *b);
void mat_sub(Matrix *result, const Matrix *a, const Matrix *b);
void mat_mul(Matrix *result, const Matrix *a, const Matrix *b);
void mat_scale(Matrix *m, double scalar);
Matrix* mat_transpose(const Matrix *m);
double mat_determinant(const Matrix *m);
Matrix* mat_inverse(const Matrix *m);

// Вывод
void mat_print(const Matrix *m);

#endif // MATRIX_H
```
src/basic.c:
```c
#include <math.h>
#include "mathlib.h"

double ml_sqrt(double x) {
    double guess = x / 2.0;
    double prev_guess;
    do {
        prev_guess = guess;
        guess = (guess + x / guess) / 2.0;
    } while (fabs(guess - prev_guess) > 1e-12);
    return guess;
}

long long ml_factorial(int n) {
    if (n < 0) return 0;
    if (n <= 1) return 1;
    long long result = 1;
    for (int i = 2; i <= n; i++) result *= i;
    return result;
}
```
src/vector.c:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

Vector* vec_create(size_t size) {
    Vector *v = malloc(sizeof(Vector));
    if (v == NULL) return NULL;
    
    v->data = calloc(size, sizeof(double));
    if (v->data == NULL) {
        free(v);
        return NULL;
    }
    
    v->size = size;
    return v;
}

Vector* vec_from_array(const double *arr, size_t size) {
    Vector *v = vec_create(size);
    if (v == NULL) return NULL;
    
    memcpy(v->data, arr, size * sizeof(double));
    return v;
}

void vec_free(Vector *v) {
    if (v == NULL) return;
    free(v->data);
    free(v);
}

Vector* vec_copy(const Vector *v) {
    if (v == NULL) return NULL;
    return vec_from_array(v->data, v->size);
}

void vec_add(Vector *result, const Vector *a, const Vector *b) {
    if (result == NULL || a == NULL || b == NULL) return;
    if (a->size != b->size || result->size != a->size) return;
    
    for (size_t i = 0; i < a->size; i++) {
        result->data[i] = a->data[i] + b->data[i];
    }
}

void vec_sub(Vector *result, const Vector *a, const Vector *b) {
    if (result == NULL || a == NULL || b == NULL) return;
    if (a->size != b->size || result->size != a->size) return;
    
    for (size_t i = 0; i < a->size; i++) {
        result->data[i] = a->data[i] - b->data[i];
    }
}

void vec_scale(Vector *v, double scalar) {
    if (v == NULL) return;
    
    for (size_t i = 0; i < v->size; i++) {
        v->data[i] *= scalar;
    }
}

double vec_dot(const Vector *a, const Vector *b) {
    if (a == NULL || b == NULL || a->size != b->size) return 0.0;
    
    double result = 0.0;
    for (size_t i = 0; i < a->size; i++) {
        result += a->data[i] * b->data[i];
    }
    return result;
}

void vec_print(const Vector *v) {
    if (v == NULL) return;
    
    printf("[");
    for (size_t i = 0; i < v->size; i++) {
        printf("%.2f", v->data[i]);
        if (i < v->size - 1) printf(", ");
    }
    printf("]\n");
}
```
src/matrix.c:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "matrix.h"

Matrix* mat_create(size_t rows, size_t cols) {
    Matrix *m = malloc(sizeof(Matrix));
    if (m == NULL) return NULL;
    
    m->data = calloc(rows * cols, sizeof(double));
    if (m->data == NULL) {
        free(m);
        return NULL;
    }
    
    m->rows = rows;
    m->cols = cols;
    return m;
}

Matrix* mat_identity(size_t n) {
    Matrix *m = mat_create(n, n);
    if (m == NULL) return NULL;
    
    for (size_t i = 0; i < n; i++) {
        m->data[i * n + i] = 1.0;
    }
    
    return m;
}

void mat_free(Matrix *m) {
    if (m == NULL) return;
    free(m->data);
    free(m);
}

Matrix* mat_copy(const Matrix *m) {
    if (m == NULL) return NULL;
    
    Matrix *copy = mat_create(m->rows, m->cols);
    if (copy == NULL) return NULL;
    
    memcpy(copy->data, m->data, m->rows * m->cols * sizeof(double));
    return copy;
}

double mat_get(const Matrix *m, size_t row, size_t col) {
    if (m == NULL || row >= m->rows || col >= m->cols) return 0.0;
    return m->data[row * m->cols + col];
}

void mat_set(Matrix *m, size_t row, size_t col, double value) {
    if (m == NULL || row >= m->rows || col >= m->cols) return;
    m->data[row * m->cols + col] = value;
}

void mat_add(Matrix *result, const Matrix *a, const Matrix *b) {
    if (result == NULL || a == NULL || b == NULL) return;
    if (a->rows != b->rows || a->cols != b->cols) return;
    if (result->rows != a->rows || result->cols != a->cols) return;
    
    for (size_t i = 0; i < a->rows * a->cols; i++) {
        result->data[i] = a->data[i] + b->data[i];
    }
}

void mat_sub(Matrix *result, const Matrix *a, const Matrix *b) {
    if (result == NULL || a == NULL || b == NULL) return;
    if (a->rows != b->rows || a->cols != b->cols) return;
    if (result->rows != a->rows || result->cols != a->cols) return;
    
    for (size_t i = 0; i < a->rows * a->cols; i++) {
        result->data[i] = a->data[i] - b->data[i];
    }
}

void mat_mul(Matrix *result, const Matrix *a, const Matrix *b) {
    if (result == NULL || a == NULL || b == NULL) return;
    if (a->cols != b->rows) return;
    if (result->rows != a->rows || result->cols != b->cols) return;
    
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += a->data[i * a->cols + k] * b->data[k * b->cols + j];
            }
            result->data[i * b->cols + j] = sum;
        }
    }
}

void mat_scale(Matrix *m, double scalar) {
    if (m == NULL) return;
    
    for (size_t i = 0; i < m->rows * m->cols; i++) {
        m->data[i] *= scalar;
    }
}

Matrix* mat_transpose(const Matrix *m) {
    if (m == NULL) return NULL;
    
    Matrix *t = mat_create(m->cols, m->rows);
    if (t == NULL) return NULL;
    
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            t->data[j * m->rows + i] = m->data[i * m->cols + j];
        }
    }
    
    return t;
}

void mat_print(const Matrix *m) {
    if (m == NULL) return;
    
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            printf("%8.2f ", mat_get(m, i, j));
        }
        printf("\n");
    }
}
```
src/main.c:
```c
#include <stdio.h>
#include "mathlib.h"

int main(void) {
    printf("=== Математическая библиотека ===\n\n");
    
    // Базовые функции
    printf("Базовые функции:\n");
    printf("ml_abs(-5) = %.0f\n", ml_abs(-5));
    printf("ml_pow(2, 10) = %.0f\n", ml_pow(2, 10));
    printf("ml_sqrt(16) = %.2f\n", ml_sqrt(16));
    printf("ml_factorial(5) = %lld\n", ml_factorial(5));
    printf("ml_gcd(48, 18) = %lld\n", ml_gcd(48, 18));
    printf("ml_lcm(12, 18) = %lld\n\n", ml_lcm(12, 18));
    
    // Тригонометрия
    printf("Тригонометрия:\n");
    printf("ml_sin(π/2) = %.4f\n", ml_sin(MATHLIB_PI / 2));
    printf("ml_cos(0) = %.4f\n", ml_cos(0));
    printf("ml_deg_to_rad(180) = %.4f\n\n", ml_deg_to_rad(180));
    
    // Векторы
    printf("Векторы:\n");
    double arr1[] = {1.0, 2.0, 3.0};
    double arr2[] = {4.0, 5.0, 6.0};
    
    Vector *v1 = vec_from_array(arr1, 3);
    Vector *v2 = vec_from_array(arr2, 3);
    Vector *v3 = vec_create(3);
    
    printf("v1 = "); vec_print(v1);
    printf("v2 = "); vec_print(v2);
    
    vec_add(v3, v1, v2);
    printf("v1 + v2 = "); vec_print(v3);
    
    printf("v1 · v2 = %.2f\n\n", vec_dot(v1, v2));
    
    // Матрицы
    printf("Матрицы:\n");
    Matrix *m1 = mat_create(2, 3);
    Matrix *m2 = mat_create(3, 2);
    
    mat_set(m1, 0, 0, 1); mat_set(m1, 0, 1, 2); mat_set(m1, 0, 2, 3);
    mat_set(m1, 1, 0, 4); mat_set(m1, 1, 1, 5); mat_set(m1, 1, 2, 6);
    
    mat_set(m2, 0, 0, 7); mat_set(m2, 0, 1, 8);
    mat_set(m2, 1, 0, 9); mat_set(m2, 1, 1, 10);
    mat_set(m2, 2, 0, 11); mat_set(m2, 2, 1, 12);
    
    printf("Матрица A (2x3):\n");
    mat_print(m1);
    printf("\nМатрица B (3x2):\n");
    mat_print(m2);
    
    Matrix *m3 = mat_create(2, 2);
    mat_mul(m3, m1, m2);
    printf("\nA × B (2x2):\n");
    mat_print(m3);
    
    // Освобождение памяти
    vec_free(v1);
    vec_free(v2);
    vec_free(v3);
    mat_free(m1);
    mat_free(m2);
    mat_free(m3);
    
    return 0;
}
```
Makefile:
```c
# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -I include
CFLAGS_DEBUG = -g -O0
CFLAGS_RELEASE = -O2 -DNDEBUG
LDFLAGS = -lm

# Директории
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Файлы
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
LIB_OBJS = $(filter-out $(OBJ_DIR)/main.o,$(OBJS))
TARGET = $(BIN_DIR)/mathlib_demo
LIB = $(BIN_DIR)/libmathlib.a

# Цели по умолчанию
.PHONY: all clean lib demo test

all: lib demo

lib: $(LIB)

demo: $(TARGET)

# Статическая библиотека
$(LIB): $(LIB_OBJS) | $(BIN_DIR)
	ar rcs $@ $^

# Исполняемый файл демо
$(TARGET): $(OBJ_DIR)/main.o $(LIB) | $(BIN_DIR)
	$(CC) $< -L$(BIN_DIR) -lmathlib $(LDFLAGS) -o $@

# Компиляция объектных файлов
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) -c $< -o $@

# Создание директорий
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Тесты
test: $(BIN_DIR)/test_mathlib
	./$(BIN_DIR)/test_mathlib

$(BIN_DIR)/test_mathlib: tests/test_mathlib.c $(LIB) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) $< -L$(BIN_DIR) -lmathlib $(LDFLAGS) -o $@

# Очистка
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
```
meson.build:
```c
project('mathlib', 'c',
    version: '1.0.0',
    default_options: [
        'warning_level=3',
        'c_std=c11'
    ]
)

inc = include_directories('include')

cc = meson.get_compiler('c')
math_dep = cc.find_library('m', required: false)

lib_src = files(
    'src/basic.c',
    'src/vector.c',
    'src/matrix.c'
)

mathlib = static_library('mathlib', lib_src,
    include_directories: inc,
    dependencies: [math_dep]
)

executable('mathlib_demo', 'src/main.c',
    include_directories: inc,
    link_with: mathlib,
    dependencies: [math_dep]
)

test_exe = executable('test_mathlib', 'tests/test_mathlib.c',
    include_directories: inc,
    link_with: mathlib,
    dependencies: [math_dep]
)

test('mathlib tests', test_exe)
```
tests/test_mathlib.c:
```c
#include <stdio.h>
#include <math.h>
#include "mathlib.h"

int _test_failed = 0;

#define ASSERT_EQ(a, b) \
    do { \
        if ((a) != (b)) { \
            printf("FAILED: %s:%d: %lld != %lld\n", __FILE__, __LINE__, (long long)(a), (long long)(b)); \
            _test_failed = 1; \
        } \
    } while(0)

#define ASSERT_DOUBLE_EQ(a, b, eps) \
    do { \
        double diff = (a) - (b); \
        if (diff < 0) diff = -diff; \
        if (diff > (eps)) { \
            printf("FAILED: %s:%d: %f != %f\n", __FILE__, __LINE__, (double)(a), (double)(b)); \
            _test_failed = 1; \
        } \
    } while(0)

void test_factorial(void) {
    ASSERT_EQ(ml_factorial(0), 1);
    ASSERT_EQ(ml_factorial(1), 1);
    ASSERT_EQ(ml_factorial(5), 120);
    ASSERT_EQ(ml_factorial(10), 3628800);
    printf("  test_factorial: PASSED\n");
}

void test_sqrt(void) {
    ASSERT_DOUBLE_EQ(ml_sqrt(4.0), 2.0, 0.0001);
    ASSERT_DOUBLE_EQ(ml_sqrt(9.0), 3.0, 0.0001);
    ASSERT_DOUBLE_EQ(ml_sqrt(2.0), 1.41421356, 0.0001);
    printf("  test_sqrt: PASSED\n");
}

void test_gcd_lcm(void) {
    ASSERT_EQ(ml_gcd(48, 18), 6);
    ASSERT_EQ(ml_gcd(17, 19), 1);
    ASSERT_EQ(ml_lcm(12, 18), 36);
    printf("  test_gcd_lcm: PASSED\n");
}

void test_trigonometry(void) {
    ASSERT_DOUBLE_EQ(ml_sin(0), 0.0, 0.0001);
    ASSERT_DOUBLE_EQ(ml_cos(0), 1.0, 0.0001);
    ASSERT_DOUBLE_EQ(ml_sin(MATHLIB_PI / 2), 1.0, 0.0001);
    printf("  test_trigonometry: PASSED\n");
}

int main(void) {
    printf("\n=== Тестирование MathLib ===\n\n");
    
    test_factorial();
    test_sqrt();
    test_gcd_lcm();
    test_trigonometry();
    
    printf("\n=== Результат ===\n");
    if (_test_failed) {
        printf("Некоторые тесты не пройдены!\n");
        return 1;
    } else {
        printf("Все тесты пройдены успешно!\n");
        return 0;
    }
}
```
