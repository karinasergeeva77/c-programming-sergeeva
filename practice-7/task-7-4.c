#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/stat.h>

typedef enum {
    LOG_DEBUG = 0,
    LOG_INFO = 1,
    LOG_WARNING = 2,
    LOG_ERROR = 3,
    LOG_FATAL = 4
} LogLevel;

typedef struct {
    FILE *file;
    char filename[256];
    LogLevel min_level;
    size_t max_size;
    int max_files;
    int current_file;
} Logger;

// Функция для создания папки
void create_directory(const char *path) {
    char temp[256];
    char *p;
    
    if (path == NULL) return;
    
    strcpy(temp, path);
    
    // Создаём папки рекурсивно
    for (p = temp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            mkdir(temp, 0755);
            *p = '/';
        }
    }
    mkdir(temp, 0755);
}

const char* level_to_string(LogLevel level) {
    switch (level) {
        case LOG_DEBUG: return "DEBUG";
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        case LOG_FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}

Logger* logger_create(const char *filename, LogLevel min_level, size_t max_size, int max_files) {
    if (filename == NULL) return NULL;
    
    // Создаём папку для лог-файла, если её нет
    char dir_path[256];
    strcpy(dir_path, filename);
    char *last_slash = strrchr(dir_path, '/');
    if (last_slash != NULL) {
        *last_slash = '\0';
        create_directory(dir_path);
    }
    
    Logger *log = malloc(sizeof(Logger));
    if (log == NULL) return NULL;
    
    strcpy(log->filename, filename);
    log->min_level = min_level;
    log->max_size = max_size;
    log->max_files = max_files;
    log->current_file = 1;
    
    log->file = fopen(filename, "a");
    if (log->file == NULL) {
        fprintf(stderr, "Ошибка открытия лог-файла: %s\n", strerror(errno));
        free(log);
        return NULL;
    }
    
    return log;
}

void logger_destroy(Logger *log) {
    if (log == NULL) return;
    
    if (log->file != NULL) {
        fclose(log->file);
    }
    free(log);
}

void logger_rotate(Logger *log) {
    if (log == NULL) return;
    
    fclose(log->file);
    
    // Переименовываем существующие файлы
    for (int i = log->max_files - 1; i > 0; i--) {
        char old_name[512], new_name[512];
        sprintf(old_name, "%s.%d", log->filename, i);
        sprintf(new_name, "%s.%d", log->filename, i + 1);
        rename(old_name, new_name);
    }
    
    char new_name[512];
    sprintf(new_name, "%s.1", log->filename);
    rename(log->filename, new_name);
    
    log->file = fopen(log->filename, "a");
}

void logger_log(Logger *log, LogLevel level, const char *file, int line, 
                const char *func, const char *fmt, ...) {
    if (log == NULL || level < log->min_level) return;
    
    // Проверка размера файла
    fseek(log->file, 0, SEEK_END);
    long size = ftell(log->file);
    if (size > (long)log->max_size) {
        logger_rotate(log);
    }
    
    // Получаем текущее время
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);
    
    // Запись заголовка лога
    fprintf(log->file, "[%s] [%s] [%s:%d:%s] ",
            time_str, level_to_string(level), file, line, func);
    
    // Запись сообщения
    va_list args;
    va_start(args, fmt);
    vfprintf(log->file, fmt, args);
    va_end(args);
    
    fprintf(log->file, "\n");
    fflush(log->file);
}

// Макросы для удобства
#define LOG_DEBUG(log, ...) \
    logger_log(log, LOG_DEBUG, __FILE__, __LINE__, __func__, __VA_ARGS__)

#define LOG_INFO(log, ...) \
    logger_log(log, LOG_INFO, __FILE__, __LINE__, __func__, __VA_ARGS__)

#define LOG_WARNING(log, ...) \
    logger_log(log, LOG_WARNING, __FILE__, __LINE__, __func__, __VA_ARGS__)

#define LOG_ERROR(log, ...) \
    logger_log(log, LOG_ERROR, __FILE__, __LINE__, __func__, __VA_ARGS__)

#define LOG_FATAL(log, ...) \
    logger_log(log, LOG_FATAL, __FILE__, __LINE__, __func__, __VA_ARGS__)



int main(void) {
    Logger *log = logger_create("logs/app.log", LOG_DEBUG, 1024 * 1024, 3);
    if (log == NULL) {
        printf("Ошибка создания логгера\n");
        return 1;
    }
    
    LOG_INFO(log, "Приложение запущено");
    LOG_DEBUG(log, "Загружена конфигурация");
    LOG_WARNING(log, "Соединение с базой данных медленное (1.5s)");
    LOG_ERROR(log, "Ошибка аутентификации пользователя 'admin'");
    LOG_INFO(log, "Приложение завершает работу");
    
    printf("Логи записаны в logs/app.log\n");
    printf("Содержимое лог-файла:\n");
    printf("----------------------------------------\n");
    
    // Вывод содержимого лог-файла
    FILE *log_file = fopen("logs/app.log", "r");
    if (log_file) {
        char line[512];
        while (fgets(line, sizeof(line), log_file)) {
            printf("%s", line);
        }
        fclose(log_file);
    }
    
    logger_destroy(log);
    return 0;
}
