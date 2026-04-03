# Практическая работа 8: Многофайловые проекты
**Студент:** Сергеева Карина Алексеевна
**Группа:** 1зб_ИВТ(ускор.)/25
**Дата:** 31 марта 2026 г.
---
## Задание 8.1: Математическая библиотека
### Постановка задачи
Создайте библиотеку математических функций.
### Структура проекта
mathlib/
├── include/
│   ├── mathlib.h      # Основной заголовок
│   ├── vector.h       # Операции с векторами
│   └── matrix.h       # Операции с матрицами
├── src/
│   ├── basic.c        # Базовые функции
│   ├── vector.c       # Реализация векторов
│   ├── matrix.c       # Реализация матриц
│   └── main.c         # Демонстрация
├── tests/
│   └── test_mathlib.c # Тесты
├── Makefile
└── meson.build
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| Vector | struct | Динамический вектор |
| Matrix | struct | Матрица |
| ml_sqrt | double (*)(double) | Квадратный корень |
| ml_factorial | long long (*)(int) | Факториал |
| ml_gcd | long long (*)(long long, long long) | НОД |
### Код программы
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
### Результаты работы
![Результат выполнения](practice-8/screenshots/task-8-1.png)
---
## Задание 8.2: Библиотека строк
### Постановка задачи
Создайте библиотеку для работы со строками.
### Структура проекта
strlib/
├── include/
│   └── strlib.h
├── src/
│   ├── strlib_core.c   # Базовые операции
│   ├── strlib_search.c # Поиск
│   ├── strlib_format.c # Форматирование
│   └── main.c
├── tests/
│   └── test_strlib.c
├── Makefile
└── meson.build
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| String | struct | Динамическая строка |
| str_new | String* (*)(const char*) | Создание строки |
| str_append | void (*)(String*, const char*) | Добавление текста |
| str_split | String** (*)(const String*, char, size_t*) | Разделение строки |
### Код программы
include/strlib.h:
```c
#ifndef STRLIB_H
#define STRLIB_H

typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} String;

String* str_new(const char *init);
void str_append(String *s, const char *text);
void str_replace(String *s, const char *old_str, const char *new_str);
String** str_split(const String *s, char delimiter, size_t *count);
String* str_join(String **parts, size_t count, const char *separator);
String* str_reverse(const String *s);

#endif
```
src/strlib_core.c:
```c
#include <stdlib.h>
#include <string.h>
#include "strlib.h"

String* str_new(const char *init) {
    String *s = malloc(sizeof(String));
    s->length = strlen(init);
    s->capacity = s->length + 16;
    s->data = malloc(s->capacity + 1);
    strcpy(s->data, init);
    return s;
}

void str_append(String *s, const char *text) {
    size_t len = strlen(text);
    s->data = realloc(s->data, s->length + len + 1);
    strcpy(s->data + s->length, text);
    s->length += len;
}
```
src/strlib_search.c:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strlib.h"

// Поиск подстроки (первое вхождение)
int str_find(const String *s, const char *substr) {
    if (s == NULL || substr == NULL) return -1;
    
    char *pos = strstr(s->data, substr);
    if (pos == NULL) return -1;
    
    return pos - s->data;
}

// Поиск подстроки (последнее вхождение)
int str_find_last(const String *s, const char *substr) {
    if (s == NULL || substr == NULL) return -1;
    
    int last_pos = -1;
    char *pos = s->data;
    size_t sub_len = strlen(substr);
    
    while ((pos = strstr(pos, substr)) != NULL) {
        last_pos = pos - s->data;
        pos += sub_len;
    }
    
    return last_pos;
}

// Поиск символа
int str_find_char(const String *s, char c) {
    if (s == NULL) return -1;
    
    char *pos = strchr(s->data, c);
    if (pos == NULL) return -1;
    
    return pos - s->data;
}

// Проверка наличия подстроки
bool str_contains(const String *s, const char *substr) {
    return str_find(s, substr) != -1;
}

// Проверка начала строки
bool str_starts_with(const String *s, const char *prefix) {
    if (s == NULL || prefix == NULL) return false;
    
    size_t prefix_len = strlen(prefix);
    if (prefix_len > s->length) return false;
    
    return strncmp(s->data, prefix, prefix_len) == 0;
}

// Проверка окончания строки
bool str_ends_with(const String *s, const char *suffix) {
    if (s == NULL || suffix == NULL) return false;
    
    size_t suffix_len = strlen(suffix);
    if (suffix_len > s->length) return false;
    
    return strcmp(s->data + s->length - suffix_len, suffix) == 0;
}

// Количество вхождений подстроки
size_t str_count(const String *s, const char *substr) {
    if (s == NULL || substr == NULL) return 0;
    
    size_t count = 0;
    char *pos = s->data;
    size_t sub_len = strlen(substr);
    
    while ((pos = strstr(pos, substr)) != NULL) {
        count++;
        pos += sub_len;
    }
    
    return count;
}

// Замена подстроки
void str_replace(String *s, const char *old_str, const char *new_str) {
    if (s == NULL || old_str == NULL || new_str == NULL) return;
    
    String *result = str_new("");
    if (result == NULL) return;
    
    char *pos = s->data;
    char *found;
    size_t old_len = strlen(old_str);
    
    while ((found = strstr(pos, old_str)) != NULL) {
        // Добавляем текст до найденной подстроки
        str_append(result, pos);
        result->length -= strlen(pos);
        str_append(result, new_str);
        pos = found + old_len;
    }
    
    str_append(result, pos);
    
    // Заменяем содержимое
    str_clear(s);
    str_append(s, result->data);
    str_free(result);
}
```
src/strlib_format.c:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "strlib.h"

// Преобразование в верхний регистр
String* str_to_upper(const String *s) {
    if (s == NULL) return NULL;
    
    String *result = str_copy(s);
    if (result == NULL) return NULL;
    
    for (size_t i = 0; i < result->length; i++) {
        result->data[i] = toupper(result->data[i]);
    }
    
    return result;
}

// Преобразование в нижний регистр
String* str_to_lower(const String *s) {
    if (s == NULL) return NULL;
    
    String *result = str_copy(s);
    if (result == NULL) return NULL;
    
    for (size_t i = 0; i < result->length; i++) {
        result->data[i] = tolower(result->data[i]);
    }
    
    return result;
}

// Удаление пробелов с обоих концов
String* str_trim(const String *s) {
    if (s == NULL) return NULL;
    
    size_t start = 0;
    size_t end = s->length;
    
    while (start < end && isspace(s->data[start])) start++;
    while (end > start && isspace(s->data[end - 1])) end--;
    
    if (start == 0 && end == s->length) {
        return str_copy(s);
    }
    
    char *sub = malloc(end - start + 1);
    memcpy(sub, s->data + start, end - start);
    sub[end - start] = '\0';
    
    String *result = str_new(sub);
    free(sub);
    
    return result;
}

// Удаление пробелов слева
String* str_trim_left(const String *s) {
    if (s == NULL) return NULL;
    
    size_t start = 0;
    while (start < s->length && isspace(s->data[start])) start++;
    
    return str_substring(s, start, s->length - start);
}

// Удаление пробелов справа
String* str_trim_right(const String *s) {
    if (s == NULL) return NULL;
    
    size_t end = s->length;
    while (end > 0 && isspace(s->data[end - 1])) end--;
    
    return str_substring(s, 0, end);
}

// Разделение строки на части
String** str_split(const String *s, char delimiter, size_t *count) {
    if (s == NULL || count == NULL) return NULL;
    
    *count = 0;
    String **parts = NULL;
    size_t start = 0;
    
    for (size_t i = 0; i <= s->length; i++) {
        if (i == s->length || s->data[i] == delimiter) {
            if (i > start) {
                parts = realloc(parts, (*count + 1) * sizeof(String*));
                char *sub = malloc(i - start + 1);
                memcpy(sub, s->data + start, i - start);
                sub[i - start] = '\0';
                parts[*count] = str_new(sub);
                free(sub);
                (*count)++;
            }
            start = i + 1;
        }
    }
    
    return parts;
}

// Объединение частей в строку
String* str_join(String **parts, size_t count, const char *separator) {
    if (parts == NULL || count == 0) return str_new("");
    
    String *result = str_copy(parts[0]);
    
    for (size_t i = 1; i < count; i++) {
        str_append(result, separator);
        str_append(result, parts[i]->data);
    }
    
    return result;
}

// Извлечение подстроки
String* str_substring(const String *s, size_t start, size_t len) {
    if (s == NULL) return NULL;
    if (start >= s->length) return str_new("");
    
    if (start + len > s->length) {
        len = s->length - start;
    }
    
    char *sub = malloc(len + 1);
    memcpy(sub, s->data + start, len);
    sub[len] = '\0';
    
    String *result = str_new(sub);
    free(sub);
    
    return result;
}

// Повторение текста
String* str_repeat(const char *text, size_t times) {
    if (text == NULL || times == 0) return str_new("");
    
    size_t text_len = strlen(text);
    size_t total_len = text_len * times;
    
    char *result_str = malloc(total_len + 1);
    result_str[0] = '\0';
    
    for (size_t i = 0; i < times; i++) {
        strcat(result_str, text);
    }
    
    String *result = str_new(result_str);
    free(result_str);
    
    return result;
}

// Переворот строки
String* str_reverse(const String *s) {
    if (s == NULL) return NULL;
    
    String *result = str_new_capacity(s->length);
    
    for (int i = s->length - 1; i >= 0; i--) {
        str_append_char(result, s->data[i]);
    }
    
    return result;
}

// Сравнение строк
int str_compare(const String *a, const String *b) {
    if (a == NULL && b == NULL) return 0;
    if (a == NULL) return -1;
    if (b == NULL) return 1;
    return strcmp(a->data, b->data);
}

// Проверка на равенство
bool str_equals(const String *a, const String *b) {
    if (a == NULL && b == NULL) return true;
    if (a == NULL || b == NULL) return false;
    return strcmp(a->data, b->data) == 0;
}

// Проверка на равенство (без учёта регистра)
bool str_equals_ignore_case(const String *a, const String *b) {
    if (a == NULL && b == NULL) return true;
    if (a == NULL || b == NULL) return false;
    return strcasecmp(a->data, b->data) == 0;
}

// Освобождение массива строк
void str_free_array(String **arr, size_t count) {
    if (arr == NULL) return;
    
    for (size_t i = 0; i < count; i++) {
        str_free(arr[i]);
    }
    free(arr);
}
```
src/main.c:
```c
#include <stdio.h>
#include "strlib.h"

int main(void) {
    printf("=== Библиотека строк (strlib) ===\n\n");
    
    // Создание строк
    String *s = str_new("Hello");
    printf("Создана строка: ");
    str_println(s);
    
    // Добавление
    str_append(s, ", World!");
    printf("После str_append: ");
    str_println(s);
    
    // Длина
    printf("Длина строки: %zu\n", str_length(s));
    
    // Поиск
    int pos = str_find(s, "World");
    printf("Позиция 'World': %d\n", pos);
    
    // Замена
    str_replace(s, "World", "C Programmer");
    printf("После замены: ");
    str_println(s);
    
    // Регистр
    String *upper = str_to_upper(s);
    printf("Верхний регистр: ");
    str_println(upper);
    
    // Обрезка пробелов
    String *trimmed = str_new("   Hello   ");
    printf("До обрезки: '");
    str_print(trimmed);
    printf("'\n");
    
    String *trim_result = str_trim(trimmed);
    printf("После обрезки: '");
    str_println(trim_result);
    
    // Разделение строки
    String *csv = str_new("apple,banana,cherry,date");
    size_t count;
    String **parts = str_split(csv, ',', &count);
    printf("Разделение строки: ");
    for (size_t i = 0; i < count; i++) {
        printf("[%s]", parts[i]->data);
        if (i < count - 1) printf(", ");
    }
    printf("\n");
    
    // Объединение
    String *joined = str_join(parts, count, " | ");
    printf("Объединение: ");
    str_println(joined);
    
    // Переворот строки
    String *reversed = str_reverse(s);
    printf("Переворот строки: ");
    str_println(reversed);
    
    // Сравнение
    String *s1 = str_new("Hello");
    String *s2 = str_new("hello");
    printf("Сравнение 'Hello' и 'hello' (с учётом регистра): %d\n", str_compare(s1, s2));
    printf("Сравнение 'Hello' и 'hello' (без учёта регистра): %d\n", str_equals_ignore_case(s1, s2));
    
    // Освобождение памяти
    str_free(s);
    str_free(upper);
    str_free(trimmed);
    str_free(trim_result);
    str_free(csv);
    str_free_array(parts, count);
    str_free(joined);
    str_free(reversed);
    str_free(s1);
    str_free(s2);
    
    return 0;
}
```
Makefile:
```c
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -I include
CFLAGS_DEBUG = -g -O0
CFLAGS_RELEASE = -O2 -DNDEBUG

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
LIB_OBJS = $(filter-out $(OBJ_DIR)/main.o,$(OBJS))
TARGET = $(BIN_DIR)/strlib_demo
LIB = $(BIN_DIR)/libstrlib.a

.PHONY: all clean lib demo test

all: lib demo

lib: $(LIB)

demo: $(TARGET)

$(LIB): $(LIB_OBJS) | $(BIN_DIR)
	ar rcs $@ $^

$(TARGET): $(OBJ_DIR)/main.o $(LIB) | $(BIN_DIR)
	$(CC) $< -L$(BIN_DIR) -lstrlib -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

test: $(BIN_DIR)/test_strlib
	./$(BIN_DIR)/test_strlib

$(BIN_DIR)/test_strlib: tests/test_strlib.c $(LIB) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) $< -L$(BIN_DIR) -lstrlib -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
```
tests/test_strlib.c:
```c
#include <stdio.h>
#include "strlib.h"

static int _test_failed = 0;

#define ASSERT_EQ(a, b) \
    do { \
        if ((a) != (b)) { \
            printf("  FAILED: %s:%d\n", __FILE__, __LINE__); \
            _test_failed = 1; \
        } \
    } while(0)

#define ASSERT_STR_EQ(a, b) \
    do { \
        if (strcmp((a), (b)) != 0) { \
            printf("  FAILED: %s:%d: \"%s\" != \"%s\"\n", __FILE__, __LINE__, (a), (b)); \
            _test_failed = 1; \
        } \
    } while(0)

void test_create(void) {
    printf("  test_create: ");
    String *s = str_new("Hello");
    ASSERT_STR_EQ(s->data, "Hello");
    ASSERT_EQ(s->length, 5);
    str_free(s);
    printf("PASSED\n");
}

void test_append(void) {
    printf("  test_append: ");
    String *s = str_new("Hello");
    str_append(s, ", World!");
    ASSERT_STR_EQ(s->data, "Hello, World!");
    str_free(s);
    printf("PASSED\n");
}

void test_find(void) {
    printf("  test_find: ");
    String *s = str_new("Hello, World!");
    int pos = str_find(s, "World");
    ASSERT_EQ(pos, 7);
    str_free(s);
    printf("PASSED\n");
}

void test_replace(void) {
    printf("  test_replace: ");
    String *s = str_new("Hello, World!");
    str_replace(s, "World", "C Programmer");
    ASSERT_STR_EQ(s->data, "Hello, C Programmer!");
    str_free(s);
    printf("PASSED\n");
}

void test_trim(void) {
    printf("  test_trim: ");
    String *s = str_new("   Hello   ");
    String *t = str_trim(s);
    ASSERT_STR_EQ(t->data, "Hello");
    str_free(s);
    str_free(t);
    printf("PASSED\n");
}

void test_split_join(void) {
    printf("  test_split_join: ");
    String *s = str_new("a,b,c,d");
    size_t count;
    String **parts = str_split(s, ',', &count);
    ASSERT_EQ(count, 4);
    ASSERT_STR_EQ(parts[0]->data, "a");
    ASSERT_STR_EQ(parts[1]->data, "b");
    ASSERT_STR_EQ(parts[2]->data, "c");
    ASSERT_STR_EQ(parts[3]->data, "d");
    
    String *joined = str_join(parts, count, "-");
    ASSERT_STR_EQ(joined->data, "a-b-c-d");
    
    str_free(s);
    str_free_array(parts, count);
    str_free(joined);
    printf("PASSED\n");
}

int main(void) {
    printf("\n=== Тестирование strlib ===\n\n");
    
    test_create();
    test_append();
    test_find();
    test_replace();
    test_trim();
    test_split_join();
    
    printf("\n=== Результат ===\n");
    if (_test_failed) {
        printf("❌ Некоторые тесты не пройдены!\n");
        return 1;
    } else {
        printf("✅ Все тесты пройдены успешно!\n");
        return 0;
    }
}
```
### Результаты работы
![Результат выполнения](practice-8/screenshots/task-8-2.png)
---
## Задание 8.3: Утилиты для работы с файлами
### Постановка задачи
Создайте библиотеку файловых утилит.
### Структура проекта
fileutils/
├── include/
│   ├── path.h          # Работа с путями
│   └── io.h            # Ввод-вывод
├── src/
│   ├── path.c
│   ├── io.c
│   └── main.c
├── tests/
│   └── test_fileutils.c
├── Makefile
└── meson.build
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| path_join | char* (*)(const char*, const char*) | Объединение путей |
| file_read_all | char* (*)(const char*, size_t*) | Чтение файла |
| file_copy | int (*)(const char*, const char*) | Копирование файла |
### Код программы
include/path.h:
```c
#ifndef PATH_H
#define PATH_H

#include <stdbool.h>
#include <stddef.h>

// Разделитель пути для разных ОС
#ifdef _WIN32
    #define PATH_SEP '\\'
#else
    #define PATH_SEP '/'
#endif

// Функции для работы с путями
char* path_join(const char *base, const char *name);
char* path_dirname(const char *path);
char* path_basename(const char *path);
char* path_extension(const char *path);
char* path_without_extension(const char *path);
char* path_normalize(const char *path);
bool path_is_absolute(const char *path);
bool path_exists(const char *path);
bool path_is_file(const char *path);
bool path_is_directory(const char *path);

#endif // PATH_H
```
include/io.h:
```c
#ifndef IO_H
#define IO_H

#include <stddef.h>

// Чтение файла целиком
char* file_read_all(const char *filename, size_t *size);

// Запись в файл
int file_write_all(const char *filename, const char *content);
int file_append(const char *filename, const char *content);

// Построчное чтение
char** file_read_lines(const char *filename, size_t *count);
void file_free_lines(char **lines, size_t count);

// Копирование и перемещение
int file_copy(const char *src, const char *dst);
int file_move(const char *src, const char *dst);
int file_delete(const char *path);

// Информация о файле
long file_size(const char *filename);
int file_exists(const char *filename);

#endif // IO_H
```
include/fileutils.h:
```c
#ifndef FILEUTILS_H
#define FILEUTILS_H

#include "path.h"
#include "io.h"

#endif // FILEUTILS_H
```
src/path.c:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "path.h"

// Объединение путей
char* path_join(const char *base, const char *name) {
    if (base == NULL || name == NULL) return NULL;
    
    size_t base_len = strlen(base);
    size_t name_len = strlen(name);
    
    // Проверяем, нужно ли добавлять разделитель
    int need_sep = 0;
    if (base_len > 0 && base[base_len - 1] != PATH_SEP && name[0] != PATH_SEP) {
        need_sep = 1;
    }
    
    size_t result_len = base_len + name_len + need_sep + 1;
    char *result = malloc(result_len);
    
    strcpy(result, base);
    if (need_sep) {
        result[base_len] = PATH_SEP;
        result[base_len + 1] = '\0';
        strcat(result, name);
    } else {
        strcat(result, name);
    }
    
    return result;
}

// Директория пути (удаляет последний компонент)
char* path_dirname(const char *path) {
    if (path == NULL) return NULL;
    
    char *result = strdup(path);
    char *last_sep = strrchr(result, PATH_SEP);
    
    if (last_sep == NULL) {
        strcpy(result, ".");
    } else {
        *last_sep = '\0';
        if (last_sep == result) {
            strcpy(result, "/");
        }
    }
    
    return result;
}

// Имя файла (последний компонент пути)
char* path_basename(const char *path) {
    if (path == NULL) return NULL;
    
    const char *last_sep = strrchr(path, PATH_SEP);
    if (last_sep == NULL) {
        return strdup(path);
    } else {
        return strdup(last_sep + 1);
    }
}

// Расширение файла
char* path_extension(const char *path) {
    if (path == NULL) return NULL;
    
    char *basename = path_basename(path);
    char *last_dot = strrchr(basename, '.');
    
    if (last_dot == NULL || last_dot == basename) {
        free(basename);
        return strdup("");
    }
    
    char *ext = strdup(last_dot + 1);
    free(basename);
    return ext;
}

// Путь без расширения
char* path_without_extension(const char *path) {
    if (path == NULL) return NULL;
    
    char *result = strdup(path);
    char *last_dot = strrchr(result, '.');
    
    if (last_dot != NULL && last_dot != result) {
        *last_dot = '\0';
    }
    
    return result;
}

// Нормализация пути (удаление лишних разделителей)
char* path_normalize(const char *path) {
    if (path == NULL) return NULL;
    
    size_t len = strlen(path);
    char *result = malloc(len + 2);
    size_t pos = 0;
    
    for (size_t i = 0; i < len; i++) {
        if (path[i] == PATH_SEP && pos > 0 && result[pos - 1] == PATH_SEP) {
            continue;
        }
        result[pos++] = path[i];
    }
    
    if (pos > 1 && result[pos - 1] == PATH_SEP) {
        pos--;
    }
    
    result[pos] = '\0';
    return result;
}

// Проверка, является ли путь абсолютным
bool path_is_absolute(const char *path) {
    if (path == NULL) return false;
    
#ifdef _WIN32
    return (path[0] != '\0' && path[1] == ':' && path[2] == '\\') ||
           (path[0] == '\\' && path[1] == '\\');
#else
    return path[0] == '/';
#endif
}

// Проверка существования пути
bool path_exists(const char *path) {
    if (path == NULL) return false;
    
    struct stat st;
    return stat(path, &st) == 0;
}

// Проверка, является ли путь файлом
bool path_is_file(const char *path) {
    if (path == NULL) return false;
    
    struct stat st;
    if (stat(path, &st) != 0) return false;
    return S_ISREG(st.st_mode);
}

// Проверка, является ли путь директорией
bool path_is_directory(const char *path) {
    if (path == NULL) return false;
    
    struct stat st;
    if (stat(path, &st) != 0) return false;
    return S_ISDIR(st.st_mode);
}
```
src/io.c:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "io.h"

// Чтение файла целиком
char* file_read_all(const char *filename, size_t *size) {
    if (filename == NULL) return NULL;
    
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return NULL;
    }
    
    // Определяем размер файла
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    if (file_size < 0) {
        fclose(file);
        return NULL;
    }
    
    char *content = malloc(file_size + 1);
    if (content == NULL) {
        fclose(file);
        return NULL;
    }
    
    size_t bytes_read = fread(content, 1, file_size, file);
    content[bytes_read] = '\0';
    
    if (size != NULL) {
        *size = bytes_read;
    }
    
    fclose(file);
    return content;
}

// Запись в файл
int file_write_all(const char *filename, const char *content) {
    if (filename == NULL || content == NULL) return -1;
    
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Ошибка открытия файла для записи");
        return -1;
    }
    
    size_t len = strlen(content);
    size_t written = fwrite(content, 1, len, file);
    
    fclose(file);
    
    return (written == len) ? 0 : -1;
}

// Добавление в конец файла
int file_append(const char *filename, const char *content) {
    if (filename == NULL || content == NULL) return -1;
    
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Ошибка открытия файла для добавления");
        return -1;
    }
    
    size_t len = strlen(content);
    size_t written = fwrite(content, 1, len, file);
    
    fclose(file);
    
    return (written == len) ? 0 : -1;
}

// Построчное чтение файла
char** file_read_lines(const char *filename, size_t *count) {
    if (filename == NULL || count == NULL) return NULL;
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return NULL;
    }
    
    char **lines = NULL;
    *count = 0;
    char buffer[4096];
    
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Удаляем символ новой строки
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        
        lines = realloc(lines, (*count + 1) * sizeof(char*));
        lines[*count] = malloc(len + 1);
        strcpy(lines[*count], buffer);
        (*count)++;
    }
    
    fclose(file);
    return lines;
}

// Освобождение массива строк
void file_free_lines(char **lines, size_t count) {
    if (lines == NULL) return;
    
    for (size_t i = 0; i < count; i++) {
        free(lines[i]);
    }
    free(lines);
}

// Копирование файла
int file_copy(const char *src, const char *dst) {
    if (src == NULL || dst == NULL) return -1;
    
    FILE *src_file = fopen(src, "rb");
    if (src_file == NULL) {
        perror("Ошибка открытия исходного файла");
        return -1;
    }
    
    FILE *dst_file = fopen(dst, "wb");
    if (dst_file == NULL) {
        perror("Ошибка открытия целевого файла");
        fclose(src_file);
        return -1;
    }
    
    char buffer[8192];
    size_t bytes;
    
    while ((bytes = fread(buffer, 1, sizeof(buffer), src_file)) > 0) {
        fwrite(buffer, 1, bytes, dst_file);
    }
    
    fclose(src_file);
    fclose(dst_file);
    
    return 0;
}

// Перемещение файла
int file_move(const char *src, const char *dst) {
    if (src == NULL || dst == NULL) return -1;
    
    // Пробуем переименовать
    if (rename(src, dst) == 0) {
        return 0;
    }
    
    // Если не получилось, копируем и удаляем
    if (file_copy(src, dst) == 0) {
        file_delete(src);
        return 0;
    }
    
    return -1;
}

// Удаление файла
int file_delete(const char *path) {
    if (path == NULL) return -1;
    
    if (remove(path) == 0) {
        return 0;
    } else {
        perror("Ошибка удаления файла");
        return -1;
    }
}

// Размер файла
long file_size(const char *filename) {
    if (filename == NULL) return -1;
    
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return -1;
    }
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);
    
    return size;
}

// Проверка существования файла
int file_exists(const char *filename) {
    if (filename == NULL) return 0;
    
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}
```
src/main.c:
```c
#include <stdio.h>
#include <stdlib.h>
#include "fileutils.h"

int main(void) {
    printf("=== Утилиты для работы с файлами ===\n\n");
    
    // ========== Работа с путями ==========
    printf("--- Работа с путями ---\n");
    
    char *joined = path_join("/home/user", "docs/file.txt");
    printf("path_join: %s\n", joined);
    free(joined);
    
    char *dirname = path_dirname("/home/user/docs/file.txt");
    printf("path_dirname: %s\n", dirname);
    free(dirname);
    
    char *basename = path_basename("/home/user/docs/file.txt");
    printf("path_basename: %s\n", basename);
    free(basename);
    
    char *ext = path_extension("image.jpg");
    printf("path_extension: %s\n", ext);
    free(ext);
    
    char *without_ext = path_without_extension("document.txt");
    printf("path_without_extension: %s\n", without_ext);
    free(without_ext);
    
    char *normalized = path_normalize("home//user///docs//file.txt");
    printf("path_normalize: %s\n", normalized);
    free(normalized);
    
    printf("\n");
    
    // ========== Работа с файлами ==========
    printf("--- Работа с файлами ---\n");
    
    // Создаём тестовый файл
    const char *test_file = "test_files/sample.txt";
    const char *content = "Первая строка\nВторая строка\nТретья строка\n";
    
    if (file_write_all(test_file, content) == 0) {
        printf("Файл создан: %s\n", test_file);
    }
    
    // Чтение файла целиком
    size_t size;
    char *file_content = file_read_all(test_file, &size);
    if (file_content) {
        printf("Размер файла: %zu байт\n", size);
        printf("Содержимое:\n%s", file_content);
        free(file_content);
    }
    
    // Построчное чтение
    size_t line_count;
    char **lines = file_read_lines(test_file, &line_count);
    printf("\nПострочное чтение (%zu строк):\n", line_count);
    for (size_t i = 0; i < line_count; i++) {
        printf("  %zu: %s\n", i + 1, lines[i]);
    }
    file_free_lines(lines, line_count);
    
    // Добавление в файл
    file_append(test_file, "Добавленная строка\n");
    printf("\nПосле добавления строки:\n");
    file_content = file_read_all(test_file, NULL);
    printf("%s", file_content);
    free(file_content);
    
    // Информация о файле
    printf("\nИнформация о файле:\n");
    printf("  Существует: %s\n", file_exists(test_file) ? "да" : "нет");
    printf("  Размер: %ld байт\n", file_size(test_file));
    
    // Копирование файла
    const char *copy_file = "test_files/copy.txt";
    if (file_copy(test_file, copy_file) == 0) {
        printf("\nФайл скопирован в %s\n", copy_file);
    }
    
    // Проверка типов
    printf("\nПроверка типов:\n");
    printf("  %s - файл: %s\n", test_file, path_is_file(test_file) ? "да" : "нет");
    printf("  %s - директория: %s\n", ".", path_is_directory(".") ? "да" : "нет");
    
    // Очистка тестовых файлов
    file_delete(copy_file);
    
    printf("\nРабота программы завершена!\n");
    
    return 0;
}
```
Makefile:
```c
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -I include
CFLAGS_DEBUG = -g -O0
CFLAGS_RELEASE = -O2 -DNDEBUG

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
LIB_OBJS = $(filter-out $(OBJ_DIR)/main.o,$(OBJS))
TARGET = $(BIN_DIR)/fileutils_demo
LIB = $(BIN_DIR)/libfileutils.a

.PHONY: all clean lib demo test

all: lib demo

lib: $(LIB)

demo: $(TARGET)

$(LIB): $(LIB_OBJS) | $(BIN_DIR)
	ar rcs $@ $^

$(TARGET): $(OBJ_DIR)/main.o $(LIB) | $(BIN_DIR)
	$(CC) $< -L$(BIN_DIR) -lfileutils -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

test: $(BIN_DIR)/test_fileutils
	./$(BIN_DIR)/test_fileutils

$(BIN_DIR)/test_fileutils: tests/test_fileutils.c $(LIB) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) $< -L$(BIN_DIR) -lfileutils -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) test_files/
```
tests/test_fileutils.c:
```c
#include <stdio.h>
#include <string.h>
#include "fileutils.h"

static int _test_failed = 0;

#define ASSERT_TRUE(cond) \
    do { \
        if (!(cond)) { \
            printf("  FAILED: %s:%d\n", __FILE__, __LINE__); \
            _test_failed = 1; \
        } \
    } while(0)

#define ASSERT_EQ(a, b) \
    do { \
        if ((a) != (b)) { \
            printf("  FAILED: %s:%d: %ld != %ld\n", __FILE__, __LINE__, (long)(a), (long)(b)); \
            _test_failed = 1; \
        } \
    } while(0)

#define ASSERT_STR_EQ(a, b) \
    do { \
        if (strcmp((a), (b)) != 0) { \
            printf("  FAILED: %s:%d: \"%s\" != \"%s\"\n", __FILE__, __LINE__, (a), (b)); \
            _test_failed = 1; \
        } \
    } while(0)

void test_path_join(void) {
    printf("  test_path_join: ");
    char *p = path_join("home", "user");
    ASSERT_STR_EQ(p, "home/user");
    free(p);
    
    p = path_join("home/", "user");
    ASSERT_STR_EQ(p, "home/user");
    free(p);
    printf("PASSED\n");
}

void test_path_basename(void) {
    printf("  test_path_basename: ");
    char *b = path_basename("/home/user/file.txt");
    ASSERT_STR_EQ(b, "file.txt");
    free(b);
    printf("PASSED\n");
}

void test_path_extension(void) {
    printf("  test_path_extension: ");
    char *e = path_extension("image.png");
    ASSERT_STR_EQ(e, "png");
    free(e);
    printf("PASSED\n");
}

void test_file_write_read(void) {
    printf("  test_file_write_read: ");
    const char *test_file = "test_files/test.txt";
    const char *content = "Hello, World!";
    
    file_write_all(test_file, content);
    char *read_content = file_read_all(test_file, NULL);
    
    ASSERT_STR_EQ(read_content, content);
    free(read_content);
    printf("PASSED\n");
}

void test_file_append(void) {
    printf("  test_file_append: ");
    const char *test_file = "test_files/append.txt";
    
    file_write_all(test_file, "Hello");
    file_append(test_file, ", World!");
    
    char *content = file_read_all(test_file, NULL);
    ASSERT_STR_EQ(content, "Hello, World!");
    free(content);
    printf("PASSED\n");
}

void test_file_copy(void) {
    printf("  test_file_copy: ");
    const char *src = "test_files/source.txt";
    const char *dst = "test_files/dest.txt";
    
    file_write_all(src, "Copy test");
    file_copy(src, dst);
    
    ASSERT_TRUE(file_exists(dst));
    printf("PASSED\n");
}

void test_file_size(void) {
    printf("  test_file_size: ");
    const char *test_file = "test_files/size.txt";
    const char *content = "12345";
    
    file_write_all(test_file, content);
    long size = file_size(test_file);
    
    ASSERT_EQ(size, 5);
    printf("PASSED\n");
}

int main(void) {
    // Создаём папку для тестов
    system("mkdir -p test_files");
    
    printf("\n=== Тестирование fileutils ===\n\n");
    
    test_path_join();
    test_path_basename();
    test_path_extension();
    test_file_write_read();
    test_file_append();
    test_file_copy();
    test_file_size();
    
    printf("\n=== Результат ===\n");
    if (_test_failed) {
        printf("❌ Некоторые тесты не пройдены!\n");
        return 1;
    } else {
        printf("✅ Все тесты пройдены успешно!\n");
        return 0;
    }
}
```
### Результаты работы
![Результат выполнения](practice-8/screenshots/task-8-3.png)
---
## Задание 8.4: Простой HTTP-парсер
### Постановка задачи
Создайте библиотеку для парсинга HTTP запросов/ответов.
### Структура проекта
httplib/
├── include/
│   ├── http_request.h
│   └── http_response.h
├── src/
│   ├── http_request.c
│   ├── http_response.c
│   └── main.c
├── tests/
│   └── test_httplib.c
├── Makefile
└── meson.build
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| HttpMethod | enum | HTTP методы |
| HttpRequest | struct | HTTP запрос |
| HttpResponse | struct | HTTP ответ |
| http_request_parse | HttpRequest* (*)(const char*) | Парсинг запроса |
### Код программы
include/http_request.h:
```c
#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <stddef.h>

// HTTP методы
typedef enum {
    HTTP_GET,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
    HTTP_HEAD,
    HTTP_OPTIONS,
    HTTP_UNKNOWN
} HttpMethod;

// HTTP заголовок
typedef struct {
    char name[64];
    char value[256];
} HttpHeader;

// HTTP запрос
typedef struct {
    HttpMethod method;
    char path[256];
    char version[16];
    HttpHeader headers[50];
    int header_count;
    char *body;
    size_t body_length;
} HttpRequest;

// Парсинг запроса
HttpRequest* http_request_parse(const char *raw);
void http_request_free(HttpRequest *req);

// Создание запроса
HttpRequest* http_request_create(HttpMethod method, const char *path);
void http_request_add_header(HttpRequest *req, const char *name, const char *value);
void http_request_set_body(HttpRequest *req, const char *body);

// Сериализация
char* http_request_to_string(const HttpRequest *req);

// Утилиты
const char* http_method_to_string(HttpMethod method);
HttpMethod http_method_from_string(const char *str);
const char* http_request_get_header(const HttpRequest *req, const char *name);

#endif // HTTP_REQUEST_H
```
include/http_response.h:
```c
#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <stddef.h>

// HTTP статус коды
typedef enum {
    HTTP_200_OK = 200,
    HTTP_201_CREATED = 201,
    HTTP_204_NO_CONTENT = 204,
    HTTP_301_MOVED_PERMANENTLY = 301,
    HTTP_302_FOUND = 302,
    HTTP_400_BAD_REQUEST = 400,
    HTTP_401_UNAUTHORIZED = 401,
    HTTP_403_FORBIDDEN = 403,
    HTTP_404_NOT_FOUND = 404,
    HTTP_500_INTERNAL_SERVER_ERROR = 500
} HttpStatus;

// HTTP заголовок
typedef struct {
    char name[64];
    char value[256];
} HttpResponseHeader;

// HTTP ответ
typedef struct {
    char version[16];
    HttpStatus status;
    char status_text[64];
    HttpResponseHeader headers[50];
    int header_count;
    char *body;
    size_t body_length;
} HttpResponse;

// Парсинг ответа
HttpResponse* http_response_parse(const char *raw);
void http_response_free(HttpResponse *res);

// Создание ответа
HttpResponse* http_response_create(HttpStatus status);
void http_response_add_header(HttpResponse *res, const char *name, const char *value);
void http_response_set_body(HttpResponse *res, const char *body);

// Сериализация
char* http_response_to_string(const HttpResponse *res);

// Утилиты
const char* http_status_to_string(HttpStatus status);
const char* http_response_get_header(const HttpResponse *res, const char *name);

#endif // HTTP_RESPONSE_H
```
src/http_request.c:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "http_request.h"

// Преобразование метода в строку
const char* http_method_to_string(HttpMethod method) {
    switch (method) {
        case HTTP_GET:     return "GET";
        case HTTP_POST:    return "POST";
        case HTTP_PUT:     return "PUT";
        case HTTP_DELETE:  return "DELETE";
        case HTTP_HEAD:    return "HEAD";
        case HTTP_OPTIONS: return "OPTIONS";
        default:           return "UNKNOWN";
    }
}

// Преобразование строки в метод
HttpMethod http_method_from_string(const char *str) {
    if (str == NULL) return HTTP_UNKNOWN;
    
    if (strcmp(str, "GET") == 0)     return HTTP_GET;
    if (strcmp(str, "POST") == 0)    return HTTP_POST;
    if (strcmp(str, "PUT") == 0)     return HTTP_PUT;
    if (strcmp(str, "DELETE") == 0)  return HTTP_DELETE;
    if (strcmp(str, "HEAD") == 0)    return HTTP_HEAD;
    if (strcmp(str, "OPTIONS") == 0) return HTTP_OPTIONS;
    
    return HTTP_UNKNOWN;
}

// Создание нового запроса
HttpRequest* http_request_create(HttpMethod method, const char *path) {
    HttpRequest *req = malloc(sizeof(HttpRequest));
    if (req == NULL) return NULL;
    
    req->method = method;
    strcpy(req->path, path);
    strcpy(req->version, "HTTP/1.1");
    req->header_count = 0;
    req->body = NULL;
    req->body_length = 0;
    
    return req;
}

// Добавление заголовка
void http_request_add_header(HttpRequest *req, const char *name, const char *value) {
    if (req == NULL || name == NULL || value == NULL) return;
    if (req->header_count >= 50) return;
    
    HttpHeader *header = &req->headers[req->header_count++];
    strcpy(header->name, name);
    strcpy(header->value, value);
}

// Установка тела запроса
void http_request_set_body(HttpRequest *req, const char *body) {
    if (req == NULL) return;
    
    if (req->body != NULL) {
        free(req->body);
    }
    
    if (body != NULL) {
        req->body_length = strlen(body);
        req->body = malloc(req->body_length + 1);
        strcpy(req->body, body);
    } else {
        req->body = NULL;
        req->body_length = 0;
    }
}

// Получение заголовка
const char* http_request_get_header(const HttpRequest *req, const char *name) {
    if (req == NULL || name == NULL) return NULL;
    
    for (int i = 0; i < req->header_count; i++) {
        if (strcasecmp(req->headers[i].name, name) == 0) {
            return req->headers[i].value;
        }
    }
    return NULL;
}

// Парсинг HTTP запроса
HttpRequest* http_request_parse(const char *raw) {
    if (raw == NULL) return NULL;
    
    HttpRequest *req = malloc(sizeof(HttpRequest));
    if (req == NULL) return NULL;
    
    req->header_count = 0;
    req->body = NULL;
    req->body_length = 0;
    
    char *raw_copy = strdup(raw);
    char *line = strtok(raw_copy, "\r\n");
    
    // Парсинг стартовой строки
    if (line == NULL) {
        free(raw_copy);
        free(req);
        return NULL;
    }
    
    char method_str[16], path[256], version[16];
    sscanf(line, "%s %s %s", method_str, path, version);
    
    req->method = http_method_from_string(method_str);
    strcpy(req->path, path);
    strcpy(req->version, version);
    
    // Парсинг заголовков
    while ((line = strtok(NULL, "\r\n")) != NULL) {
        if (strlen(line) == 0) break;
        
        char *colon = strchr(line, ':');
        if (colon != NULL) {
            *colon = '\0';
            char *name = line;
            char *value = colon + 1;
            
            // Удаляем пробелы в начале значения
            while (isspace(*value)) value++;
            
            http_request_add_header(req, name, value);
        }
    }
    
    // Парсинг тела (оставшаяся часть)
    char *body_start = strstr(raw, "\r\n\r\n");
    if (body_start != NULL) {
        body_start += 4;
        req->body_length = strlen(body_start);
        req->body = malloc(req->body_length + 1);
        strcpy(req->body, body_start);
    }
    
    free(raw_copy);
    return req;
}

// Сериализация запроса в строку
char* http_request_to_string(const HttpRequest *req) {
    if (req == NULL) return NULL;
    
    size_t size = 256;
    char *buffer = malloc(size);
    if (buffer == NULL) return NULL;
    
    // Стартовая строка
    snprintf(buffer, size, "%s %s %s\r\n",
             http_method_to_string(req->method), req->path, req->version);
    
    // Заголовки
    for (int i = 0; i < req->header_count; i++) {
        char header_line[512];
        snprintf(header_line, sizeof(header_line), "%s: %s\r\n",
                 req->headers[i].name, req->headers[i].value);
        
        size_t new_size = strlen(buffer) + strlen(header_line) + 1;
        buffer = realloc(buffer, new_size);
        strcat(buffer, header_line);
    }
    
    // Разделитель
    strcat(buffer, "\r\n");
    
    // Тело
    if (req->body != NULL && req->body_length > 0) {
        size_t new_size = strlen(buffer) + req->body_length + 1;
        buffer = realloc(buffer, new_size);
        strcat(buffer, req->body);
    }
    
    return buffer;
}

// Освобождение памяти
void http_request_free(HttpRequest *req) {
    if (req == NULL) return;
    
    if (req->body != NULL) {
        free(req->body);
    }
    free(req);
}
```
src/http_response.c:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "http_response.h"

// Преобразование статуса в строку
const char* http_status_to_string(HttpStatus status) {
    switch (status) {
        case HTTP_200_OK: return "OK";
        case HTTP_201_CREATED: return "Created";
        case HTTP_204_NO_CONTENT: return "No Content";
        case HTTP_301_MOVED_PERMANENTLY: return "Moved Permanently";
        case HTTP_302_FOUND: return "Found";
        case HTTP_400_BAD_REQUEST: return "Bad Request";
        case HTTP_401_UNAUTHORIZED: return "Unauthorized";
        case HTTP_403_FORBIDDEN: return "Forbidden";
        case HTTP_404_NOT_FOUND: return "Not Found";
        case HTTP_500_INTERNAL_SERVER_ERROR: return "Internal Server Error";
        default: return "Unknown";
    }
}

// Создание нового ответа
HttpResponse* http_response_create(HttpStatus status) {
    HttpResponse *res = malloc(sizeof(HttpResponse));
    if (res == NULL) return NULL;
    
    strcpy(res->version, "HTTP/1.1");
    res->status = status;
    strcpy(res->status_text, http_status_to_string(status));
    res->header_count = 0;
    res->body = NULL;
    res->body_length = 0;
    
    return res;
}

// Добавление заголовка
void http_response_add_header(HttpResponse *res, const char *name, const char *value) {
    if (res == NULL || name == NULL || value == NULL) return;
    if (res->header_count >= 50) return;
    
    HttpResponseHeader *header = &res->headers[res->header_count++];
    strcpy(header->name, name);
    strcpy(header->value, value);
}

// Установка тела ответа
void http_response_set_body(HttpResponse *res, const char *body) {
    if (res == NULL) return;
    
    if (res->body != NULL) {
        free(res->body);
    }
    
    if (body != NULL) {
        res->body_length = strlen(body);
        res->body = malloc(res->body_length + 1);
        strcpy(res->body, body);
        
        // Добавляем заголовок Content-Length
        char len_str[32];
        snprintf(len_str, sizeof(len_str), "%zu", res->body_length);
        http_response_add_header(res, "Content-Length", len_str);
    } else {
        res->body = NULL;
        res->body_length = 0;
    }
}

// Получение заголовка
const char* http_response_get_header(const HttpResponse *res, const char *name) {
    if (res == NULL || name == NULL) return NULL;
    
    for (int i = 0; i < res->header_count; i++) {
        if (strcasecmp(res->headers[i].name, name) == 0) {
            return res->headers[i].value;
        }
    }
    return NULL;
}

// Парсинг HTTP ответа
HttpResponse* http_response_parse(const char *raw) {
    if (raw == NULL) return NULL;
    
    HttpResponse *res = malloc(sizeof(HttpResponse));
    if (res == NULL) return NULL;
    
    res->header_count = 0;
    res->body = NULL;
    res->body_length = 0;
    
    char *raw_copy = strdup(raw);
    char *line = strtok(raw_copy, "\r\n");
    
    // Парсинг стартовой строки
    if (line == NULL) {
        free(raw_copy);
        free(res);
        return NULL;
    }
    
    char version[16];
    int status_code;
    char status_text[64];
    sscanf(line, "%s %d %[^\n]", version, &status_code, status_text);
    
    strcpy(res->version, version);
    res->status = (HttpStatus)status_code;
    strcpy(res->status_text, status_text);
    
    // Парсинг заголовков
    while ((line = strtok(NULL, "\r\n")) != NULL) {
        if (strlen(line) == 0) break;
        
        char *colon = strchr(line, ':');
        if (colon != NULL) {
            *colon = '\0';
            char *name = line;
            char *value = colon + 1;
            
            while (isspace(*value)) value++;
            
            HttpResponseHeader *header = &res->headers[res->header_count++];
            strcpy(header->name, name);
            strcpy(header->value, value);
        }
    }
    
    // Парсинг тела
    char *body_start = strstr(raw, "\r\n\r\n");
    if (body_start != NULL) {
        body_start += 4;
        res->body_length = strlen(body_start);
        res->body = malloc(res->body_length + 1);
        strcpy(res->body, body_start);
    }
    
    free(raw_copy);
    return res;
}

// Сериализация ответа в строку
char* http_response_to_string(const HttpResponse *res) {
    if (res == NULL) return NULL;
    
    size_t size = 256;
    char *buffer = malloc(size);
    if (buffer == NULL) return NULL;
    
    // Стартовая строка
    snprintf(buffer, size, "%s %d %s\r\n",
             res->version, res->status, res->status_text);
    
    // Заголовки
    for (int i = 0; i < res->header_count; i++) {
        char header_line[512];
        snprintf(header_line, sizeof(header_line), "%s: %s\r\n",
                 res->headers[i].name, res->headers[i].value);
        
        size_t new_size = strlen(buffer) + strlen(header_line) + 1;
        buffer = realloc(buffer, new_size);
        strcat(buffer, header_line);
    }
    
    // Разделитель
    strcat(buffer, "\r\n");
    
    // Тело
    if (res->body != NULL && res->body_length > 0) {
        size_t new_size = strlen(buffer) + res->body_length + 1;
        buffer = realloc(buffer, new_size);
        strcat(buffer, res->body);
    }
    
    return buffer;
}

// Освобождение памяти
void http_response_free(HttpResponse *res) {
    if (res == NULL) return;
    
    if (res->body != NULL) {
        free(res->body);
    }
    free(res);
}
```
src/main.c:
```c
#include <stdio.h>
#include <stdlib.h>
#include "httplib.h"

int main(void) {
    printf("=== HTTP парсер ===\n\n");
    
    // ========== HTTP ЗАПРОСЫ ==========
    printf("--- HTTP ЗАПРОСЫ ---\n\n");
    
    // Пример 1: Простой GET запрос
    const char *get_request = 
        "GET /index.html HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "User-Agent: Mozilla/5.0\r\n"
        "Accept: text/html\r\n"
        "\r\n";
    
    printf("1. Парсинг GET запроса:\n");
    HttpRequest *req = http_request_parse(get_request);
    if (req) {
        printf("   Метод: %s\n", http_method_to_string(req->method));
        printf("   Путь: %s\n", req->path);
        printf("   Версия: %s\n", req->version);
        printf("   Заголовок Host: %s\n", http_request_get_header(req, "Host"));
        printf("   Заголовок User-Agent: %s\n", http_request_get_header(req, "User-Agent"));
        http_request_free(req);
    }
    
    // Пример 2: POST запрос с телом
    const char *post_request = 
        "POST /api/users HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: 27\r\n"
        "\r\n"
        "{\"name\":\"John\",\"age\":30}";
    
    printf("\n2. Парсинг POST запроса:\n");
    req = http_request_parse(post_request);
    if (req) {
        printf("   Метод: %s\n", http_method_to_string(req->method));
        printf("   Путь: %s\n", req->path);
        printf("   Content-Type: %s\n", http_request_get_header(req, "Content-Type"));
        printf("   Тело: %s\n", req->body);
        http_request_free(req);
    }
    
    // Пример 3: Создание запроса программно
    printf("\n3. Создание запроса программно:\n");
    req = http_request_create(HTTP_POST, "/api/data");
    http_request_add_header(req, "Content-Type", "application/json");
    http_request_add_header(req, "Authorization", "Bearer token123");
    http_request_set_body(req, "{\"key\":\"value\"}");
    
    char *req_str = http_request_to_string(req);
    printf("%s", req_str);
    free(req_str);
    http_request_free(req);
    
    // ========== HTTP ОТВЕТЫ ==========
    printf("\n--- HTTP ОТВЕТЫ ---\n\n");
    
    // Пример 4: Успешный ответ
    const char *ok_response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 15\r\n"
        "\r\n"
        "<h1>Hello!</h1>";
    
    printf("1. Парсинг успешного ответа:\n");
    HttpResponse *res = http_response_parse(ok_response);
    if (res) {
        printf("   Версия: %s\n", res->version);
        printf("   Статус: %d %s\n", res->status, res->status_text);
        printf("   Content-Type: %s\n", http_response_get_header(res, "Content-Type"));
        printf("   Тело: %s\n", res->body);
        http_response_free(res);
    }
    
    // Пример 5: Ответ с ошибкой
    const char *not_found_response = 
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 0\r\n"
        "\r\n";
    
    printf("\n2. Парсинг ответа с ошибкой:\n");
    res = http_response_parse(not_found_response);
    if (res) {
        printf("   Статус: %d %s\n", res->status, res->status_text);
        http_response_free(res);
    }
    
    // Пример 6: Создание ответа программно
    printf("\n3. Создание ответа программно:\n");
    res = http_response_create(HTTP_200_OK);
    http_response_add_header(res, "Content-Type", "application/json");
    http_response_set_body(res, "{\"status\":\"success\"}");
    
    char *res_str = http_response_to_string(res);
    printf("%s", res_str);
    free(res_str);
    http_response_free(res);
    
    return 0;
}
```
Makefile:
```c
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -I include
CFLAGS_DEBUG = -g -O0
CFLAGS_RELEASE = -O2 -DNDEBUG

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
LIB_OBJS = $(filter-out $(OBJ_DIR)/main.o,$(OBJS))
TARGET = $(BIN_DIR)/httplib_demo
LIB = $(BIN_DIR)/libhttplib.a

.PHONY: all clean lib demo test

all: lib demo

lib: $(LIB)

demo: $(TARGET)

$(LIB): $(LIB_OBJS) | $(BIN_DIR)
	ar rcs $@ $^

$(TARGET): $(OBJ_DIR)/main.o $(LIB) | $(BIN_DIR)
	$(CC) $< -L$(BIN_DIR) -lhttplib -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

test: $(BIN_DIR)/test_httplib
	./$(BIN_DIR)/test_httplib

$(BIN_DIR)/test_httplib: tests/test_httplib.c $(LIB) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) $< -L$(BIN_DIR) -lhttplib -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
```
tests/test_httplib.c:
```c
#include <stdio.h>
#include <string.h>
#include "httplib.h"

static int _test_failed = 0;

#define ASSERT_TRUE(cond) \
    do { \
        if (!(cond)) { \
            printf("  FAILED: %s:%d\n", __FILE__, __LINE__); \
            _test_failed = 1; \
        } \
    } while(0)

#define ASSERT_EQ(a, b) \
    do { \
        if ((a) != (b)) { \
            printf("  FAILED: %s:%d: %d != %d\n", __FILE__, __LINE__, (a), (b)); \
            _test_failed = 1; \
        } \
    } while(0)

#define ASSERT_STR_EQ(a, b) \
    do { \
        if (strcmp((a), (b)) != 0) { \
            printf("  FAILED: %s:%d: \"%s\" != \"%s\"\n", __FILE__, __LINE__, (a), (b)); \
            _test_failed = 1; \
        } \
    } while(0)

void test_parse_get_request(void) {
    printf("  test_parse_get_request: ");
    const char *raw = "GET /test.html HTTP/1.1\r\nHost: example.com\r\n\r\n";
    
    HttpRequest *req = http_request_parse(raw);
    ASSERT_TRUE(req != NULL);
    ASSERT_EQ(req->method, HTTP_GET);
    ASSERT_STR_EQ(req->path, "/test.html");
    ASSERT_STR_EQ(http_request_get_header(req, "Host"), "example.com");
    
    http_request_free(req);
    printf("PASSED\n");
}

void test_parse_post_request_with_body(void) {
    printf("  test_parse_post_request_with_body: ");
    const char *raw = "POST /api HTTP/1.1\r\n"
                      "Content-Type: application/json\r\n"
                      "Content-Length: 18\r\n"
                      "\r\n"
                      "{\"name\":\"John\"}";
    
    HttpRequest *req = http_request_parse(raw);
    ASSERT_TRUE(req != NULL);
    ASSERT_EQ(req->method, HTTP_POST);
    ASSERT_STR_EQ(req->body, "{\"name\":\"John\"}");
    
    http_request_free(req);
    printf("PASSED\n");
}

void test_create_request(void) {
    printf("  test_create_request: ");
    HttpRequest *req = http_request_create(HTTP_GET, "/test");
    http_request_add_header(req, "X-Custom", "value");
    
    ASSERT_STR_EQ(req->path, "/test");
    ASSERT_STR_EQ(http_request_get_header(req, "X-Custom"), "value");
    
    char *str = http_request_to_string(req);
    ASSERT_TRUE(strstr(str, "GET /test HTTP/1.1") != NULL);
    ASSERT_TRUE(strstr(str, "X-Custom: value") != NULL);
    
    free(str);
    http_request_free(req);
    printf("PASSED\n");
}

void test_parse_response_200(void) {
    printf("  test_parse_response_200: ");
    const char *raw = "HTTP/1.1 200 OK\r\n"
                      "Content-Type: text/html\r\n"
                      "\r\n"
                      "<h1>OK</h1>";
    
    HttpResponse *res = http_response_parse(raw);
    ASSERT_TRUE(res != NULL);
    ASSERT_EQ(res->status, 200);
    ASSERT_STR_EQ(res->status_text, "OK");
    ASSERT_STR_EQ(res->body, "<h1>OK</h1>");
    
    http_response_free(res);
    printf("PASSED\n");
}

void test_parse_response_404(void) {
    printf("  test_parse_response_404: ");
    const char *raw = "HTTP/1.1 404 Not Found\r\n\r\n";
    
    HttpResponse *res = http_response_parse(raw);
    ASSERT_TRUE(res != NULL);
    ASSERT_EQ(res->status, 404);
    ASSERT_STR_EQ(res->status_text, "Not Found");
    
    http_response_free(res);
    printf("PASSED\n");
}

void test_create_response(void) {
    printf("  test_create_response: ");
    HttpResponse *res = http_response_create(HTTP_200_OK);
    http_response_add_header(res, "X-Custom", "value");
    http_response_set_body(res, "Hello");
    
    ASSERT_EQ(res->status, 200);
    ASSERT_STR_EQ(http_response_get_header(res, "X-Custom"), "value");
    
    char *str = http_response_to_string(res);
    ASSERT_TRUE(strstr(str, "200 OK") != NULL);
    ASSERT_TRUE(strstr(str, "Hello") != NULL);
    
    free(str);
    http_response_free(res);
    printf("PASSED\n");
}

int main(void) {
    printf("\n=== Тестирование httplib ===\n\n");
    
    test_parse_get_request();
    test_parse_post_request_with_body();
    test_create_request();
    test_parse_response_200();
    test_parse_response_404();
    test_create_response();
    
    printf("\n=== Результат ===\n");
    if (_test_failed) {
        printf("❌ Некоторые тесты не пройдены!\n");
        return 1;
    } else {
        printf("✅ Все тесты пройдены успешно!\n");
        return 0;
    }
}
```
### Результаты работы
![Результат выполнения](practice-8/screenshots/task-8-4.png)
---
## Задание 8.5: Мини-тестовый фреймворк
### Постановка задачи
Создайте простой фреймворк для модульного тестирования.
### Структура проекта
minitest/
├── include/
│   └── minitest.h
├── src/
│   ├── minitest.c
│   └── main.c
├── tests/
│   └── test_example.c
├── Makefile
└── meson.build
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| TestSuite | struct | Набор тестов |
| TestCase | struct | Отдельный тест |
| ASSERT_EQ | macro | Проверка равенства |
| ASSERT_STR_EQ | macro | Проверка строк |
### Код программы
include/minitest.h:
```c
#ifndef MINITEST_H
#define MINITEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// ========== СТРУКТУРЫ ДЛЯ ТЕСТОВ ==========

typedef void (*TestFunc)(void);

typedef struct {
    const char *name;
    TestFunc func;
} TestCase;

typedef struct {
    const char *name;
    TestCase *tests;
    int test_count;
    int passed;
    int failed;
} TestSuite;

// ========== МАКРОСЫ ДЛЯ УТВЕРЖДЕНИЙ ==========

// Глобальная переменная для отслеживания неудач
extern int _test_failed;

// Базовое утверждение
#define ASSERT_TRUE(cond) \
    do { \
        if (!(cond)) { \
            printf("  ❌ FAILED: %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            _test_failed = 1; \
            return; \
        } \
    } while(0)

#define ASSERT_FALSE(cond) ASSERT_TRUE(!(cond))

// Сравнение целых чисел
#define ASSERT_EQ(a, b) \
    do { \
        if ((a) != (b)) { \
            printf("  ❌ FAILED: %s:%d: %lld != %lld\n", __FILE__, __LINE__, \
                   (long long)(a), (long long)(b)); \
            _test_failed = 1; \
            return; \
        } \
    } while(0)

#define ASSERT_NEQ(a, b) \
    do { \
        if ((a) == (b)) { \
            printf("  ❌ FAILED: %s:%d: %lld == %lld\n", __FILE__, __LINE__, \
                   (long long)(a), (long long)(b)); \
            _test_failed = 1; \
            return; \
        } \
    } while(0)

// Сравнение строк
#define ASSERT_STR_EQ(a, b) \
    do { \
        if (strcmp((a), (b)) != 0) { \
            printf("  ❌ FAILED: %s:%d: \"%s\" != \"%s\"\n", __FILE__, __LINE__, (a), (b)); \
            _test_failed = 1; \
            return; \
        } \
    } while(0)

#define ASSERT_STR_NEQ(a, b) \
    do { \
        if (strcmp((a), (b)) == 0) { \
            printf("  ❌ FAILED: %s:%d: \"%s\" == \"%s\"\n", __FILE__, __LINE__, (a), (b)); \
            _test_failed = 1; \
            return; \
        } \
    } while(0)

// Сравнение указателей
#define ASSERT_NULL(ptr) ASSERT_TRUE((ptr) == NULL)
#define ASSERT_NOT_NULL(ptr) ASSERT_TRUE((ptr) != NULL)

// Сравнение чисел с плавающей точкой
#define ASSERT_DOUBLE_EQ(a, b, eps) \
    do { \
        double _diff = (a) - (b); \
        if (_diff < 0) _diff = -_diff; \
        if (_diff > (eps)) { \
            printf("  ❌ FAILED: %s:%d: %f != %f (eps=%f)\n", __FILE__, __LINE__, \
                   (double)(a), (double)(b), (eps)); \
            _test_failed = 1; \
            return; \
        } \
    } while(0)

#define ASSERT_FLOAT_EQ(a, b, eps) ASSERT_DOUBLE_EQ(a, b, eps)

// Сравнение массивов
#define ASSERT_ARRAY_EQ(arr1, arr2, len, type) \
    do { \
        for (size_t _i = 0; _i < (len); _i++) { \
            if ((arr1)[_i] != (arr2)[_i]) { \
                printf("  ❌ FAILED: %s:%d: array mismatch at index %zu\n", \
                       __FILE__, __LINE__, _i); \
                _test_failed = 1; \
                return; \
            } \
        } \
    } while(0)

// Проверка на выброс исключения (для C можно использовать флаги)
#define ASSERT_THROWS(expr, expected) \
    do { \
        /* В C нет исключений, этот макрос для совместимости */ \
        (void)(expected); \
        expr; \
    } while(0)

// ========== ФУНКЦИИ ДЛЯ РАБОТЫ С ТЕСТОВЫМИ НАБОРАМИ ==========

// Инициализация тестового набора
void test_suite_init(TestSuite *suite, const char *name);

// Добавление теста в набор
void test_suite_add(TestSuite *suite, const char *name, TestFunc func);

// Запуск всех тестов
void test_suite_run(TestSuite *suite);

// Вывод итоговой статистики
void test_suite_summary(const TestSuite *suite);

// Сброс флага ошибки
void test_reset_failed(void);

// ========== УДОБНЫЕ МАКРОСЫ ==========

// Макрос для добавления теста
#define RUN_TEST(suite, test_func) \
    test_suite_add(&suite, #test_func, test_func)

// Макрос для создания простого теста без набора
#define TEST(name) \
    void name(void); \
    void name(void)

// Макрос для запуска одного теста
#define RUN_SINGLE_TEST(test_func) \
    do { \
        printf("\n=== Запуск теста: %s ===\n", #test_func); \
        _test_failed = 0; \
        test_func(); \
        if (_test_failed) { \
            printf("❌ Тест %s НЕ ПРОЙДЕН\n", #test_func); \
        } else { \
            printf("✅ Тест %s пройден\n", #test_func); \
        } \
    } while(0)

#endif // MINITEST_H
```
src/minitest.c:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minitest.h"

int _test_failed = 0;

void test_suite_init(TestSuite *suite, const char *name) {
    if (suite == NULL) return;
    
    suite->name = name;
    suite->tests = NULL;
    suite->test_count = 0;
    suite->passed = 0;
    suite->failed = 0;
}

void test_suite_add(TestSuite *suite, const char *name, TestFunc func) {
    if (suite == NULL || name == NULL || func == NULL) return;
    
    suite->tests = realloc(suite->tests, (suite->test_count + 1) * sizeof(TestCase));
    suite->tests[suite->test_count].name = name;
    suite->tests[suite->test_count].func = func;
    suite->test_count++;
}

void test_suite_run(TestSuite *suite) {
    if (suite == NULL) return;
    
    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                    ЗАПУСК ТЕСТОВ: %-30s ║\n", suite->name);
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    
    for (int i = 0; i < suite->test_count; i++) {
        printf("\n▶ Тест: %s\n", suite->tests[i].name);
        printf("  ");
        
        _test_failed = 0;
        suite->tests[i].func();
        
        if (_test_failed) {
            suite->failed++;
            printf("  ❌ НЕ ПРОЙДЕН\n");
        } else {
            suite->passed++;
            printf("  ✅ ПРОЙДЕН\n");
        }
    }
}

void test_suite_summary(const TestSuite *suite) {
    if (suite == NULL) return;
    
    int total = suite->passed + suite->failed;
    
    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                    РЕЗУЛЬТАТЫ ТЕСТОВ                          ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║  Набор: %-45s ║\n", suite->name);
    printf("║  ✅ Пройдено: %-38d ║\n", suite->passed);
    printf("║  ❌ Не пройдено: %-35d ║\n", suite->failed);
    printf("║  📊 Всего: %-41d ║\n", total);
    printf("║  📈 Успешность: %-36.1f%% ║\n", 
           total > 0 ? (float)suite->passed / total * 100 : 0);
    printf("╚══════════════════════════════════════════════════════════════╝\n");
}

void test_reset_failed(void) {
    _test_failed = 0;
}
```
src/main.c:
```c
#include <stdio.h>
#include <math.h>
#include "minitest.h"

// ========== ФУНКЦИИ ДЛЯ ТЕСТИРОВАНИЯ ==========

int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }
int multiply(int a, int b) { return a * b; }
double divide(int a, int b) { return b != 0 ? (double)a / b : 0; }
int factorial(int n) {
    if (n < 0) return 0;
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}
int is_even(int n) { return n % 2 == 0; }

// ========== ТЕСТЫ ==========

// Тест для функции сложения
void test_add(void) {
    ASSERT_EQ(add(2, 3), 5);
    ASSERT_EQ(add(-1, 1), 0);
    ASSERT_EQ(add(0, 0), 0);
    ASSERT_EQ(add(-5, -3), -8);
}

// Тест для функции вычитания
void test_subtract(void) {
    ASSERT_EQ(subtract(5, 3), 2);
    ASSERT_EQ(subtract(0, 5), -5);
    ASSERT_EQ(subtract(-2, -3), 1);
}

// Тест для функции умножения
void test_multiply(void) {
    ASSERT_EQ(multiply(2, 3), 6);
    ASSERT_EQ(multiply(-2, 3), -6);
    ASSERT_EQ(multiply(0, 100), 0);
}

// Тест для функции деления
void test_divide(void) {
    ASSERT_DOUBLE_EQ(divide(10, 2), 5.0, 0.0001);
    ASSERT_DOUBLE_EQ(divide(7, 2), 3.5, 0.0001);
    ASSERT_DOUBLE_EQ(divide(5, 0), 0.0, 0.0001);
}

// Тест для факториала
void test_factorial(void) {
    ASSERT_EQ(factorial(0), 1);
    ASSERT_EQ(factorial(1), 1);
    ASSERT_EQ(factorial(5), 120);
    ASSERT_EQ(factorial(10), 3628800);
    ASSERT_EQ(factorial(-5), 0);
}

// Тест для проверки чётности
void test_is_even(void) {
    ASSERT_TRUE(is_even(2));
    ASSERT_TRUE(is_even(0));
    ASSERT_FALSE(is_even(3));
    ASSERT_FALSE(is_even(-1));
}

// Тест со строками
void test_strings(void) {
    ASSERT_STR_EQ("hello", "hello");
    ASSERT_STR_NEQ("hello", "world");
    ASSERT_STR_EQ("", "");
}

// Тест с указателями
void test_pointers(void) {
    int x = 42;
    int *p = &x;
    int *q = NULL;
    
    ASSERT_NOT_NULL(p);
    ASSERT_NULL(q);
}

// ========== ЗАПУСК ТЕСТОВ ==========

int main(void) {
    // Создание тестового набора
    TestSuite suite;
    test_suite_init(&suite, "Моя тестовая библиотека");
    
    // Добавление тестов
    RUN_TEST(suite, test_add);
    RUN_TEST(suite, test_subtract);
    RUN_TEST(suite, test_multiply);
    RUN_TEST(suite, test_divide);
    RUN_TEST(suite, test_factorial);
    RUN_TEST(suite, test_is_even);
    RUN_TEST(suite, test_strings);
    RUN_TEST(suite, test_pointers);
    
    // Запуск тестов
    test_suite_run(&suite);
    
    // Вывод результатов
    test_suite_summary(&suite);
    
    // Запуск одного теста (демонстрация RUN_SINGLE_TEST)
    printf("\n=== Демонстрация RUN_SINGLE_TEST ===\n");
    RUN_SINGLE_TEST(test_add);
    
    return suite.failed > 0 ? 1 : 0;
}
```
Makefile:
```c
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -I include
CFLAGS_DEBUG = -g -O0
CFLAGS_RELEASE = -O2 -DNDEBUG

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
LIB_OBJS = $(filter-out $(OBJ_DIR)/main.o,$(OBJS))
TARGET = $(BIN_DIR)/minitest_demo
LIB = $(BIN_DIR)/libminitest.a

.PHONY: all clean lib demo test

all: lib demo

lib: $(LIB)

demo: $(TARGET)

$(LIB): $(LIB_OBJS) | $(BIN_DIR)
	ar rcs $@ $^

$(TARGET): $(OBJ_DIR)/main.o $(LIB) | $(BIN_DIR)
	$(CC) $< -L$(BIN_DIR) -lminitest -lm -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Пример тестов с использованием библиотеки
test: $(BIN_DIR)/test_example
	./$(BIN_DIR)/test_example

$(BIN_DIR)/test_example: tests/test_example.c $(LIB) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) $< -L$(BIN_DIR) -lminitest -lm -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
```
tests/test_example.c:
```c
#include <stdio.h>
#include <math.h>
#include "minitest.h"

// ========== ТЕСТИРУЕМЫЕ ФУНКЦИИ ==========

double my_sqrt(double x) {
    if (x < 0) return -1;
    if (x == 0) return 0;
    
    double guess = x / 2;
    double prev;
    do {
        prev = guess;
        guess = (guess + x / guess) / 2;
    } while (fabs(guess - prev) > 1e-12);
    
    return guess;
}

int my_gcd(int a, int b) {
    a = abs(a);
    b = abs(b);
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int my_lcm(int a, int b) {
    return a / my_gcd(a, b) * b;
}

// ========== ТЕСТЫ ==========

void test_sqrt(void) {
    ASSERT_DOUBLE_EQ(my_sqrt(4.0), 2.0, 0.0001);
    ASSERT_DOUBLE_EQ(my_sqrt(9.0), 3.0, 0.0001);
    ASSERT_DOUBLE_EQ(my_sqrt(2.0), 1.41421356, 0.0001);
    ASSERT_DOUBLE_EQ(my_sqrt(0.0), 0.0, 0.0001);
    ASSERT_TRUE(my_sqrt(-1.0) < 0);
}

void test_gcd(void) {
    ASSERT_EQ(my_gcd(48, 18), 6);
    ASSERT_EQ(my_gcd(17, 19), 1);
    ASSERT_EQ(my_gcd(100, 25), 25);
    ASSERT_EQ(my_gcd(0, 5), 5);
    ASSERT_EQ(my_gcd(-48, 18), 6);
}

void test_lcm(void) {
    ASSERT_EQ(my_lcm(12, 18), 36);
    ASSERT_EQ(my_lcm(3, 5), 15);
    ASSERT_EQ(my_lcm(6, 8), 24);
}

void test_failing_test(void) {
    // Этот тест специально падает для демонстрации
    ASSERT_EQ(1, 2);
}

// ========== ЗАПУСК ==========

int main(void) {
    TestSuite suite;
    test_suite_init(&suite, "Math Functions Tests");
    
    RUN_TEST(suite, test_sqrt);
    RUN_TEST(suite, test_gcd);
    RUN_TEST(suite, test_lcm);
    // RUN_TEST(suite, test_failing_test);  // Раскомментировать для проверки падающего теста
    
    test_suite_run(&suite);
    test_suite_summary(&suite);
    
    return suite.failed > 0 ? 1 : 0;
}
```
meson.build:
```c
project('minitest', 'c',
    version: '1.0.0',
    default_options: ['warning_level=3', 'c_std=c11']
)

inc = include_directories('include')

lib_src = files('src/minitest.c')

minitest = static_library('minitest', lib_src,
    include_directories: inc
)

executable('minitest_demo', 'src/main.c',
    include_directories: inc,
    link_with: minitest,
    dependencies: meson.get_compiler('c').find_library('m', required: false)
)

test_exe = executable('test_example', 'tests/test_example.c',
    include_directories: inc,
    link_with: minitest,
    dependencies: meson.get_compiler('c').find_library('m', required: false)
)

test('minitest example', test_exe)
```
### Результаты работы
![Результат выполнения](practice-8/screenshots/task-8-5.png)
