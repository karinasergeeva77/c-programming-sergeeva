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
