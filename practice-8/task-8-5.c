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
