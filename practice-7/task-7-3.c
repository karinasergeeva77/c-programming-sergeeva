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
