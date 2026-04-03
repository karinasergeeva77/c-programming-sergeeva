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
