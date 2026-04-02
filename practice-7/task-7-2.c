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
