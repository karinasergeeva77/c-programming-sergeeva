weather.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <sqlite3.h>
#include <cjson/cJSON.h>
#include "http.h"
#include "db.h"

#define DB_FILE "weather.db"
#define WEATHER_API_URL "https://wttr.in/%s?format=j1"

// Функция для кодирования пробелов в URL
char *url_encode(const char *str) {
    if (!str) return NULL;
    size_t len = strlen(str);
    char *encoded = malloc(len * 3 + 1);
    if (!encoded) return NULL;
    
    size_t pos = 0;
    for (size_t i = 0; i < len; i++) {
        if (str[i] == ' ') {
            encoded[pos++] = '%';
            encoded[pos++] = '2';
            encoded[pos++] = '0';
        } else {
            encoded[pos++] = str[i];
        }
    }
    encoded[pos] = '\0';
    return encoded;
}

// Парсинг JSON от wttr.in
int parse_weather_json(const char *json_str, double *temp, double *feels_like, int *humidity,
                       double *wind_speed, char *wind_dir, char *condition) {
    cJSON *root = cJSON_Parse(json_str);
    if (!root) {
        fprintf(stderr, "❌ Ошибка парсинга JSON: %s\n", cJSON_GetErrorPtr());
        return 0;
    }

    cJSON *current_array = cJSON_GetObjectItem(root, "current_condition");
    if (!current_array || !cJSON_IsArray(current_array)) {
        fprintf(stderr, "❌ Ошибка: в JSON нет поля 'current_condition'\n");
        cJSON_Delete(root);
        return 0;
    }

    cJSON *current = cJSON_GetArrayItem(current_array, 0);
    if (!current) {
        fprintf(stderr, "❌ Ошибка: массив 'current_condition' пуст\n");
        cJSON_Delete(root);
        return 0;
    }

    cJSON *temp_c = cJSON_GetObjectItem(current, "temp_C");
    if (temp_c) *temp = atof(temp_c->valuestring);

    cJSON *feels_like_c = cJSON_GetObjectItem(current, "FeelsLikeC");
    if (feels_like_c) *feels_like = atof(feels_like_c->valuestring);

    cJSON *humidity_json = cJSON_GetObjectItem(current, "humidity");
    if (humidity_json) *humidity = atoi(humidity_json->valuestring);

    cJSON *wind_speed_json = cJSON_GetObjectItem(current, "windspeedKmph");
    if (wind_speed_json) *wind_speed = atof(wind_speed_json->valuestring);

    cJSON *wind_dir_json = cJSON_GetObjectItem(current, "winddir16Point");
    if (wind_dir_json) strcpy(wind_dir, wind_dir_json->valuestring);

    cJSON *weather_desc_array = cJSON_GetObjectItem(current, "weatherDesc");
    if (weather_desc_array && cJSON_IsArray(weather_desc_array)) {
        cJSON *desc_obj = cJSON_GetArrayItem(weather_desc_array, 0);
        if (desc_obj) {
            cJSON *desc_value = cJSON_GetObjectItem(desc_obj, "value");
            if (desc_value) strcpy(condition, desc_value->valuestring);
        }
    }

    cJSON_Delete(root);
    return 1;
}

void print_current_weather(const char *city, double temp, double feels_like, int humidity,
                           double wind_speed, const char *wind_dir, const char *condition) {
    printf("\n╔══════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                          ПОГОДА В ГОРОДЕ %-32s ║\n", city);
    printf("╠══════════════════════════════════════════════════════════════════════════╣\n");
    printf("║ 🌡️  Температура:     %.1f°C (ощущается как %.1f°C)                      ║\n", temp, feels_like);
    printf("║ 💧 Влажность:       %d%%                                                  ║\n", humidity);
    printf("║ 💨 Ветер:           %.1f км/ч (%s)                                       ║\n", wind_speed, wind_dir);
    printf("║ ☁️  Условие:         %s                                                  ║\n", condition);
    printf("╚══════════════════════════════════════════════════════════════════════════╝\n");
}

void print_usage(const char *prog_name) {
    printf("\n╔══════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                         WEATHER APP v1.0                                 ║\n");
    printf("╠══════════════════════════════════════════════════════════════════════════╣\n");
    printf("║ Использование:                                                           ║\n");
    printf("║   %s fetch <город>     - получить погоду и сохранить в БД                ║\n", prog_name);
    printf("║   %s history           - показать историю запросов                       ║\n", prog_name);
    printf("║   %s last              - показать последнюю запись                       ║\n", prog_name);
    printf("╠══════════════════════════════════════════════════════════════════════════╣\n");
    printf("║ Примеры:                                                                ║\n");
    printf("║   %s fetch Moscow                                                       ║\n", prog_name);
    printf("║   %s fetch \"St Petersburg\"                                              ║\n", prog_name);
    printf("║   %s history                                                            ║\n", prog_name);
    printf("║   %s last                                                               ║\n", prog_name);
    printf("╚══════════════════════════════════════════════════════════════════════════╝\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    // Инициализация базы данных
    sqlite3 *db;
    if (sqlite3_open(DB_FILE, &db) != SQLITE_OK) {
        fprintf(stderr, "❌ Ошибка открытия БД: %s\n", sqlite3_errmsg(db));
        return 1;
    }
    if (!db_init(db)) {
        db_close(db);
        return 1;
    }

    // Инициализация libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);

    int result = 0;

    if (strcmp(argv[1], "fetch") == 0 && argc >= 3) {
        char *city_encoded = url_encode(argv[2]);
        if (!city_encoded) {
            fprintf(stderr, "❌ Ошибка кодирования названия города\n");
            curl_global_cleanup();
            db_close(db);
            return 1;
        }

        char url[512];
        snprintf(url, sizeof(url), WEATHER_API_URL, city_encoded);
        free(city_encoded);

        printf("\n📡 Запрос погоды для города: %s\n", argv[2]);

        HttpResponse resp = {NULL, 0};
        if (http_get(url, &resp)) {
            double temp = 0, feels_like = 0, wind_speed = 0;
            int humidity = 0;
            char wind_dir[20] = "";
            char condition[100] = "";

            if (parse_weather_json(resp.data, &temp, &feels_like, &humidity, 
                                   &wind_speed, wind_dir, condition)) {
                print_current_weather(argv[2], temp, feels_like, humidity, 
                                      wind_speed, wind_dir, condition);
                db_save_weather(db, argv[2], temp, feels_like, humidity, 
                                wind_speed, wind_dir, condition);
            } else {
                fprintf(stderr, "❌ Ошибка при обработке данных от сервера\n");
                result = 1;
            }
        } else {
            fprintf(stderr, "❌ Не удалось выполнить HTTP-запрос\n");
            result = 1;
        }
        http_response_free(&resp);
    } else if (strcmp(argv[1], "history") == 0) {
        db_get_history(db);
    } else if (strcmp(argv[1], "last") == 0) {
        db_get_last(db);
    } else {
        print_usage(argv[0]);
        result = 1;
    }

    curl_global_cleanup();
    db_close(db);
    return result;
}
```
http.h
```c
#ifndef HTTP_H
#define HTTP_H

typedef struct {
    char *data;
    size_t size;
} HttpResponse;

int http_get(const char *url, HttpResponse *resp);
void http_response_free(HttpResponse *resp);

#endif
```
http.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "http.h"

static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total = size * nmemb;
    HttpResponse *resp = (HttpResponse *)userdata;
    
    char *new_data = realloc(resp->data, resp->size + total + 1);
    if (!new_data) return 0;
    
    resp->data = new_data;
    memcpy(resp->data + resp->size, ptr, total);
    resp->size += total;
    resp->data[resp->size] = '\0';
    
    return total;
}

int http_get(const char *url, HttpResponse *resp) {
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    resp->data = malloc(1);
    if (!resp->data) {
        curl_easy_cleanup(curl);
        return 0;
    }
    resp->data[0] = '\0';
    resp->size = 0;
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, resp);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "WeatherApp/1.0 (libcurl)");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
    
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    
    if (res != CURLE_OK) {
        fprintf(stderr, "❌ HTTP error: %s\n", curl_easy_strerror(res));
        free(resp->data);
        resp->data = NULL;
        resp->size = 0;
        return 0;
    }
    return 1;
}

void http_response_free(HttpResponse *resp) {
    if (resp && resp->data) {
        free(resp->data);
        resp->data = NULL;
        resp->size = 0;
    }
}
```
db.h
```c
#ifndef DB_H
#define DB_H

#include <sqlite3.h>

int db_init(sqlite3 *db);
int db_save_weather(sqlite3 *db, const char *city, double temp, double feels_like, 
                    int humidity, double wind_speed, const char *wind_dir, const char *condition);
int db_get_history(sqlite3 *db);
int db_get_last(sqlite3 *db);
void db_close(sqlite3 *db);

#endif
```
db.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "db.h"

int db_init(sqlite3 *db) {
    const char *sql = 
        "CREATE TABLE IF NOT EXISTS weather ("
        " id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " city TEXT NOT NULL,"
        " temperature REAL,"
        " feels_like REAL,"
        " humidity INTEGER,"
        " wind_speed REAL,"
        " wind_dir TEXT,"
        " condition TEXT,"
        " created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";
    
    char *err_msg = NULL;
    int rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "❌ SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 0;
    }
    return 1;
}

int db_save_weather(sqlite3 *db, const char *city, double temp, double feels_like, 
                    int humidity, double wind_speed, const char *wind_dir, const char *condition) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO weather (city, temperature, feels_like, humidity, wind_speed, wind_dir, condition) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?);";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "❌ Prepare error: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    sqlite3_bind_text(stmt, 1, city, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 2, temp);
    sqlite3_bind_double(stmt, 3, feels_like);
    sqlite3_bind_int(stmt, 4, humidity);
    sqlite3_bind_double(stmt, 5, wind_speed);
    sqlite3_bind_text(stmt, 6, wind_dir, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, condition, -1, SQLITE_STATIC);
    
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "❌ Insert error: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    printf("✅ Данные о погоде сохранены (ID: %lld)\n", sqlite3_last_insert_rowid(db));
    return 1;
}

int db_get_history(sqlite3 *db) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id, city, temperature, feels_like, humidity, wind_speed, wind_dir, condition, created_at "
                      "FROM weather ORDER BY created_at DESC LIMIT 10;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "❌ Query error: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    printf("\n╔══════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                              ИСТОРИЯ ЗАПРОСОВ ПОГОДЫ (последние 10)                                         ║\n");
    printf("╠════╦═══════════════╦════════════╦════════════╦════════╦════════════╦════════════╦════════════════════════════╣\n");
    printf("║ ID ║ Город        ║ Температура║ Ощущается  ║ Влажн. ║ Ветер      ║ Направление║ Условие                    ║\n");
    printf("╠════╬═══════════════╬════════════╬════════════╬════════╬════════════╬════════════╬════════════════════════════╣\n");
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *city = (const char *)sqlite3_column_text(stmt, 1);
        double temp = sqlite3_column_double(stmt, 2);
        double feels = sqlite3_column_double(stmt, 3);
        int humidity = sqlite3_column_int(stmt, 4);
        double wind = sqlite3_column_double(stmt, 5);
        const char *wind_dir = (const char *)sqlite3_column_text(stmt, 6);
        const char *cond = (const char *)sqlite3_column_text(stmt, 7);
        
        printf("║ %-2d ║ %-13s ║ %7.1f°C ║ %7.1f°C ║ %5d%% ║ %7.1f ║ %-10s ║ %-26s ║\n",
               id, city, temp, feels, humidity, wind, wind_dir ? wind_dir : "N/A", cond ? cond : "N/A");
    }
    
    printf("╚════╩═══════════════╩════════════╩════════════╩════════╩════════════╩════════════╩════════════════════════════╝\n");
    
    sqlite3_finalize(stmt);
    return 1;
}

int db_get_last(sqlite3 *db) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT city, temperature, feels_like, humidity, wind_speed, wind_dir, condition, created_at "
                      "FROM weather ORDER BY created_at DESC LIMIT 1;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "❌ Query error: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    printf("\n╔══════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                          ПОСЛЕДНЯЯ ЗАПИСЬ                                ║\n");
    printf("╠════════════════╦═════════════════════════════════════════════════════════╣\n");
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *city = (const char *)sqlite3_column_text(stmt, 0);
        double temp = sqlite3_column_double(stmt, 1);
        double feels = sqlite3_column_double(stmt, 2);
        int humidity = sqlite3_column_int(stmt, 3);
        double wind = sqlite3_column_double(stmt, 4);
        const char *wind_dir = (const char *)sqlite3_column_text(stmt, 5);
        const char *cond = (const char *)sqlite3_column_text(stmt, 6);
        const char *date = (const char *)sqlite3_column_text(stmt, 7);
        
        printf("║ Город        ║ %-57s ║\n", city);
        printf("╠════════════════╬═════════════════════════════════════════════════════════╣\n");
        printf("║ Температура   ║ %.1f°C (ощущается как %.1f°C)                          ║\n", temp, feels);
        printf("║ Влажность     ║ %d%%                                                      ║\n", humidity);
        printf("║ Ветер         ║ %.1f км/ч (%s)                                         ║\n", wind, wind_dir);
        printf("║ Условие       ║ %-57s ║\n", cond);
        printf("║ Дата          ║ %-57s ║\n", date);
    } else {
        printf("║ ║ Нет данных                                                           ║\n");
    }
    
    printf("╚════════════════╩═════════════════════════════════════════════════════════╝\n");
    
    sqlite3_finalize(stmt);
    return 1;
}

void db_close(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}
```
Makefile
```c
CC = gcc
CFLAGS = -Wall -Wextra
LIBS = $(shell pkg-config --cflags --libs libcurl sqlite3 libcjson)

TARGET = weather

all: $(TARGET)

$(TARGET): weather.c http.c db.c
	$(CC) $(CFLAGS) weather.c http.c db.c $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET) weather.db

run: $(TARGET)
	./$(TARGET) fetch Moscow

history: $(TARGET)
	./$(TARGET) history

last: $(TARGET)
	./$(TARGET) last

.PHONY: all clean run history last
```
