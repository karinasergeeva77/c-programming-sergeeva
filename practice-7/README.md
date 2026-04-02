# Практическая работа 7: Файловый ввод-вывод
**Студент:** Сергеева Карина Алексеевна
**Группа:** 1зб_ИВТ(ускор.)/25
**Дата:** 31 марта 2026 г.
---
## Задание 7.1: Анализатор текстового файла
### Постановка задачи
Создайте программу для анализа текстового файла.
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| stats | FileStats | Структура со статистикой файла |
| filename | const char* | Имя анализируемого файла |
| freq | int[256] | Массив частот символов |
### Код программы
```c
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

// Структуры

typedef struct {
    size_t chars;           // Всего символов
    size_t chars_no_ws;     // Без пробельных
    size_t words;           // Слов
    size_t lines;           // Строк
    size_t empty_lines;     // Пустых строк
    size_t max_line_len;    // Длина самой длинной строки
    char longest_word[100]; // Самое длинное слово
} FileStats;

// Функции 

int analyze_file(const char *filename, FileStats *stats) {
    if (filename == NULL || stats == NULL) return -1;
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return -1;
    }
    
    // Инициализация статистики
    memset(stats, 0, sizeof(FileStats));
    strcpy(stats->longest_word, "");
    
    char line[1024];
    int in_word = 0;
    size_t word_len = 0;
    char current_word[100] = "";
    
    while (fgets(line, sizeof(line), file) != NULL) {
        stats->lines++;
        
        size_t line_len = strlen(line);
        stats->chars += line_len;
        
        // Удаляем символ новой строки для подсчёта длины строки
        if (line_len > 0 && line[line_len - 1] == '\n') {
            line_len--;
        }
        
        if (line_len == 0) {
            stats->empty_lines++;
        }
        
        if (line_len > stats->max_line_len) {
            stats->max_line_len = line_len;
        }
        
        // Анализ строки
        in_word = 0;
        word_len = 0;
        
        for (size_t i = 0; i < line_len; i++) {
            char c = line[i];
            
            // Подсчёт символов без пробелов
            if (!isspace(c)) {
                stats->chars_no_ws++;
            }
            
            // Подсчёт слов и поиск самого длинного
            if (isalpha(c)) {
                if (!in_word) {
                    in_word = 1;
                    stats->words++;
                    word_len = 0;
                    memset(current_word, 0, sizeof(current_word));
                }
                if (word_len < sizeof(current_word) - 1) {
                    current_word[word_len++] = c;
                }
            } else {
                if (in_word) {
                    in_word = 0;
                    current_word[word_len] = '\0';
                    if (word_len > strlen(stats->longest_word)) {
                        strcpy(stats->longest_word, current_word);
                    }
                }
            }
        }
        
        // Проверка слова в конце строки
        if (in_word) {
            current_word[word_len] = '\0';
            if (word_len > strlen(stats->longest_word)) {
                strcpy(stats->longest_word, current_word);
            }
        }
    }
    
    fclose(file);
    return 0;
}

void print_stats(const FileStats *stats) {
    if (stats == NULL) return;
    
    printf("=== Статистика файла ===\n");
    printf("Символов: %zu\n", stats->chars);
    printf("Символов (без пробелов): %zu\n", stats->chars_no_ws);
    printf("Слов: %zu\n", stats->words);
    printf("Строк: %zu\n", stats->lines);
    printf("Пустых строк: %zu\n", stats->empty_lines);
    printf("Максимальная длина строки: %zu\n", stats->max_line_len);
    printf("Самое длинное слово: \"%s\"\n", stats->longest_word);
}

void char_frequency(const char *filename, int freq[256]) {
    if (filename == NULL || freq == NULL) return;
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return;
    }
    
    // Инициализация частот
    for (int i = 0; i < 256; i++) {
        freq[i] = 0;
    }
    
    int c;
    while ((c = fgetc(file)) != EOF) {
        freq[c]++;
    }
    
    fclose(file);
}

void find_word(const char *filename, const char *word) {
    if (filename == NULL || word == NULL) return;
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return;
    }
    
    char line[1024];
    int line_num = 0;
    int found_count = 0;
    
    printf("Поиск слова \"%s\":\n", word);
    
    while (fgets(line, sizeof(line), file) != NULL) {
        line_num++;
        
        if (strstr(line, word) != NULL) {
            printf("Строка %d: %s", line_num, line);
            found_count++;
        }
    }
    
    if (found_count == 0) {
        printf("Слово не найдено\n");
    } else {
        printf("Найдено: %d вхождений\n", found_count);
    }
    
    fclose(file);
}

int count_word(const char *filename, const char *word) {
    if (filename == NULL || word == NULL) return 0;
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return 0;
    }
    
    char line[1024];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL) {
        char *pos = line;
        while ((pos = strstr(pos, word)) != NULL) {
            count++;
            pos++;
        }
    }
    
    fclose(file);
    return count;
}

// Функция MAIN 

int main(void) {
    // Создаём тестовый файл
    FILE *test = fopen("test_files/sample.txt", "w");
    if (test) {
        fprintf(test, "Это первая строка тестового файла.\n");
        fprintf(test, "Вторая строка с числами 123 и словами.\n");
        fprintf(test, "\n");
        fprintf(test, "Четвёртая строка после пустой.\n");
        fprintf(test, "Самое длинное слово в этом файле: программирование\n");
        fclose(test);
    }
    
    // Анализ файла
    FileStats stats;
    if (analyze_file("test_files/sample.txt", &stats) == 0) {
        print_stats(&stats);
    }
    
    printf("\n");
    
    // Частотный анализ
    int freq[256];
    char_frequency("test_files/sample.txt", freq);
    printf("Частота символов:\n");
    for (int i = 'a'; i <= 'z'; i++) {
        if (freq[i] > 0) {
            printf("'%c': %d\n", i, freq[i]);
        }
    }
    
    printf("\n");
    
    // Поиск слова
    find_word("test_files/sample.txt", "строка");
    
    printf("\n");
    
    // Подсчёт вхождений
    int cnt = count_word("test_files/sample.txt", "строка");
    printf("Количество вхождений слова \"строка\": %d\n", cnt);
    
    return 0;
}
```
### Результаты работы
![Результат выполнения](practice-7/screenshots/task-7-1.png)
---
## Задание 7.2: Конфигурационный файл (INI)
### Постановка задачи
Реализуйте парсер и генератор конфигурационных файлов формата INI.
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| cfg | Config | Структура конфигурации |
| section | ConfigSection | Секция конфигурации |
| entry | ConfigEntry | Пара ключ-значение |
### Код программы
```c
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_KEY_LEN 64
#define MAX_VALUE_LEN 256
#define MAX_SECTION_LEN 64
#define MAX_ENTRIES 100
#define MAX_SECTIONS 20

typedef struct {
    char key[MAX_KEY_LEN];
    char value[MAX_VALUE_LEN];
} ConfigEntry;

typedef struct {
    char name[MAX_SECTION_LEN];
    ConfigEntry entries[MAX_ENTRIES];
    int entry_count;
} ConfigSection;

typedef struct {
    ConfigSection sections[MAX_SECTIONS];
    int section_count;
} Config;

// Удаление пробелов в начале и конце строки
void trim(char *str) {
    if (str == NULL) return;
    
    char *start = str;
    while (isspace(*start)) start++;
    
    char *end = start + strlen(start) - 1;
    while (end > start && isspace(*end)) end--;
    
    *(end + 1) = '\0';
    
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

int config_load(const char *filename, Config *cfg) {
    if (filename == NULL || cfg == NULL) return -1;
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return -1;
    }
    
    // Инициализация конфигурации
    cfg->section_count = 0;
    ConfigSection *current_section = NULL;
    
    char line[512];
    int line_num = 0;
    
    while (fgets(line, sizeof(line), file) != NULL) {
        line_num++;
        
        // Удаляем символ новой строки
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
            len--;
        }
        
        // Пропускаем пустые строки и комментарии
        if (len == 0 || line[0] == ';' || line[0] == '#') {
            continue;
        }
        
        // Проверка на секцию
        if (line[0] == '[') {
            char *end = strchr(line, ']');
            if (end == NULL) {
                fprintf(stderr, "Ошибка в строке %d: некорректная секция\n", line_num);
                fclose(file);
                return -1;
            }
            
            *end = '\0';
            char section_name[MAX_SECTION_LEN];
            strcpy(section_name, line + 1);
            trim(section_name);
            
            // Поиск существующей секции
            current_section = NULL;
            for (int i = 0; i < cfg->section_count; i++) {
                if (strcmp(cfg->sections[i].name, section_name) == 0) {
                    current_section = &cfg->sections[i];
                    break;
                }
            }
            
            // Создание новой секции
            if (current_section == NULL && cfg->section_count < MAX_SECTIONS) {
                current_section = &cfg->sections[cfg->section_count++];
                strcpy(current_section->name, section_name);
                current_section->entry_count = 0;
            }
            
            continue;
        }
        
        // Парсинг ключ=значение
        char *equals = strchr(line, '=');
        if (equals == NULL) {
            fprintf(stderr, "Ошибка в строке %d: ожидается '='\n", line_num);
            fclose(file);
            return -1;
        }
        
        if (current_section == NULL) {
            fprintf(stderr, "Ошибка в строке %d: ключ вне секции\n", line_num);
            fclose(file);
            return -1;
        }
        
        *equals = '\0';
        char key[MAX_KEY_LEN];
        char value[MAX_VALUE_LEN];
        
        strcpy(key, line);
        strcpy(value, equals + 1);
        trim(key);
        trim(value);
        
        if (current_section->entry_count < MAX_ENTRIES) {
            ConfigEntry *entry = &current_section->entries[current_section->entry_count++];
            strcpy(entry->key, key);
            strcpy(entry->value, value);
        } else {
            fprintf(stderr, "Предупреждение: превышено максимальное количество записей\n");
        }
    }
    
    fclose(file);
    return 0;
}

int config_save(const char *filename, const Config *cfg) {
    if (filename == NULL || cfg == NULL) return -1;
    
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Ошибка открытия файла для записи");
        return -1;
    }
    
    for (int i = 0; i < cfg->section_count; i++) {
        fprintf(file, "[%s]\n", cfg->sections[i].name);
        
        for (int j = 0; j < cfg->sections[i].entry_count; j++) {
            fprintf(file, "%s = %s\n", 
                    cfg->sections[i].entries[j].key,
                    cfg->sections[i].entries[j].value);
        }
        
        fprintf(file, "\n");
    }
    
    fclose(file);
    return 0;
}

const char* config_get(const Config *cfg, const char *section, const char *key) {
    if (cfg == NULL || section == NULL || key == NULL) return NULL;
    
    for (int i = 0; i < cfg->section_count; i++) {
        if (strcmp(cfg->sections[i].name, section) == 0) {
            for (int j = 0; j < cfg->sections[i].entry_count; j++) {
                if (strcmp(cfg->sections[i].entries[j].key, key) == 0) {
                    return cfg->sections[i].entries[j].value;
                }
            }
        }
    }
    
    return NULL;
}

int config_set(Config *cfg, const char *section, const char *key, const char *value) {
    if (cfg == NULL || section == NULL || key == NULL || value == NULL) return -1;
    
    // Поиск секции
    ConfigSection *target_section = NULL;
    for (int i = 0; i < cfg->section_count; i++) {
        if (strcmp(cfg->sections[i].name, section) == 0) {
            target_section = &cfg->sections[i];
            break;
        }
    }
    
    // Создание новой секции
    if (target_section == NULL && cfg->section_count < MAX_SECTIONS) {
        target_section = &cfg->sections[cfg->section_count++];
        strcpy(target_section->name, section);
        target_section->entry_count = 0;
    }
    
    if (target_section == NULL) return -1;
    
    // Поиск ключа
    for (int i = 0; i < target_section->entry_count; i++) {
        if (strcmp(target_section->entries[i].key, key) == 0) {
            strcpy(target_section->entries[i].value, value);
            return 0;
        }
    }
    
    // Добавление нового ключа
    if (target_section->entry_count < MAX_ENTRIES) {
        ConfigEntry *entry = &target_section->entries[target_section->entry_count++];
        strcpy(entry->key, key);
        strcpy(entry->value, value);
        return 0;
    }
    
    return -1;
}

// Функция MAIN 

int main(void) {
    // Создание тестового конфигурационного файла
    FILE *test = fopen("test_files/config.ini", "w");
    if (test) {
        fprintf(test, "; Это комментарий\n");
        fprintf(test, "[database]\n");
        fprintf(test, "host = localhost\n");
        fprintf(test, "port = 5432\n");
        fprintf(test, "user = admin\n");
        fprintf(test, "\n");
        fprintf(test, "[logging]\n");
        fprintf(test, "level = INFO\n");
        fprintf(test, "file = app.log\n");
        fclose(test);
    }
    
    Config cfg;
    if (config_load("test_files/config.ini", &cfg) == 0) {
        printf("Конфигурация загружена:\n");
        printf("database.host = %s\n", config_get(&cfg, "database", "host"));
        printf("database.port = %s\n", config_get(&cfg, "database", "port"));
        printf("logging.level = %s\n", config_get(&cfg, "logging", "level"));
        
        // Изменение значения
        config_set(&cfg, "database", "port", "3306");
        
        // Сохранение в новый файл
        config_save("test_files/config_new.ini", &cfg);
        printf("\nКонфигурация сохранена в config_new.ini\n");
    }
    
    return 0;
}
```
### Результаты работы
![Результат выполнения](practice-7/screenshots/task-7-2.png)
---
## Задание 7.3: База данных на файлах
### Постановка задачи
Реализуйте простую базу данных с хранением в бинарном файле.
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| db | Database* | База данных |
| record | Record | Запись |
| header | DBHeader | Заголовок файла |
### Код программы
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define DB_MAGIC 0x44425F31  // "DB_1"

typedef struct {
    int id;
    char name[50];
    char email[100];
    int age;
    double balance;
    int is_active;
} Record;

typedef struct {
    uint32_t magic;
    uint32_t version;
    uint32_t record_count;
    uint32_t record_size;
    uint32_t next_id;
} DBHeader;

typedef struct {
    FILE *file;
    DBHeader header;
    char filename[256];
} Database;

// Открытие/создание базы данных
Database* db_open(const char *filename) {
    if (filename == NULL) return NULL;
    
    Database *db = malloc(sizeof(Database));
    if (db == NULL) return NULL;
    
    strcpy(db->filename, filename);
    
    // Попытка открыть существующий файл
    db->file = fopen(filename, "rb+");
    
    if (db->file == NULL) {
        // Создание новой базы данных
        db->file = fopen(filename, "wb+");
        if (db->file == NULL) {
            free(db);
            perror("Ошибка создания базы данных");
            return NULL;
        }
        
        // Инициализация заголовка
        db->header.magic = DB_MAGIC;
        db->header.version = 1;
        db->header.record_count = 0;
        db->header.record_size = sizeof(Record);
        db->header.next_id = 1;
        
        // Запись заголовка
        fseek(db->file, 0, SEEK_SET);
        fwrite(&db->header, sizeof(DBHeader), 1, db->file);
    } else {
        // Чтение существующего заголовка
        fseek(db->file, 0, SEEK_SET);
        fread(&db->header, sizeof(DBHeader), 1, db->file);
        
        if (db->header.magic != DB_MAGIC) {
            fprintf(stderr, "Некорректный формат файла\n");
            fclose(db->file);
            free(db);
            return NULL;
        }
    }
    
    return db;
}

void db_close(Database *db) {
    if (db == NULL) return;
    
    if (db->file != NULL) {
        fclose(db->file);
    }
    free(db);
}

int db_insert(Database *db, Record *record) {
    if (db == NULL || record == NULL) return -1;
    
    record->id = db->header.next_id++;
    
    fseek(db->file, 0, SEEK_END);
    fwrite(record, sizeof(Record), 1, db->file);
    
    db->header.record_count++;
    
    // Обновляем заголовок
    fseek(db->file, 0, SEEK_SET);
    fwrite(&db->header, sizeof(DBHeader), 1, db->file);
    
    return record->id;
}

Record* db_get(Database *db, int id) {
    if (db == NULL) return NULL;
    
    fseek(db->file, sizeof(DBHeader), SEEK_SET);
    
    Record *record = malloc(sizeof(Record));
    if (record == NULL) return NULL;
    
    while (fread(record, sizeof(Record), 1, db->file) == 1) {
        if (record->id == id) {
            return record;
        }
    }
    
    free(record);
    return NULL;
}

int db_update(Database *db, int id, const Record *record) {
    if (db == NULL || record == NULL) return -1;
    
    long pos = sizeof(DBHeader);
    Record current;
    
    fseek(db->file, pos, SEEK_SET);
    
    while (fread(&current, sizeof(Record), 1, db->file) == 1) {
        if (current.id == id) {
            fseek(db->file, -sizeof(Record), SEEK_CUR);
            fwrite(record, sizeof(Record), 1, db->file);
            return 0;
        }
    }
    
    return -1;
}

int db_delete(Database *db, int id) {
    if (db == NULL) return -1;
    
    long pos = sizeof(DBHeader);
    Record record;
    
    fseek(db->file, pos, SEEK_SET);
    
    while (fread(&record, sizeof(Record), 1, db->file) == 1) {
        if (record.id == id) {
            // Помечаем как удалённую (устанавливаем is_active = 0)
            record.is_active = 0;
            fseek(db->file, -sizeof(Record), SEEK_CUR);
            fwrite(&record, sizeof(Record), 1, db->file);
            return 0;
        }
    }
    
    return -1;
}

void db_print_all(Database *db) {
    if (db == NULL) return;
    
    printf("\n=== База данных %s ===\n", db->filename);
    printf("%-5s | %-25s | %-25s | %-6s | %-10s | %s\n",
           "ID", "Имя", "Email", "Возраст", "Баланс", "Активен");
    printf("-----+---------------------------+---------------------------+--------+------------+-------\n");
    
    fseek(db->file, sizeof(DBHeader), SEEK_SET);
    
    Record record;
    while (fread(&record, sizeof(Record), 1, db->file) == 1) {
        printf("%-5d | %-25s | %-25s | %-6d | %-10.2f | %s\n",
               record.id, record.name, record.email,
               record.age, record.balance,
               record.is_active ? "Да" : "Нет");
    }
    
    printf("Всего записей: %u\n", db->header.record_count);
}

int db_count(Database *db) {
    if (db == NULL) return 0;
    return db->header.record_count;
}

// Функция MAIN

int main(void) {
    Database *db = db_open("data/test.db");
    if (db == NULL) {
        printf("Ошибка открытия базы данных\n");
        return 1;
    }
    
    Record r1 = {0, "Иван Петров", "ivan@email.com", 30, 1500.00, 1};
    Record r2 = {0, "Мария Сидорова", "maria@email.com", 25, 2300.50, 1};
    Record r3 = {0, "Пётр Козлов", "petr@email.com", 35, 800.00, 0};
    
    db_insert(db, &r1);
    db_insert(db, &r2);
    db_insert(db, &r3);
    
    db_print_all(db);
    
    // Обновление записи
    Record updated = {2, "Мария Иванова", "maria.new@email.com", 26, 2500.00, 1};
    db_update(db, 2, &updated);
    
    printf("\nПосле обновления:\n");
    db_print_all(db);
    
    db_close(db);
    
    return 0;
}
```
### Результаты работы
![Результат выполнения](practice-7/screenshots/task-7-3.png)
---
## Задание 7.4: Журнал (лог-файл)
### Постановка задачи
Реализуйте систему логирования с ротацией файлов.
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| log | Logger* | Логгер |
| level | LogLevel | Уровень логирования |
### Код программы
```c
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
```
### Результаты работы
![Результат выполнения](practice-7/screenshots/task-7-4.png)
---
## Задание 7.5: Работа с CSV файлами
### Постановка задачи
Реализуйте парсер и генератор CSV файлов.
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| csv | CSVFile* | CSV файл |
| row | CSVRow | Строка данных |
### Код программы
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_COLUMNS 50
#define MAX_CELL_LEN 256
#define MAX_LINE_LEN 4096

typedef struct {
    char **cells;
    int cell_count;
} CSVRow;

typedef struct {
    CSVRow *rows;
    int row_count;
    int column_count;
    char **headers;
    int has_headers;
} CSVFile;


void trim_whitespace(char *str) {
    if (str == NULL) return;
    
    char *start = str;
    while (isspace(*start)) start++;
    
    char *end = start + strlen(start) - 1;
    while (end > start && isspace(*end)) end--;
    
    *(end + 1) = '\0';
    
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

CSVFile* csv_load(const char *filename, char delimiter, int has_headers) {
    if (filename == NULL) return NULL;
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return NULL;
    }
    
    CSVFile *csv = malloc(sizeof(CSVFile));
    if (csv == NULL) {
        fclose(file);
        return NULL;
    }
    
    csv->rows = NULL;
    csv->row_count = 0;
    csv->column_count = 0;
    csv->headers = NULL;
    csv->has_headers = has_headers;
    
    char line[MAX_LINE_LEN];
    int line_num = 0;
    
    while (fgets(line, sizeof(line), file) != NULL) {
        line_num++;
        
        // Удаляем символ новой строки
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
            len--;
        }
        
        // Пропускаем пустые строки
        if (len == 0) continue;
        
        // Разбор строки
        CSVRow row;
        row.cells = NULL;
        row.cell_count = 0;
        
        char *token = strtok(line, &delimiter);
        while (token != NULL) {
            trim_whitespace(token);
            row.cells = realloc(row.cells, (row.cell_count + 1) * sizeof(char*));
            row.cells[row.cell_count] = malloc(MAX_CELL_LEN);
            strcpy(row.cells[row.cell_count], token);
            row.cell_count++;
            token = strtok(NULL, &delimiter);
        }
        
        // Определение количества колонок
        if (line_num == 1 && has_headers) {
            csv->column_count = row.cell_count;
            csv->headers = malloc(csv->column_count * sizeof(char*));
            for (int i = 0; i < csv->column_count; i++) {
                csv->headers[i] = malloc(MAX_CELL_LEN);
                strcpy(csv->headers[i], row.cells[i]);
                free(row.cells[i]);
            }
            free(row.cells);
        } else {
            csv->rows = realloc(csv->rows, (csv->row_count + 1) * sizeof(CSVRow));
            csv->rows[csv->row_count++] = row;
            
            if (csv->column_count == 0) {
                csv->column_count = row.cell_count;
            }
        }
    }
    
    fclose(file);
    printf("Загружено %d строк, %d колонок\n", csv->row_count, csv->column_count);
    return csv;
}

int csv_save(const CSVFile *csv, const char *filename, char delimiter) {
    if (csv == NULL || filename == NULL) return -1;
    
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Ошибка открытия файла для записи");
        return -1;
    }
    
    // Сохраняем заголовки
    if (csv->has_headers && csv->headers != NULL) {
        for (int i = 0; i < csv->column_count; i++) {
            fprintf(file, "%s", csv->headers[i]);
            if (i < csv->column_count - 1) {
                fprintf(file, "%c", delimiter);
            }
        }
        fprintf(file, "\n");
    }
    
    // Сохраняем строки
    for (int i = 0; i < csv->row_count; i++) {
        for (int j = 0; j < csv->rows[i].cell_count; j++) {
            fprintf(file, "%s", csv->rows[i].cells[j]);
            if (j < csv->rows[i].cell_count - 1) {
                fprintf(file, "%c", delimiter);
            }
        }
        fprintf(file, "\n");
    }
    
    fclose(file);
    return 0;
}


const char* csv_get(const CSVFile *csv, int row, int col) {
    if (csv == NULL || row < 0 || row >= csv->row_count) return NULL;
    if (col < 0 || col >= csv->rows[row].cell_count) return NULL;
    return csv->rows[row].cells[col];
}

const char* csv_get_by_header(const CSVFile *csv, int row, const char *header) {
    if (csv == NULL || row < 0 || row >= csv->row_count || header == NULL) return NULL;
    
    // Находим индекс колонки по заголовку
    int col_index = -1;
    for (int i = 0; i < csv->column_count; i++) {
        if (strcmp(csv->headers[i], header) == 0) {
            col_index = i;
            break;
        }
    }
    
    if (col_index == -1 || col_index >= csv->rows[row].cell_count) return NULL;
    return csv->rows[row].cells[col_index];
}

int csv_set(CSVFile *csv, int row, int col, const char *value) {
    if (csv == NULL || row < 0 || row >= csv->row_count) return -1;
    if (col < 0 || col >= csv->rows[row].cell_count) return -1;
    
    strcpy(csv->rows[row].cells[col], value);
    return 0;
}


int csv_add_row(CSVFile *csv, const char **cells, int count) {
    if (csv == NULL || cells == NULL) return -1;
    
    CSVRow row;
    row.cells = NULL;
    row.cell_count = count;
    
    for (int i = 0; i < count; i++) {
        row.cells = realloc(row.cells, (i + 1) * sizeof(char*));
        row.cells[i] = malloc(MAX_CELL_LEN);
        strcpy(row.cells[i], cells[i]);
    }
    
    csv->rows = realloc(csv->rows, (csv->row_count + 1) * sizeof(CSVRow));
    csv->rows[csv->row_count++] = row;
    
    return csv->row_count - 1;
}

int csv_remove_row(CSVFile *csv, int row) {
    if (csv == NULL || row < 0 || row >= csv->row_count) return -1;
    
    // Освобождаем память удаляемой строки
    for (int i = 0; i < csv->rows[row].cell_count; i++) {
        free(csv->rows[row].cells[i]);
    }
    free(csv->rows[row].cells);
    
    // Сдвигаем строки
    for (int i = row; i < csv->row_count - 1; i++) {
        csv->rows[i] = csv->rows[i + 1];
    }
    
    csv->row_count--;
    return 0;
}



int csv_find_rows(const CSVFile *csv, int column, const char *value, int *indices, int max_results) {
    if (csv == NULL || value == NULL || indices == NULL) return 0;
    if (column < 0 || column >= csv->column_count) return 0;
    
    int found = 0;
    for (int i = 0; i < csv->row_count && found < max_results; i++) {
        if (csv->rows[i].cell_count > column) {
            if (strcmp(csv->rows[i].cells[column], value) == 0) {
                indices[found++] = i;
            }
        }
    }
    
    return found;
}

int csv_to_json(const CSVFile *csv, char *buffer, size_t size) {
    if (csv == NULL || buffer == NULL) return -1;
    
    size_t pos = 0;
    pos += snprintf(buffer + pos, size - pos, "[\n");
    
    for (int i = 0; i < csv->row_count; i++) {
        pos += snprintf(buffer + pos, size - pos, "  {\n");
        
        for (int j = 0; j < csv->column_count && j < csv->rows[i].cell_count; j++) {
            const char *header = (csv->has_headers && csv->headers) ? csv->headers[j] : "field";
            pos += snprintf(buffer + pos, size - pos, "    \"%s\": \"%s\"%s\n",
                           header, csv->rows[i].cells[j],
                           (j < csv->column_count - 1) ? "," : "");
        }
        
        pos += snprintf(buffer + pos, size - pos, "  }%s\n",
                       (i < csv->row_count - 1) ? "," : "");
    }
    
    pos += snprintf(buffer + pos, size - pos, "]\n");
    return 0;
}

void csv_free(CSVFile *csv) {
    if (csv == NULL) return;
    
    if (csv->headers) {
        for (int i = 0; i < csv->column_count; i++) {
            free(csv->headers[i]);
        }
        free(csv->headers);
    }
    
    for (int i = 0; i < csv->row_count; i++) {
        for (int j = 0; j < csv->rows[i].cell_count; j++) {
            free(csv->rows[i].cells[j]);
        }
        free(csv->rows[i].cells);
    }
    free(csv->rows);
    free(csv);
}

int main(void) {
    // Создаём тестовый CSV файл
    FILE *test = fopen("test_files/employees.csv", "w");
    if (test) {
        fprintf(test, "name, age, city, salary\n");
        fprintf(test, "Иван, 30, Москва, 50000\n");
        fprintf(test, "Мария, 25, Санкт-Петербург, 45000\n");
        fprintf(test, "Пётр, 35, Казань, 55000\n");
        fprintf(test, "Анна, 28, Москва, 60000\n");
        fclose(test);
        printf("Создан тестовый файл: test_files/employees.csv\n\n");
    }
    
    // Загрузка CSV
    CSVFile *csv = csv_load("test_files/employees.csv", ',', 1);
    if (csv == NULL) {
        printf("Ошибка загрузки CSV\n");
        return 1;
    }
    
    printf("=== Содержимое CSV ===\n");
    printf("Заголовки: ");
    for (int i = 0; i < csv->column_count; i++) {
        printf("%s ", csv->headers[i]);
    }
    printf("\n\n");
    
    printf("Строки:\n");
    for (int i = 0; i < csv->row_count; i++) {
        printf("%d: ", i);
        for (int j = 0; j < csv->rows[i].cell_count; j++) {
            printf("%s | ", csv->rows[i].cells[j]);
        }
        printf("\n");
    }
    
    printf("\n=== Доступ по заголовку ===\n");
    const char *age = csv_get_by_header(csv, 1, "age");
    printf("Возраст Марии: %s\n", age);
    
    printf("\n=== Поиск всех из Москвы ===\n");
    int indices[100];
    int count = csv_find_rows(csv, 2, "Москва", indices, 100);
    printf("Найдено %d строк(и):\n", count);
    for (int i = 0; i < count; i++) {
        printf("  Строка %d: %s, %s, %s, %s\n", indices[i],
               csv_get(csv, indices[i], 0),
               csv_get(csv, indices[i], 1),
               csv_get(csv, indices[i], 2),
               csv_get(csv, indices[i], 3));
    }
    
    printf("\n=== Добавление новой строки ===\n");
    const char *new_row[] = {"Сергей", "32", "Новосибирск", "48000"};
    csv_add_row(csv, new_row, 4);
    printf("Добавлен Сергей\n");
    
    printf("\n=== Преобразование в JSON ===\n");
    char json_buffer[4096];
    csv_to_json(csv, json_buffer, sizeof(json_buffer));
    printf("%s\n", json_buffer);
    
    // Сохранение в новый файл
    csv_save(csv, "test_files/employees_updated.csv", ',');
    printf("\nСохранено в test_files/employees_updated.csv\n");
    
    csv_free(csv);
    return 0;
}
```
### Результаты работы
![Результат выполнения](practice-7/screenshots/task-7-5-1.png)
---
## Задание 7.6: Бинарный формат изображения
### Постановка задачи
Реализуйте чтение и запись простого формата изображения.
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| img | Image* | Изображение |
| pixel | Pixel | Пиксель |
### Код программы
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#define IMG_MAGIC 0x494D4731  // "IMG1"

// Структуры

typedef struct {
    uint32_t magic;
    uint32_t width;
    uint32_t height;
    uint8_t bits_per_pixel;  // 8 (grayscale), 24 (RGB), 32 (RGBA)
    uint8_t compression;      // 0 = none
    uint16_t reserved;
} ImageHeader;

typedef struct {
    uint8_t r, g, b, a;
} Pixel;

typedef struct {
    ImageHeader header;
    Pixel *pixels;
} Image;

// Создание

Image* image_create(int width, int height, int bits_per_pixel) {
    if (width <= 0 || height <= 0) return NULL;
    
    Image *img = malloc(sizeof(Image));
    if (img == NULL) return NULL;
    
    img->header.magic = IMG_MAGIC;
    img->header.width = width;
    img->header.height = height;
    img->header.bits_per_pixel = bits_per_pixel;
    img->header.compression = 0;
    img->header.reserved = 0;
    
    int pixel_count = width * height;
    img->pixels = malloc(pixel_count * sizeof(Pixel));
    if (img->pixels == NULL) {
        free(img);
        return NULL;
    }
    
    // Инициализация чёрным цветом
    for (int i = 0; i < pixel_count; i++) {
        img->pixels[i].r = 0;
        img->pixels[i].g = 0;
        img->pixels[i].b = 0;
        img->pixels[i].a = 255;
    }
    
    return img;
}

// Загрузка

Image* image_load(const char *filename) {
    if (filename == NULL) return NULL;
    
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return NULL;
    }
    
    Image *img = malloc(sizeof(Image));
    if (img == NULL) {
        fclose(file);
        return NULL;
    }
    
    // Чтение заголовка
    fread(&img->header, sizeof(ImageHeader), 1, file);
    
    if (img->header.magic != IMG_MAGIC) {
        fprintf(stderr, "Некорректный формат изображения\n");
        fclose(file);
        free(img);
        return NULL;
    }
    
    int pixel_count = img->header.width * img->header.height;
    img->pixels = malloc(pixel_count * sizeof(Pixel));
    if (img->pixels == NULL) {
        fclose(file);
        free(img);
        return NULL;
    }
    
    // Чтение пикселей
    fread(img->pixels, sizeof(Pixel), pixel_count, file);
    
    fclose(file);
    printf("Загружено изображение %dx%d\n", img->header.width, img->header.height);
    return img;
}

// Сохранение

int image_save(const Image *img, const char *filename) {
    if (img == NULL || filename == NULL) return -1;
    
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Ошибка создания файла");
        return -1;
    }
    
    // Запись заголовка
    fwrite(&img->header, sizeof(ImageHeader), 1, file);
    
    // Запись пикселей
    int pixel_count = img->header.width * img->header.height;
    fwrite(img->pixels, sizeof(Pixel), pixel_count, file);
    
    fclose(file);
    printf("Сохранено изображение в %s\n", filename);
    return 0;
}

void image_free(Image *img) {
    if (img == NULL) return;
    
    if (img->pixels != NULL) {
        free(img->pixels);
    }
    free(img);
}

// Доступ к пикселям

Pixel image_get_pixel(const Image *img, int x, int y) {
    Pixel p = {0, 0, 0, 255};
    if (img == NULL) return p;
    if (x < 0 || x >= (int)img->header.width) return p;
    if (y < 0 || y >= (int)img->header.height) return p;
    
    int index = y * img->header.width + x;
    return img->pixels[index];
}

void image_set_pixel(Image *img, int x, int y, Pixel p) {
    if (img == NULL) return;
    if (x < 0 || x >= (int)img->header.width) return;
    if (y < 0 || y >= (int)img->header.height) return;
    
    int index = y * img->header.width + x;
    img->pixels[index] = p;
}

// Преобразования

void image_invert(Image *img) {
    if (img == NULL) return;
    
    int pixel_count = img->header.width * img->header.height;
    for (int i = 0; i < pixel_count; i++) {
        img->pixels[i].r = 255 - img->pixels[i].r;
        img->pixels[i].g = 255 - img->pixels[i].g;
        img->pixels[i].b = 255 - img->pixels[i].b;
    }
}

void image_grayscale(Image *img) {
    if (img == NULL) return;
    
    int pixel_count = img->header.width * img->header.height;
    for (int i = 0; i < pixel_count; i++) {
        uint8_t gray = (img->pixels[i].r + img->pixels[i].g + img->pixels[i].b) / 3;
        img->pixels[i].r = gray;
        img->pixels[i].g = gray;
        img->pixels[i].b = gray;
    }
}

void image_brightness(Image *img, int delta) {
    if (img == NULL) return;
    
    int pixel_count = img->header.width * img->header.height;
    for (int i = 0; i < pixel_count; i++) {
        int r = img->pixels[i].r + delta;
        int g = img->pixels[i].g + delta;
        int b = img->pixels[i].b + delta;
        
        img->pixels[i].r = (r < 0) ? 0 : (r > 255) ? 255 : r;
        img->pixels[i].g = (g < 0) ? 0 : (g > 255) ? 255 : g;
        img->pixels[i].b = (b < 0) ? 0 : (b > 255) ? 255 : b;
    }
}

void image_contrast(Image *img, float factor) {
    if (img == NULL) return;
    
    int pixel_count = img->header.width * img->header.height;
    for (int i = 0; i < pixel_count; i++) {
        float r = (img->pixels[i].r - 128) * factor + 128;
        float g = (img->pixels[i].g - 128) * factor + 128;
        float b = (img->pixels[i].b - 128) * factor + 128;
        
        img->pixels[i].r = (r < 0) ? 0 : (r > 255) ? 255 : r;
        img->pixels[i].g = (g < 0) ? 0 : (g > 255) ? 255 : g;
        img->pixels[i].b = (b < 0) ? 0 : (b > 255) ? 255 : b;
    }
}

// Геометрия

Image* image_crop(const Image *img, int x, int y, int w, int h) {
    if (img == NULL) return NULL;
    
    Image *cropped = image_create(w, h, img->header.bits_per_pixel);
    if (cropped == NULL) return NULL;
    
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            Pixel p = image_get_pixel(img, x + j, y + i);
            image_set_pixel(cropped, j, i, p);
        }
    }
    
    return cropped;
}

Image* image_rotate_90(const Image *img) {
    if (img == NULL) return NULL;
    
    Image *rotated = image_create(img->header.height, img->header.width, img->header.bits_per_pixel);
    if (rotated == NULL) return NULL;
    
    for (int i = 0; i < (int)img->header.height; i++) {
        for (int j = 0; j < (int)img->header.width; j++) {
            Pixel p = image_get_pixel(img, j, i);
            image_set_pixel(rotated, img->header.height - 1 - i, j, p);
        }
    }
    
    return rotated;
}

void image_flip_horizontal(Image *img) {
    if (img == NULL) return;
    
    int w = img->header.width;
    int h = img->header.height;
    
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w / 2; j++) {
            Pixel p1 = image_get_pixel(img, j, i);
            Pixel p2 = image_get_pixel(img, w - 1 - j, i);
            image_set_pixel(img, j, i, p2);
            image_set_pixel(img, w - 1 - j, i, p1);
        }
    }
}

void image_flip_vertical(Image *img) {
    if (img == NULL) return;
    
    int w = img->header.width;
    int h = img->header.height;
    
    for (int i = 0; i < h / 2; i++) {
        for (int j = 0; j < w; j++) {
            Pixel p1 = image_get_pixel(img, j, i);
            Pixel p2 = image_get_pixel(img, j, h - 1 - i);
            image_set_pixel(img, j, i, p2);
            image_set_pixel(img, j, h - 1 - i, p1);
        }
    }
}

// Рисование

void image_draw_line(Image *img, int x1, int y1, int x2, int y2, Pixel color) {
    if (img == NULL) return;
    
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    
    while (1) {
        image_set_pixel(img, x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void image_draw_rect(Image *img, int x, int y, int w, int h, Pixel color) {
    if (img == NULL) return;
    
    image_draw_line(img, x, y, x + w, y, color);
    image_draw_line(img, x + w, y, x + w, y + h, color);
    image_draw_line(img, x + w, y + h, x, y + h, color);
    image_draw_line(img, x, y + h, x, y, color);
}

void image_fill_rect(Image *img, int x, int y, int w, int h, Pixel color) {
    if (img == NULL) return;
    
    for (int i = y; i < y + h && i < (int)img->header.height; i++) {
        for (int j = x; j < x + w && j < (int)img->header.width; j++) {
            image_set_pixel(img, j, i, color);
        }
    }
}

// Вывод изображения

void image_print_ascii(const Image *img) {
    if (img == NULL) return;
    
    printf("Изображение %dx%d:\n", img->header.width, img->header.height);
    printf("+");
    for (int i = 0; i < (int)img->header.width; i++) printf("-");
    printf("+\n");
    
    for (int i = 0; i < (int)img->header.height; i++) {
        printf("|");
        for (int j = 0; j < (int)img->header.width; j++) {
            Pixel p = image_get_pixel(img, j, i);
            // Преобразуем цвет в символ
            char c = ' ';
            if (p.r + p.g + p.b > 600) c = '#';
            else if (p.r + p.g + p.b > 400) c = '*';
            else if (p.r + p.g + p.b > 200) c = '+';
            else if (p.r + p.g + p.b > 50) c = '.';
            else c = ' ';
            printf("%c", c);
        }
        printf("|\n");
    }
    printf("+");
    for (int i = 0; i < (int)img->header.width; i++) printf("-");
    printf("+\n");
}

int main(void) {
    // Создаём папку для изображений
    system("mkdir -p images");
    
    // Создаём изображение 100x100
    printf("=== Создание изображения ===\n");
    Image *img = image_create(100, 100, 24);
    if (img == NULL) {
        printf("Ошибка создания изображения\n");
        return 1;
    }
    
    // Рисуем градиент
    printf("Рисование градиента...\n");
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            Pixel p = {i * 255 / 100, j * 255 / 100, (i + j) * 255 / 200, 255};
            image_set_pixel(img, j, i, p);
        }
    }
    
    // Сохраняем оригинал
    image_save(img, "images/original.img");
    
    // Инвертирование
    printf("\n=== Инвертирование ===\n");
    Image *inverted = image_create(img->header.width, img->header.height, 24);
    for (int i = 0; i < (int)img->header.height; i++) {
        for (int j = 0; j < (int)img->header.width; j++) {
            Pixel p = image_get_pixel(img, j, i);
            image_set_pixel(inverted, j, i, p);
        }
    }
    image_invert(inverted);
    image_save(inverted, "images/inverted.img");
    image_free(inverted);
    
    // Оттенки серого
    printf("\n=== Оттенки серого ===\n");
    Image *gray = image_create(img->header.width, img->header.height, 24);
    for (int i = 0; i < (int)img->header.height; i++) {
        for (int j = 0; j < (int)img->header.width; j++) {
            Pixel p = image_get_pixel(img, j, i);
            image_set_pixel(gray, j, i, p);
        }
    }
    image_grayscale(gray);
    image_save(gray, "images/grayscale.img");
    image_free(gray);
    
    // Поворот на 90 градусов
    printf("\n=== Поворот на 90° ===\n");
    Image *rotated = image_rotate_90(img);
    image_save(rotated, "images/rotated.img");
    image_free(rotated);
    
    // Обрезка
    printf("\n=== Обрезка ===\n");
    Image *cropped = image_crop(img, 25, 25, 50, 50);
    image_save(cropped, "images/cropped.img");
    image_free(cropped);
    
    // Создание простого рисунка
    printf("\n=== Создание рисунка ===\n");
    Image *drawing = image_create(80, 40, 24);
    Pixel white = {255, 255, 255, 255};
    Pixel red = {255, 0, 0, 255};
    Pixel blue = {0, 0, 255, 255};
    Pixel green = {0, 255, 0, 255};
    
    image_fill_rect(drawing, 0, 0, 80, 40, white);
    image_draw_rect(drawing, 10, 5, 60, 30, red);
    image_draw_line(drawing, 10, 20, 70, 20, blue);
    image_draw_line(drawing, 40, 5, 40, 35, green);
    
    image_save(drawing, "images/drawing.img");
    image_print_ascii(drawing);
    image_free(drawing);
    
    // Загрузка и проверка
    printf("\n=== Проверка загрузки ===\n");
    Image *loaded = image_load("images/original.img");
    if (loaded) {
        printf("Изображение успешно загружено!\n");
        image_free(loaded);
    }
    
    image_free(img);
    
    printf("\nВсе изображения сохранены в папке images/\n");
    return 0;
}
```
### Результаты работы
![Результат выполнения](practice-7/screenshots/task-7-6-2.png)
