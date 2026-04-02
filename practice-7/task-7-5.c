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
