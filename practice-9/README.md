# Практическая работа 9: Библиотечный каталог (Вариант D)
**Студент:** Сергеева Карина Алексеевна
**Группа:** 1зб_ИВТ(ускор.)/25
**Дата:** 01 апреля 2026 г.
---
# Library catalog
Консольное приложение для управления библиотечным каталогом.
## Сборка
### С помощью Make:
```bash
make
./bin/library_catalog
```
### С помощью Meson:
```bash
cd ~/c-projects/practice-9/library_catalog
meson setup builddir
meson compile -C builddir
./builddir/library_catalog
```
### Функциональность
| Функция | Описание |
|-----|-----|
| Управление книгами | Добавление, редактирование, удаление, просмотр списка |
| Управление читателями | Регистрация, редактирование, деактивация, просмотр |
| Выдача/возврат книг | Выдача на определённый срок, возврат |
| Отчёты | Список должников с просроченными книгами |
| Поиск | По названию, автору, жанру, ISBN, имени читателя, email |
| Сохранение | Автоматическое сохранение данных в бинарные файлы |
### Структура проекта
library_catalog/
├── include/
│   ├── book.h          # Структура и функции для книг
│   ├── reader.h        # Структура и функции для читателей
│   ├── loan.h          # Структура и функции для выдач
│   ├── storage.h       # Функции сохранения/загрузки
│   ├── ui.h            # Функции интерфейса
│   └── utils.h         # Вспомогательные функции
├── src/
│   ├── main.c          # Точка входа
│   ├── book.c          # Реализация операций с книгами
│   ├── reader.c        # Реализация операций с читателями
│   ├── loan.c          # Реализация операций с выдачами
│   ├── storage.c       # Реализация файлового ввода-вывода
│   ├── ui.c            # Реализация интерфейса
│   └── utils.c         # Реализация вспомогательных функций
├── tests/
│   └── test_main.c     # Модульные тесты
├── data/               # Папка для файлов данных
├── Makefile
├── meson.build
└── README.md
### Структура данных
Book
```c
typedef struct {
    int id;
    char title[200];
    char author[100];
    char isbn[20];
    int year;
    char genre[50];
    int total_copies;
    int available_copies;
} Book;
```
Reader
```c
typedef struct {
    int id;
    char name[100];
    char email[100];
    char phone[20];
    time_t registered_at;
    int active;
} Reader;
```
Loan
```c
typedef struct {
    int id;
    int book_id;
    int reader_id;
    time_t borrowed_at;
    time_t due_date;
    time_t returned_at;
} Loan;
```
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| Book | struct | Книга (ID, название, автор, ISBN, год, жанр, экземпляры) |
| Reader | struct | Читатель (ID, имя, email, телефон, дата регистрации, статус) |
| Loan | struct | Выдача (ID, ID книги, ID читателя, даты выдачи/возврата/срока) |
| book_create | Book* (*)(...) | Создание новой книги |
| reader_create | Reader* (*)(...) | Создание нового читателя |
| loan_create | Loan* (*)(...) | Создание новой выдачи |
| save_books | int (*)(...) | Сохранение книг в файл |
| load_books | int (*)(...) | Загрузка книг из файла |
### Код программы
include/book.h
```c
#ifndef BOOK_H
#define BOOK_H

#include <time.h>

typedef struct {
    int id;
    char title[200];
    char author[100];
    char isbn[20];
    int year;
    char genre[50];
    int total_copies;
    int available_copies;
} Book;

Book* book_create(int id, const char *title, const char *author, 
                  const char *isbn, int year, const char *genre, int total_copies);
void book_free(Book *book);
Book* book_copy(const Book *src);
void book_print(const Book *book);
int book_update(Book *book, const char *title, const char *author,
                const char *isbn, int year, const char *genre, int total_copies);
int book_borrow(Book *book);
int book_return(Book *book);
int book_is_available(const Book *book);
int book_matches(const Book *book, const char *query);

#endif
```
include/reader.h
```c
#ifndef READER_H
#define READER_H

#include <time.h>

typedef struct {
    int id;
    char name[100];
    char email[100];
    char phone[20];
    time_t registered_at;
    int active;
} Reader;

Reader* reader_create(int id, const char *name, const char *email, const char *phone);
void reader_free(Reader *reader);
Reader* reader_copy(const Reader *src);
void reader_print(const Reader *reader);
int reader_update(Reader *reader, const char *name, const char *email, const char *phone);
int reader_deactivate(Reader *reader);
int reader_activate(Reader *reader);
int reader_is_active(const Reader *reader);
int reader_matches(const Reader *reader, const char *query);

#endif
```
include/loan.h
```c
#ifndef LOAN_H
#define LOAN_H

#include <time.h>

typedef struct {
    int id;
    int book_id;
    int reader_id;
    time_t borrowed_at;
    time_t due_date;
    time_t returned_at;
} Loan;

Loan* loan_create(int id, int book_id, int reader_id, time_t due_date);
void loan_free(Loan *loan);
Loan* loan_copy(const Loan *src);
void loan_print(const Loan *loan);
void loan_return(Loan *loan);
int loan_is_active(const Loan *loan);
int loan_is_overdue(const Loan *loan);

#endif
```
include/storage.h
```c
#ifndef STORAGE_H
#define STORAGE_H

#include <stddef.h>
#include "book.h"
#include "reader.h"
#include "loan.h"

int save_array_to_file(const char *filename, void *array, size_t count, size_t elem_size);
int load_array_from_file(const char *filename, void **array, size_t *count, size_t elem_size);

int save_books(const char *filename, void *books, size_t count);
int load_books(const char *filename, void **books, size_t *count);

int save_readers(const char *filename, void *readers, size_t count);
int load_readers(const char *filename, void **readers, size_t *count);

int save_loans(const char *filename, void *loans, size_t count);
int load_loans(const char *filename, void **loans, size_t *count);

#endif
```
include/ui.h
```c
#ifndef UI_H
#define UI_H

#include "book.h"
#include "reader.h"
#include "loan.h"

void show_main_menu(void);
void show_books_menu(void);
void show_readers_menu(void);
void show_loans_menu(void);
void show_reports_menu(void);

int get_int_input(const char *prompt);
char* get_string_input(const char *prompt, char *buffer, size_t size);

void print_books_table(Book *books, int count);
void print_readers_table(Reader *readers, int count);
void print_loans_table(Loan *loans, int count, Book *books, Reader *readers);

void print_overdue_report(Loan *loans, int loan_count, 
                          Book *books, int book_count,
                          Reader *readers, int reader_count);

#endif
```
include/utils.h
```c
#ifndef UTILS_H
#define UTILS_H

#include <time.h>
#include <stddef.h>

struct Book;
struct Reader;
struct Loan;

char* str_trim(char *str);
char* str_to_lower(char *str);
int str_contains(const char *haystack, const char *needle);

time_t parse_date(const char *date_str);
void format_date(char *buffer, size_t size, time_t timestamp);
int days_between(time_t later, time_t earlier);

int find_book_by_id(struct Book *books, int count, int id);
int find_reader_by_id(struct Reader *readers, int count, int id);
int find_loan_by_id(struct Loan *loans, int count, int id);

void sort_books_by_title(struct Book *books, int count);
void sort_readers_by_name(struct Reader *readers, int count);
void sort_loans_by_date(struct Loan *loans, int count);

int get_next_id(int *current_max);

#endif
```
src/book.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "book.h"
#include "utils.h"

Book* book_create(int id, const char *title, const char *author, 
                  const char *isbn, int year, const char *genre, int total_copies) {
    if (title == NULL || author == NULL) return NULL;
    
    Book *book = malloc(sizeof(Book));
    if (book == NULL) return NULL;
    
    book->id = id;
    strncpy(book->title, title, sizeof(book->title) - 1);
    book->title[sizeof(book->title) - 1] = '\0';
    strncpy(book->author, author, sizeof(book->author) - 1);
    book->author[sizeof(book->author) - 1] = '\0';
    
    if (isbn) {
        strncpy(book->isbn, isbn, sizeof(book->isbn) - 1);
        book->isbn[sizeof(book->isbn) - 1] = '\0';
    } else book->isbn[0] = '\0';
    
    book->year = year;
    if (genre) {
        strncpy(book->genre, genre, sizeof(book->genre) - 1);
        book->genre[sizeof(book->genre) - 1] = '\0';
    } else book->genre[0] = '\0';
    
    book->total_copies = total_copies;
    book->available_copies = total_copies;
    return book;
}

void book_free(Book *book) { if (book) free(book); }

Book* book_copy(const Book *src) {
    if (!src) return NULL;
    return book_create(src->id, src->title, src->author, 
                       src->isbn, src->year, src->genre, src->total_copies);
}

void book_print(const Book *book) {
    if (!book) return;
    printf("ID: %d\nНазвание: %s\nАвтор: %s\nISBN: %s\nГод: %d\nЖанр: %s\nВсего: %d, Доступно: %d\n",
           book->id, book->title, book->author, book->isbn, book->year,
           book->genre, book->total_copies, book->available_copies);
}

int book_update(Book *book, const char *title, const char *author,
                const char *isbn, int year, const char *genre, int total_copies) {
    if (!book) return -1;
    if (title) { strncpy(book->title, title, sizeof(book->title)-1); }
    if (author) { strncpy(book->author, author, sizeof(book->author)-1); }
    if (isbn) { strncpy(book->isbn, isbn, sizeof(book->isbn)-1); }
    if (year > 0) book->year = year;
    if (genre) { strncpy(book->genre, genre, sizeof(book->genre)-1); }
    if (total_copies > 0) {
        int diff = total_copies - book->total_copies;
        book->total_copies = total_copies;
        book->available_copies += diff;
        if (book->available_copies < 0) book->available_copies = 0;
    }
    return 0;
}

int book_borrow(Book *book) {
    if (!book || book->available_copies <= 0) return -1;
    book->available_copies--;
    return 0;
}

int book_return(Book *book) {
    if (!book || book->available_copies >= book->total_copies) return -1;
    book->available_copies++;
    return 0;
}

int book_is_available(const Book *book) { return book && book->available_copies > 0; }

int book_matches(const Book *book, const char *query) {
    if (!book || !query) return 0;
    char q[256], t[200], a[100], g[50];
    strcpy(q, query); str_to_lower(q);
    strcpy(t, book->title); str_to_lower(t);
    strcpy(a, book->author); str_to_lower(a);
    strcpy(g, book->genre); str_to_lower(g);
    return strstr(t, q) || strstr(a, q) || strstr(g, q) || strstr(book->isbn, query);
}
```
src/reader.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "reader.h"
#include "utils.h"

Reader* reader_create(int id, const char *name, const char *email, const char *phone) {
    if (!name) return NULL;
    Reader *r = malloc(sizeof(Reader));
    if (!r) return NULL;
    r->id = id;
    strncpy(r->name, name, sizeof(r->name)-1);
    strncpy(r->email, email ? email : "", sizeof(r->email)-1);
    strncpy(r->phone, phone ? phone : "", sizeof(r->phone)-1);
    r->registered_at = time(NULL);
    r->active = 1;
    return r;
}

void reader_free(Reader *r) { if (r) free(r); }

Reader* reader_copy(const Reader *src) {
    if (!src) return NULL;
    return reader_create(src->id, src->name, src->email, src->phone);
}

void reader_print(const Reader *r) {
    if (!r) return;
    char date[20];
    format_date(date, sizeof(date), r->registered_at);
    printf("ID: %d\nИмя: %s\nEmail: %s\nТелефон: %s\nЗарегистрирован: %s\nСтатус: %s\n",
           r->id, r->name, r->email, r->phone, date, r->active ? "Активен" : "Деактивирован");
}

int reader_update(Reader *r, const char *name, const char *email, const char *phone) {
    if (!r) return -1;
    if (name) strncpy(r->name, name, sizeof(r->name)-1);
    if (email) strncpy(r->email, email, sizeof(r->email)-1);
    if (phone) strncpy(r->phone, phone, sizeof(r->phone)-1);
    return 0;
}

int reader_deactivate(Reader *r) { if (!r) return -1; r->active = 0; return 0; }
int reader_activate(Reader *r) { if (!r) return -1; r->active = 1; return 0; }
int reader_is_active(const Reader *r) { return r && r->active; }

int reader_matches(const Reader *r, const char *query) {
    if (!r || !query) return 0;
    char q[256], n[100], e[100];
    strcpy(q, query); str_to_lower(q);
    strcpy(n, r->name); str_to_lower(n);
    strcpy(e, r->email); str_to_lower(e);
    return strstr(n, q) || strstr(e, q) || strstr(r->phone, query);
}
```
src/loan.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "loan.h"
#include "utils.h"

Loan* loan_create(int id, int book_id, int reader_id, time_t due_date) {
    Loan *l = malloc(sizeof(Loan));
    if (!l) return NULL;
    l->id = id;
    l->book_id = book_id;
    l->reader_id = reader_id;
    l->borrowed_at = time(NULL);
    l->due_date = due_date;
    l->returned_at = 0;
    return l;
}

void loan_free(Loan *l) { if (l) free(l); }

Loan* loan_copy(const Loan *src) {
    if (!src) return NULL;
    Loan *l = malloc(sizeof(Loan));
    memcpy(l, src, sizeof(Loan));
    return l;
}

void loan_print(const Loan *l) {
    if (!l) return;
    char borrowed[20], due[20];
    format_date(borrowed, sizeof(borrowed), l->borrowed_at);
    format_date(due, sizeof(due), l->due_date);
    printf("ID выдачи: %d\nID книги: %d\nID читателя: %d\nДата выдачи: %s\nСрок: %s\n",
           l->id, l->book_id, l->reader_id, borrowed, due);
    if (l->returned_at) {
        char returned[20];
        format_date(returned, sizeof(returned), l->returned_at);
        printf("Возвращена: %s\n", returned);
    } else printf("Статус: %s\n", loan_is_overdue(l) ? "ПРОСРОЧЕНА" : "Активна");
}

void loan_return(Loan *l) { if (l && !l->returned_at) l->returned_at = time(NULL); }
int loan_is_active(const Loan *l) { return l && l->returned_at == 0; }
int loan_is_overdue(const Loan *l) { return l && l->returned_at == 0 && time(NULL) > l->due_date; }
```
src/storage.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"
#include "book.h"
#include "reader.h"
#include "loan.h"

int save_array_to_file(const char *filename, void *array, size_t count, size_t elem_size) {
    if (!filename) return -1;
    FILE *f = fopen(filename, "wb");
    if (!f) return -1;
    if (fwrite(&count, sizeof(size_t), 1, f) != 1) { fclose(f); return -1; }
    if (count > 0 && array && fwrite(array, elem_size, count, f) != count) { fclose(f); return -1; }
    fclose(f);
    return 0;
}

int load_array_from_file(const char *filename, void **array, size_t *count, size_t elem_size) {
    if (!filename || !array || !count) return -1;
    FILE *f = fopen(filename, "rb");
    if (!f) return -1;
    if (fread(count, sizeof(size_t), 1, f) != 1) { fclose(f); return -1; }
    if (*count == 0) { *array = NULL; fclose(f); return 0; }
    void *data = malloc(*count * elem_size);
    if (!data) { fclose(f); return -1; }
    if (fread(data, elem_size, *count, f) != *count) { free(data); fclose(f); return -1; }
    *array = data;
    fclose(f);
    return 0;
}

int save_books(const char *filename, void *books, size_t count) {
    return save_array_to_file(filename, books, count, sizeof(Book));
}

int load_books(const char *filename, void **books, size_t *count) {
    return load_array_from_file(filename, books, count, sizeof(Book));
}

int save_readers(const char *filename, void *readers, size_t count) {
    return save_array_to_file(filename, readers, count, sizeof(Reader));
}

int load_readers(const char *filename, void **readers, size_t *count) {
    return load_array_from_file(filename, readers, count, sizeof(Reader));
}

int save_loans(const char *filename, void *loans, size_t count) {
    return save_array_to_file(filename, loans, count, sizeof(Loan));
}

int load_loans(const char *filename, void **loans, size_t *count) {
    return load_array_from_file(filename, loans, count, sizeof(Loan));
}
```
src/main.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "book.h"
#include "reader.h"
#include "loan.h"
#include "storage.h"
#include "ui.h"
#include "utils.h"

#define DATA_DIR "data/"
#define BOOKS_FILE DATA_DIR "books.dat"
#define READERS_FILE DATA_DIR "readers.dat"
#define LOANS_FILE DATA_DIR "loans.dat"

int main(void) {
    Book *books = NULL;
    Reader *readers = NULL;
    Loan *loans = NULL;
    int book_count = 0, reader_count = 0, loan_count = 0;
    int next_book_id = 0, next_reader_id = 0, next_loan_id = 0;
    
    system("mkdir -p data");
    load_books(BOOKS_FILE, (void**)&books, &book_count);
    load_readers(READERS_FILE, (void**)&readers, &reader_count);
    load_loans(LOANS_FILE, (void**)&loans, &loan_count);
    
    for (int i = 0; i < book_count; i++) if (books[i].id > next_book_id) next_book_id = books[i].id;
    for (int i = 0; i < reader_count; i++) if (readers[i].id > next_reader_id) next_reader_id = readers[i].id;
    for (int i = 0; i < loan_count; i++) if (loans[i].id > next_loan_id) next_loan_id = loans[i].id;
    
    int running = 1;
    while (running) {
        show_main_menu();
        int choice = get_int_input("");
        switch (choice) {
            case 1: { // Книги
                int books_running = 1;
                while (books_running) {
                    show_books_menu();
                    int b_choice = get_int_input("");
                    switch (b_choice) {
                        case 1: { // Добавить книгу
                            char title[200], author[100], isbn[20], genre[50];
                            int year, copies;
                            get_string_input("Название: ", title, sizeof(title));
                            get_string_input("Автор: ", author, sizeof(author));
                            get_string_input("ISBN: ", isbn, sizeof(isbn));
                            year = get_int_input("Год издания: ");
                            get_string_input("Жанр: ", genre, sizeof(genre));
                            copies = get_int_input("Количество экземпляров: ");
                            next_book_id++;
                            Book *new_b = book_create(next_book_id, title, author, isbn, year, genre, copies);
                            books = realloc(books, (book_count + 1) * sizeof(Book));
                            books[book_count++] = *new_b;
                            free(new_b);
                            printf("Книга добавлена. ID: %d\n", next_book_id);
                            break;
                        }
                        case 2: { // Редактировать книгу
                            print_books_table(books, book_count);
                            int id = get_int_input("ID книги для редактирования: ");
                            int idx = find_book_by_id(books, book_count, id);
                            if (idx >= 0) {
                                char title[200], author[100], isbn[20], genre[50];
                                get_string_input("Новое название (Enter - оставить): ", title, sizeof(title));
                                get_string_input("Новый автор: ", author, sizeof(author));
                                get_string_input("Новый ISBN: ", isbn, sizeof(isbn));
                                int year = get_int_input("Новый год (0 - оставить): ");
                                get_string_input("Новый жанр: ", genre, sizeof(genre));
                                int copies = get_int_input("Новое количество (0 - оставить): ");
                                book_update(&books[idx], 
                                    strlen(title) ? title : NULL,
                                    strlen(author) ? author : NULL,
                                    strlen(isbn) ? isbn : NULL,
                                    year, strlen(genre) ? genre : NULL, copies);
                                printf("Книга обновлена.\n");
                            } else printf("Книга не найдена.\n");
                            break;
                        }
                        case 3: { // Удалить книгу
                            print_books_table(books, book_count);
                            int id = get_int_input("ID книги для удаления: ");
                            int idx = find_book_by_id(books, book_count, id);
                            if (idx >= 0) {
                                for (int i = idx; i < book_count - 1; i++) books[i] = books[i+1];
                                book_count--;
                                books = realloc(books, book_count * sizeof(Book));
                                printf("Книга удалена.\n");
                            } else printf("Книга не найдена.\n");
                            break;
                        }
                        case 4: print_books_table(books, book_count); break;
                        case 5: books_running = 0; break;
                        default: printf("Неверный выбор.\n");
                    }
                }
                break;
            }
            case 2: { // Читатели
                int readers_running = 1;
                while (readers_running) {
                    show_readers_menu();
                    int r_choice = get_int_input("");
                    switch (r_choice) {
                        case 1: {
                            char name[100], email[100], phone[20];
                            get_string_input("Имя: ", name, sizeof(name));
                            get_string_input("Email: ", email, sizeof(email));
                            get_string_input("Телефон: ", phone, sizeof(phone));
                            next_reader_id++;
                            Reader *new_r = reader_create(next_reader_id, name, email, phone);
                            readers = realloc(readers, (reader_count + 1) * sizeof(Reader));
                            readers[reader_count++] = *new_r;
                            free(new_r);
                            printf("Читатель добавлен. ID: %d\n", next_reader_id);
                            break;
                        }
                        case 2: {
                            print_readers_table(readers, reader_count);
                            int id = get_int_input("ID читателя для редактирования: ");
                            int idx = find_reader_by_id(readers, reader_count, id);
                            if (idx >= 0) {
                                char name[100], email[100], phone[20];
                                get_string_input("Новое имя: ", name, sizeof(name));
                                get_string_input("Новый email: ", email, sizeof(email));
                                get_string_input("Новый телефон: ", phone, sizeof(phone));
                                reader_update(&readers[idx], 
                                    strlen(name) ? name : NULL,
                                    strlen(email) ? email : NULL,
                                    strlen(phone) ? phone : NULL);
                                printf("Читатель обновлён.\n");
                            } else printf("Читатель не найден.\n");
                            break;
                        }
                        case 3: {
                            print_readers_table(readers, reader_count);
                            int id = get_int_input("ID читателя для удаления: ");
                            int idx = find_reader_by_id(readers, reader_count, id);
                            if (idx >= 0) {
                                reader_deactivate(&readers[idx]);
                                printf("Читатель деактивирован.\n");
                            } else printf("Читатель не найден.\n");
                            break;
                        }
                        case 4: print_readers_table(readers, reader_count); break;
                        case 5: readers_running = 0; break;
                        default: printf("Неверный выбор.\n");
                    }
                }
                break;
            }
            case 3: { // Выдача книг
                print_books_table(books, book_count);
                print_readers_table(readers, reader_count);
                int book_id = get_int_input("ID книги: ");
                int reader_id = get_int_input("ID читателя: ");
                int bi = find_book_by_id(books, book_count, book_id);
                int ri = find_reader_by_id(readers, reader_count, reader_id);
                if (bi >= 0 && ri >= 0 && readers[ri].active && books[bi].available_copies > 0) {
                    next_loan_id++;
                    int days = get_int_input("Срок выдачи (дней): ");
                    time_t due = time(NULL) + days * 24 * 3600;
                    Loan *new_l = loan_create(next_loan_id, book_id, reader_id, due);
                    loans = realloc(loans, (loan_count + 1) * sizeof(Loan));
                    loans[loan_count++] = *new_l;
                    free(new_l);
                    book_borrow(&books[bi]);
                    printf("Книга выдана. ID выдачи: %d\n", next_loan_id);
                } else printf("Выдача невозможна.\n");
                break;
            }
            case 4: { // Возврат книг
                print_loans_table(loans, loan_count, books, readers);
                int loan_id = get_int_input("ID выдачи: ");
                int idx = find_loan_by_id(loans, loan_count, loan_id);
                if (idx >= 0 && loans[idx].returned_at == 0) {
                    int bi = find_book_by_id(books, book_count, loans[idx].book_id);
                    if (bi >= 0) book_return(&books[bi]);
                    loan_return(&loans[idx]);
                    printf("Книга возвращена.\n");
                } else printf("Выдача не найдена или уже возвращена.\n");
                break;
            }
            case 5: { // Отчёты
                int reports_running = 1;
                while (reports_running) {
                    show_reports_menu();
                    int r_choice = get_int_input("");
                    switch (r_choice) {
                        case 1: print_overdue_report(loans, loan_count, books, book_count, readers, reader_count); break;
                        case 2: reports_running = 0; break;
                        default: printf("Неверный выбор.\n");
                    }
                }
                break;
            }
            case 6: { // Поиск
                char query[100];
                get_string_input("Поисковый запрос: ", query, sizeof(query));
                printf("\n=== Результаты поиска ===\n");
                printf("\nКниги:\n");
                for (int i = 0; i < book_count; i++)
                    if (book_matches(&books[i], query))
                        printf("  [%d] %s - %s (%s)\n", books[i].id, books[i].title, books[i].author, books[i].genre);
                printf("\nЧитатели:\n");
                for (int i = 0; i < reader_count; i++)
                    if (reader_matches(&readers[i], query))
                        printf("  [%d] %s (%s, %s)\n", readers[i].id, readers[i].name, readers[i].email, readers[i].phone);
                break;
            }
            case 7: // Сохранить и выйти
                save_books(BOOKS_FILE, books, book_count);
                save_readers(READERS_FILE, readers, reader_count);
                save_loans(LOANS_FILE, loans, loan_count);
                printf("Данные сохранены.\n");
                running = 0;
                break;
            default: printf("Неверный выбор.\n");
        }
    }
    
    free(books); free(readers); free(loans);
    return 0;
}
```
src/ui.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ui.h"
#include "utils.h"

void show_main_menu(void) {
    printf("\n╔════════════════════════════════════════════════════╗\n");
    printf("║              БИБЛИОТЕЧНЫЙ КАТАЛОГ                   ║\n");
    printf("╠════════════════════════════════════════════════════╣\n");
    printf("║  1. Управление книгами                             ║\n");
    printf("║  2. Управление читателями                          ║\n");
    printf("║  3. Выдача книг                                    ║\n");
    printf("║  4. Возврат книг                                   ║\n");
    printf("║  5. Отчёты                                         ║\n");
    printf("║  6. Поиск                                          ║\n");
    printf("║  7. Сохранить и выйти                              ║\n");
    printf("╚════════════════════════════════════════════════════╝\n");
    printf("Выберите действие: ");
}

void show_books_menu(void) {
    printf("\n--- Управление книгами ---\n");
    printf("1. Добавить книгу\n2. Редактировать книгу\n3. Удалить книгу\n4. Список всех книг\n5. Назад\nВыберите: ");
}

void show_readers_menu(void) {
    printf("\n--- Управление читателями ---\n");
    printf("1. Добавить читателя\n2. Редактировать читателя\n3. Удалить читателя\n4. Список всех читателей\n5. Назад\nВыберите: ");
}

void show_loans_menu(void) {
    printf("\n--- Управление выдачами ---\n");
    printf("1. Выдать книгу\n2. Принять возврат\n3. Список активных выдач\n4. Назад\nВыберите: ");
}

void show_reports_menu(void) {
    printf("\n--- Отчёты ---\n");
    printf("1. Список должников\n2. Назад\nВыберите: ");
}

int get_int_input(const char *prompt) {
    int val;
    printf("%s", prompt);
    scanf("%d", &val);
    while (getchar() != '\n');
    return val;
}

char* get_string_input(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    return str_trim(buffer);
}

void print_books_table(Book *books, int count) {
    if (count == 0) { printf("Нет книг.\n"); return; }
    printf("\n%-5s | %-30s | %-20s | %-10s | %s\n", "ID", "Название", "Автор", "Жанр", "Доступно");
    printf("------+--------------------------------+----------------------+------------+----------\n");
    for (int i = 0; i < count; i++) {
        printf("%-5d | %-30.30s | %-20.20s | %-10.10s | %d/%d\n",
               books[i].id, books[i].title, books[i].author, books[i].genre,
               books[i].available_copies, books[i].total_copies);
    }
}

void print_readers_table(Reader *readers, int count) {
    if (count == 0) { printf("Нет читателей.\n"); return; }
    printf("\n%-5s | %-25s | %-25s | %-12s | %s\n", "ID", "Имя", "Email", "Телефон", "Статус");
    printf("------+---------------------------+---------------------------+--------------+----------\n");
    for (int i = 0; i < count; i++) {
        printf("%-5d | %-25.25s | %-25.25s | %-12s | %s\n",
               readers[i].id, readers[i].name, readers[i].email,
               readers[i].phone, readers[i].active ? "Активен" : "Деактивирован");
    }
}

void print_loans_table(Loan *loans, int count, Book *books, Reader *readers) {
    if (count == 0) { printf("Нет выдач.\n"); return; }
    char borrowed[20], due[20];
    printf("\n%-5s | %-30s | %-25s | %-12s | %-12s | %s\n", "ID", "Книга", "Читатель", "Выдана", "Срок", "Статус");
    printf("------+--------------------------------+---------------------------+--------------+--------------+------------\n");
    for (int i = 0; i < count; i++) {
        format_date(borrowed, sizeof(borrowed), loans[i].borrowed_at);
        format_date(due, sizeof(due), loans[i].due_date);
        // Приводим типы для передачи в find_*_by_id
        int bi = find_book_by_id((struct Book*)books, -1, loans[i].book_id);
        int ri = find_reader_by_id((struct Reader*)readers, -1, loans[i].reader_id);
        const char *bt = (bi >= 0) ? books[bi].title : "Не найдена";
        const char *rn = (ri >= 0) ? readers[ri].name : "Не найден";
        const char *st = loans[i].returned_at ? "Возвращена" : (loan_is_overdue(&loans[i]) ? "ПРОСРОЧЕНА" : "Активна");
        printf("%-5d | %-30.30s | %-25.25s | %-12s | %-12s | %s\n",
               loans[i].id, bt, rn, borrowed, due, st);
    }
}

void print_overdue_report(Loan *loans, int loan_count, Book *books, int book_count, Reader *readers, int reader_count) {
    printf("\n=== СПИСОК ДОЛЖНИКОВ ===\n");
    int found = 0;
    for (int i = 0; i < loan_count; i++) {
        if (loan_is_overdue(&loans[i])) {
            int bi = find_book_by_id((struct Book*)books, book_count, loans[i].book_id);
            int ri = find_reader_by_id((struct Reader*)readers, reader_count, loans[i].reader_id);
            if (bi >= 0 && ri >= 0) {
                char due[20];
                format_date(due, sizeof(due), loans[i].due_date);
                printf("\nЧитатель: %s\n  Книга: %s\n  Срок: %s\n  Просрочка: %d дней\n",
                       readers[ri].name, books[bi].title, due,
                       days_between(time(NULL), loans[i].due_date));
                found++;
            }
        }
    }
    if (!found) printf("Нет должников.\n");
}
```
src/utils.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "utils.h"
#include "book.h"
#include "reader.h"
#include "loan.h"

char* str_trim(char *str) {
    if (!str) return NULL;
    while (isspace((unsigned char)*str)) str++;
    if (*str == '\0') return str;
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

char* str_to_lower(char *str) {
    if (!str) return NULL;
    for (int i = 0; str[i]; i++) str[i] = tolower((unsigned char)str[i]);
    return str;
}

int str_contains(const char *haystack, const char *needle) {
    if (!haystack || !needle) return 0;
    return strstr(haystack, needle) != NULL;
}

time_t parse_date(const char *date_str) {
    if (!date_str) return time(NULL);
    int y, m, d;
    if (sscanf(date_str, "%d-%d-%d", &y, &m, &d) == 3) {
        struct tm tm = {0};
        tm.tm_year = y - 1900;
        tm.tm_mon = m - 1;
        tm.tm_mday = d;
        tm.tm_hour = 23;
        tm.tm_min = 59;
        tm.tm_sec = 59;
        return mktime(&tm);
    }
    return time(NULL) + 14 * 24 * 3600;
}

void format_date(char *buffer, size_t size, time_t timestamp) {
    if (!buffer) return;
    if (timestamp == 0) {
        snprintf(buffer, size, "Не возвращена");
        return;
    }
    struct tm *tm = localtime(&timestamp);
    strftime(buffer, size, "%Y-%m-%d", tm);
}

int days_between(time_t later, time_t earlier) {
    return (int)(difftime(later, earlier) / (60 * 60 * 24));
}

// Используем struct Book * для соответствия с utils.h
int find_book_by_id(struct Book *books, int count, int id) {
    if (!books) return -1;
    Book *b = (Book*)books;
    for (int i = 0; i < count; i++) if (b[i].id == id) return i;
    return -1;
}

int find_reader_by_id(struct Reader *readers, int count, int id) {
    if (!readers) return -1;
    Reader *r = (Reader*)readers;
    for (int i = 0; i < count; i++) if (r[i].id == id) return i;
    return -1;
}

int find_loan_by_id(struct Loan *loans, int count, int id) {
    if (!loans) return -1;
    Loan *l = (Loan*)loans;
    for (int i = 0; i < count; i++) if (l[i].id == id) return i;
    return -1;
}

static int compare_books(const void *a, const void *b) {
    return strcmp(((Book*)a)->title, ((Book*)b)->title);
}

void sort_books_by_title(struct Book *books, int count) {
    if (books && count > 1) qsort(books, count, sizeof(Book), compare_books);
}

static int compare_readers(const void *a, const void *b) {
    return strcmp(((Reader*)a)->name, ((Reader*)b)->name);
}

void sort_readers_by_name(struct Reader *readers, int count) {
    if (readers && count > 1) qsort(readers, count, sizeof(Reader), compare_readers);
}

static int compare_loans(const void *a, const void *b) {
    return ((Loan*)a)->borrowed_at - ((Loan*)b)->borrowed_at;
}

void sort_loans_by_date(struct Loan *loans, int count) {
    if (loans && count > 1) qsort(loans, count, sizeof(Loan), compare_loans);
}

int get_next_id(int *current_max) {
    if (!current_max) return 1;
    return ++(*current_max);
}
```
makefile
```c
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -I include
CFLAGS_DEBUG = -g -O0 -DDEBUG
CFLAGS_RELEASE = -O2 -DNDEBUG
LDFLAGS = -lm

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(SRC_DIR)/book.c $(SRC_DIR)/reader.c $(SRC_DIR)/loan.c \
       $(SRC_DIR)/storage.c $(SRC_DIR)/ui.c $(SRC_DIR)/utils.c $(SRC_DIR)/main.c
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
TARGET = $(BIN_DIR)/library_catalog

.PHONY: all clean debug release test

all: release

debug: CFLAGS += $(CFLAGS_DEBUG)
debug: $(TARGET)

release: CFLAGS += $(CFLAGS_RELEASE)
release: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

init:
	mkdir -p data
```
meson.build
```c
project('library_catalog', 'c',
    version: '1.0.0',
    default_options: ['warning_level=3', 'c_std=c11']
)

inc = include_directories('include')

lib_src = files(
    'src/book.c', 'src/reader.c', 'src/loan.c',
    'src/storage.c', 'src/ui.c', 'src/utils.c'
)

executable('library_catalog', 'src/main.c', lib_src,
    include_directories: inc,
    dependencies: meson.get_compiler('c').find_library('m', required: false)
)

test('library tests', executable('test_library', 'tests/test_main.c', lib_src,
    include_directories: inc))
```
### Результат
В папке screenshots
