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
