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
