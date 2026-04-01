# Практическая работа 6: Структуры и объединения
**Студент:** Сергеева Карина Алексеевна
**Группа:** 1зб_ИВТ(ускор.)/25
**Дата:** 31 марта 2026 г.
---
## Задание 6.1: Адресная книга
### Постановка задачи
Создайте систему управления контактами.
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| book | AddressBook | Адресная книга |
| contact | Contact | Контакт |
| phone | Phone | Телефон |
| birthday | Date | Дата рождения |
### Код программы
```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Структуры:

typedef enum {
    PHONE_HOME,
    PHONE_WORK,
    PHONE_MOBILE
} PhoneType;

typedef struct {
    PhoneType type;
    char number[20];
} Phone;

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    char first_name[30];
    char last_name[30];
    char email[50];
    Phone phones[3];
    int phone_count;
    Date birthday;
} Contact;

typedef struct {
    Contact contacts[100];
    int count;
} AddressBook;

// Функции:

const char* phone_type_to_string(PhoneType type) {
    switch (type) {
        case PHONE_HOME:   return "Домашний";
        case PHONE_WORK:   return "Рабочий";
        case PHONE_MOBILE: return "Мобильный";
        default:           return "Неизвестный";
    }
}

// Инициализация книги
void book_init(AddressBook *book) {
    if (book == NULL) return;
    book->count = 0;
    memset(book->contacts, 0, sizeof(book->contacts));
}

// Добавление контакта
int book_add(AddressBook *book, const Contact *contact) {
    if (book == NULL || contact == NULL) return -1;
    if (book->count >= 100) return -1;
    
    book->contacts[book->count] = *contact;
    book->count++;
    
    return book->count - 1;
}

// Поиск по фамилии
int book_find_by_lastname(const AddressBook *book, const char *lastname) {
    if (book == NULL || lastname == NULL) return -1;
    
    for (int i = 0; i < book->count; i++) {
        if (strcmp(book->contacts[i].last_name, lastname) == 0) {
            return i;
        }
    }
    return -1;
}

// Поиск по email
int book_find_by_email(const AddressBook *book, const char *email) {
    if (book == NULL || email == NULL) return -1;
    
    for (int i = 0; i < book->count; i++) {
        if (strcmp(book->contacts[i].email, email) == 0) {
            return i;
        }
    }
    return -1;
}

// Удаление контакта
int book_remove(AddressBook *book, int index) {
    if (book == NULL) return -1;
    if (index < 0 || index >= book->count) return -1;
    
    for (int i = index; i < book->count - 1; i++) {
        book->contacts[i] = book->contacts[i + 1];
    }
    book->count--;
    
    return 0;
}

// Вывод всех контактов
void book_print(const AddressBook *book) {
    if (book == NULL) return;
    
    printf("\n=== Адресная книга ===\n");
    
    for (int i = 0; i < book->count; i++) {
        Contact c = book->contacts[i];
        printf("%d. %s %s\n", i + 1, c.last_name, c.first_name);
        printf("   Email: %s\n", c.email);
        printf("   Телефоны:\n");
        for (int j = 0; j < c.phone_count; j++) {
            printf("     [%s] %s\n", phone_type_to_string(c.phones[j].type), c.phones[j].number);
        }
        printf("   День рождения: %02d.%02d.%04d\n", c.birthday.day, c.birthday.month, c.birthday.year);
        printf("\n");
    }
}

// Вывод контактов с днём рождения в указанном месяце
void book_print_birthdays(const AddressBook *book, int month) {
    if (book == NULL) return;
    
    printf("\nКонтакты с днём рождения в %02d месяце:\n", month);
    
    for (int i = 0; i < book->count; i++) {
        Contact c = book->contacts[i];
        if (c.birthday.month == month) {
            printf("- %s %s (%02d.%02d)\n", c.last_name, c.first_name, 
                   c.birthday.day, c.birthday.month);
        }
    }
}

// Сортировка по фамилии
void book_sort_by_lastname(AddressBook *book) {
    if (book == NULL) return;
    
    for (int i = 0; i < book->count - 1; i++) {
        for (int j = 0; j < book->count - 1 - i; j++) {
            if (strcmp(book->contacts[j].last_name, book->contacts[j + 1].last_name) > 0) {
                Contact temp = book->contacts[j];
                book->contacts[j] = book->contacts[j + 1];
                book->contacts[j + 1] = temp;
            }
        }
    }
}

// Функция MAIN 

int main(void) {
    AddressBook book;
    book_init(&book);
    
    // Создание контакта 1
    Contact contact1 = {
        .first_name = "Пётр",
        .last_name = "Иванов",
        .email = "petrov@email.com",
        .phones = {
            {PHONE_MOBILE, "+7-999-123-4567"},
            {PHONE_WORK, "+7-495-123-4567"}
        },
        .phone_count = 2,
        .birthday = {15, 3, 1990}
    };
    book_add(&book, &contact1);
    
    // Создание контакта 2
    Contact contact2 = {
        .first_name = "Мария",
        .last_name = "Сидорова",
        .email = "sidorova@email.com",
        .phones = {
            {PHONE_HOME, "+7-495-987-6543"}
        },
        .phone_count = 1,
        .birthday = {22, 7, 1985}
    };
    book_add(&book, &contact2);
    
    // Вывод всех контактов
    book_print(&book);
    
    // Поиск по фамилии
    int idx = book_find_by_lastname(&book, "Иванов");
    printf("Поиск 'Иванов': индекс %d\n", idx);
    
    // Контакты с днём рождения в марте
    book_print_birthdays(&book, 3);
    
    // Сортировка по фамилии
    book_sort_by_lastname(&book);
    printf("\nПосле сортировки по фамилии:\n");
    book_print(&book);
    
    // Удаление контакта
    book_remove(&book, 0);
    printf("\nПосле удаления первого контакта:\n");
    book_print(&book);
    
    return 0;
}
```
### Результаты работы
![Результат выполнения](practice-6/screenshots/task-6-1.png)
---
## Задание 6.2: Геометрические фигуры (Tagged Union)
### Постановка задачи
Реализуйте систему для работы с различными геометрическими фигурами.
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| s | Shape | Геометрическая фигура |
| point | Point | Точка |
| circle | Circle | Круг |
| rectangle | Rectangle | Прямоугольник |
| triangle | Triangle | Треугольник |
### Код программы
```c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Структуры

typedef enum {
    SHAPE_POINT,
    SHAPE_CIRCLE,
    SHAPE_RECTANGLE,
    SHAPE_TRIANGLE,
    SHAPE_POLYGON
} ShapeType;

typedef struct {
    double x, y;
} Point;

typedef struct {
    Point center;
    double radius;
} Circle;

typedef struct {
    Point top_left;
    double width, height;
} Rectangle;

typedef struct {
    Point a, b, c;
} Triangle;

typedef struct {
    Point *vertices;
    int count;
} Polygon;

typedef struct {
    ShapeType type;
    union {
        Point point;
        Circle circle;
        Rectangle rectangle;
        Triangle triangle;
        Polygon polygon;
    } data;
} Shape;

// Конструкторы

Shape shape_point(double x, double y) {
    Shape s;
    s.type = SHAPE_POINT;
    s.data.point.x = x;
    s.data.point.y = y;
    return s;
}

Shape shape_circle(double cx, double cy, double radius) {
    Shape s;
    s.type = SHAPE_CIRCLE;
    s.data.circle.center.x = cx;
    s.data.circle.center.y = cy;
    s.data.circle.radius = radius;
    return s;
}

Shape shape_rectangle(double x, double y, double w, double h) {
    Shape s;
    s.type = SHAPE_RECTANGLE;
    s.data.rectangle.top_left.x = x;
    s.data.rectangle.top_left.y = y;
    s.data.rectangle.width = w;
    s.data.rectangle.height = h;
    return s;
}

Shape shape_triangle(Point a, Point b, Point c) {
    Shape s;
    s.type = SHAPE_TRIANGLE;
    s.data.triangle.a = a;
    s.data.triangle.b = b;
    s.data.triangle.c = c;
    return s;
}

// Вычисления

double shape_area(const Shape *s) {
    if (s == NULL) return 0.0;
    
    switch (s->type) {
        case SHAPE_POINT:
            return 0.0;
            
        case SHAPE_CIRCLE:
            return M_PI * s->data.circle.radius * s->data.circle.radius;
            
        case SHAPE_RECTANGLE:
            return s->data.rectangle.width * s->data.rectangle.height;
            
        case SHAPE_TRIANGLE: {
            Triangle t = s->data.triangle;
            // Формула Герона
            double a = hypot(t.b.x - t.a.x, t.b.y - t.a.y);
            double b = hypot(t.c.x - t.b.x, t.c.y - t.b.y);
            double c = hypot(t.a.x - t.c.x, t.a.y - t.c.y);
            double p = (a + b + c) / 2.0;
            return sqrt(p * (p - a) * (p - b) * (p - c));
        }
            
        case SHAPE_POLYGON:
            // Упрощённая версия для многоугольника
            return 0.0;
            
        default:
            return 0.0;
    }
}

double shape_perimeter(const Shape *s) {
    if (s == NULL) return 0.0;
    
    switch (s->type) {
        case SHAPE_POINT:
            return 0.0;
            
        case SHAPE_CIRCLE:
            return 2 * M_PI * s->data.circle.radius;
            
        case SHAPE_RECTANGLE:
            return 2 * (s->data.rectangle.width + s->data.rectangle.height);
            
        case SHAPE_TRIANGLE: {
            Triangle t = s->data.triangle;
            double a = hypot(t.b.x - t.a.x, t.b.y - t.a.y);
            double b = hypot(t.c.x - t.b.x, t.c.y - t.b.y);
            double c = hypot(t.a.x - t.c.x, t.a.y - t.c.y);
            return a + b + c;
        }
            
        default:
            return 0.0;
    }
}

Point shape_center(const Shape *s) {
    Point center = {0, 0};
    if (s == NULL) return center;
    
    switch (s->type) {
        case SHAPE_POINT:
            center = s->data.point;
            break;
            
        case SHAPE_CIRCLE:
            center = s->data.circle.center;
            break;
            
        case SHAPE_RECTANGLE:
            center.x = s->data.rectangle.top_left.x + s->data.rectangle.width / 2;
            center.y = s->data.rectangle.top_left.y + s->data.rectangle.height / 2;
            break;
            
        case SHAPE_TRIANGLE: {
            Triangle t = s->data.triangle;
            center.x = (t.a.x + t.b.x + t.c.x) / 3;
            center.y = (t.a.y + t.b.y + t.c.y) / 3;
            break;
        }
            
        default:
            break;
    }
    return center;
}

// Трансформации

void shape_move(Shape *s, double dx, double dy) {
    if (s == NULL) return;
    
    switch (s->type) {
        case SHAPE_POINT:
            s->data.point.x += dx;
            s->data.point.y += dy;
            break;
            
        case SHAPE_CIRCLE:
            s->data.circle.center.x += dx;
            s->data.circle.center.y += dy;
            break;
            
        case SHAPE_RECTANGLE:
            s->data.rectangle.top_left.x += dx;
            s->data.rectangle.top_left.y += dy;
            break;
            
        case SHAPE_TRIANGLE:
            s->data.triangle.a.x += dx;
            s->data.triangle.a.y += dy;
            s->data.triangle.b.x += dx;
            s->data.triangle.b.y += dy;
            s->data.triangle.c.x += dx;
            s->data.triangle.c.y += dy;
            break;
            
        default:
            break;
    }
}

void shape_scale(Shape *s, double factor) {
    if (s == NULL || factor <= 0) return;
    
    switch (s->type) {
        case SHAPE_CIRCLE:
            s->data.circle.radius *= factor;
            break;
            
        case SHAPE_RECTANGLE:
            s->data.rectangle.width *= factor;
            s->data.rectangle.height *= factor;
            break;
            
        default:
            break;
    }
}

// Вывод

void shape_print(const Shape *s) {
    if (s == NULL) return;
    
    switch (s->type) {
        case SHAPE_POINT:
            printf("Точка: (%.2f, %.2f)\n", s->data.point.x, s->data.point.y);
            break;
            
        case SHAPE_CIRCLE:
            printf("Круг: центр (%.2f, %.2f), радиус %.2f\n", 
                   s->data.circle.center.x, s->data.circle.center.y, s->data.circle.radius);
            printf("  Площадь: %.2f, Периметр: %.2f\n", shape_area(s), shape_perimeter(s));
            break;
            
        case SHAPE_RECTANGLE:
            printf("Прямоугольник: (%.2f, %.2f), %.2f x %.2f\n",
                   s->data.rectangle.top_left.x, s->data.rectangle.top_left.y,
                   s->data.rectangle.width, s->data.rectangle.height);
            printf("  Площадь: %.2f, Периметр: %.2f\n", shape_area(s), shape_perimeter(s));
            break;
            
        case SHAPE_TRIANGLE:
            printf("Треугольник: (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f)\n",
                   s->data.triangle.a.x, s->data.triangle.a.y,
                   s->data.triangle.b.x, s->data.triangle.b.y,
                   s->data.triangle.c.x, s->data.triangle.c.y);
            printf("  Площадь: %.2f, Периметр: %.2f\n", shape_area(s), shape_perimeter(s));
            break;
            
        default:
            printf("Неизвестная фигура\n");
            break;
    }
}

// Функция MAIN 

int main(void) {
    Shape circle = shape_circle(0, 0, 5);
    Shape rect = shape_rectangle(0, 0, 4, 3);
    
    Point a = {0, 0};
    Point b = {4, 0};
    Point c = {2, 3};
    Shape triangle = shape_triangle(a, b, c);
    
    printf("=== Геометрические фигуры ===\n\n");
    
    shape_print(&circle);
    printf("\n");
    shape_print(&rect);
    printf("\n");
    shape_print(&triangle);
    
    // Трансформации
    shape_move(&circle, 2, 3);
    shape_scale(&rect, 2);
    
    printf("\n=== После трансформаций ===\n\n");
    printf("Круг после перемещения:\n");
    shape_print(&circle);
    printf("\nПрямоугольник после масштабирования:\n");
    shape_print(&rect);
    
    return 0;
}
```
### Результаты работы
![Результат выполнения](practice-6/screenshots/task-6-2.png)
---
## Задание 6.3: Выражения калькулятора (AST)
### Постановка задачи
Реализуйте структуру данных для представления математических выражений.
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| expr | Expr* | Выражение |
| ctx | Context | Контекст переменных |
| derivative | Expr* | Производная |
### Код программы
```c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Структуры

typedef enum {
    EXPR_NUMBER,
    EXPR_VARIABLE,
    EXPR_ADD,
    EXPR_SUB,
    EXPR_MUL,
    EXPR_DIV,
    EXPR_NEG,
    EXPR_SQRT,
    EXPR_POW
} ExprType;

typedef struct Expr Expr;

struct Expr {
    ExprType type;
    union {
        double number;
        char variable;
        struct {
            Expr *left;
            Expr *right;
        } binary;
        Expr *operand;
    } data;
};

// Контекст для переменных

typedef struct {
    char name;
    double value;
} Variable;

typedef struct {
    Variable vars[26];
    int count;
} Context;

// Конструкторы

Expr* expr_number(double value) {
    Expr *e = malloc(sizeof(Expr));
    e->type = EXPR_NUMBER;
    e->data.number = value;
    return e;
}

Expr* expr_variable(char name) {
    Expr *e = malloc(sizeof(Expr));
    e->type = EXPR_VARIABLE;
    e->data.variable = name;
    return e;
}

Expr* expr_add(Expr *left, Expr *right) {
    Expr *e = malloc(sizeof(Expr));
    e->type = EXPR_ADD;
    e->data.binary.left = left;
    e->data.binary.right = right;
    return e;
}

Expr* expr_sub(Expr *left, Expr *right) {
    Expr *e = malloc(sizeof(Expr));
    e->type = EXPR_SUB;
    e->data.binary.left = left;
    e->data.binary.right = right;
    return e;
}

Expr* expr_mul(Expr *left, Expr *right) {
    Expr *e = malloc(sizeof(Expr));
    e->type = EXPR_MUL;
    e->data.binary.left = left;
    e->data.binary.right = right;
    return e;
}

Expr* expr_div(Expr *left, Expr *right) {
    Expr *e = malloc(sizeof(Expr));
    e->type = EXPR_DIV;
    e->data.binary.left = left;
    e->data.binary.right = right;
    return e;
}

// Вычисление

double expr_eval(const Expr *e, const Context *ctx) {
    if (e == NULL) return 0;
    
    switch (e->type) {
        case EXPR_NUMBER:
            return e->data.number;
            
        case EXPR_VARIABLE: {
            char var = e->data.variable;
            for (int i = 0; i < ctx->count; i++) {
                if (ctx->vars[i].name == var) {
                    return ctx->vars[i].value;
                }
            }
            return 0;
        }
            
        case EXPR_ADD:
            return expr_eval(e->data.binary.left, ctx) + expr_eval(e->data.binary.right, ctx);
            
        case EXPR_SUB:
            return expr_eval(e->data.binary.left, ctx) - expr_eval(e->data.binary.right, ctx);
            
        case EXPR_MUL:
            return expr_eval(e->data.binary.left, ctx) * expr_eval(e->data.binary.right, ctx);
            
        case EXPR_DIV:
            return expr_eval(e->data.binary.left, ctx) / expr_eval(e->data.binary.right, ctx);
            
        default:
            return 0;
    }
}

// Вывод в строку

void expr_print(const Expr *e) {
    if (e == NULL) return;
    
    switch (e->type) {
        case EXPR_NUMBER:
            printf("%g", e->data.number);
            break;
            
        case EXPR_VARIABLE:
            printf("%c", e->data.variable);
            break;
            
        case EXPR_ADD:
            printf("(");
            expr_print(e->data.binary.left);
            printf(" + ");
            expr_print(e->data.binary.right);
            printf(")");
            break;
            
        case EXPR_SUB:
            printf("(");
            expr_print(e->data.binary.left);
            printf(" - ");
            expr_print(e->data.binary.right);
            printf(")");
            break;
            
        case EXPR_MUL:
            printf("(");
            expr_print(e->data.binary.left);
            printf(" * ");
            expr_print(e->data.binary.right);
            printf(")");
            break;
            
        case EXPR_DIV:
            printf("(");
            expr_print(e->data.binary.left);
            printf(" / ");
            expr_print(e->data.binary.right);
            printf(")");
            break;
            
        default:
            printf("?");
            break;
    }
}

// Освобождение памяти

void expr_free(Expr *e) {
    if (e == NULL) return;
    
    switch (e->type) {
        case EXPR_ADD:
        case EXPR_SUB:
        case EXPR_MUL:
        case EXPR_DIV:
            expr_free(e->data.binary.left);
            expr_free(e->data.binary.right);
            break;
            
        default:
            break;
    }
    free(e);
}

// Контекст

void context_init(Context *ctx) {
    if (ctx == NULL) return;
    ctx->count = 0;
    memset(ctx->vars, 0, sizeof(ctx->vars));
}

void context_set(Context *ctx, char name, double value) {
    if (ctx == NULL) return;
    
    for (int i = 0; i < ctx->count; i++) {
        if (ctx->vars[i].name == name) {
            ctx->vars[i].value = value;
            return;
        }
    }
    
    if (ctx->count < 26) {
        ctx->vars[ctx->count].name = name;
        ctx->vars[ctx->count].value = value;
        ctx->count++;
    }
}

double context_get(const Context *ctx, char name) {
    if (ctx == NULL) return 0;
    
    for (int i = 0; i < ctx->count; i++) {
        if (ctx->vars[i].name == name) {
            return ctx->vars[i].value;
        }
    }
    return 0;
}

// Функция MAIN 

int main(void) {
    // Создаём выражение: (x + 3) * (x - 2)
    Expr *x = expr_variable('x');
    Expr *three = expr_number(3);
    Expr *two = expr_number(2);
    
    Expr *left = expr_add(x, three);
    Expr *right = expr_sub(x, two);
    Expr *expr = expr_mul(left, right);
    
    printf("Выражение: ");
    expr_print(expr);
    printf("\n\n");
    
    // Контекст с переменной x
    Context ctx;
    context_init(&ctx);
    context_set(&ctx, 'x', 5);
    
    double result = expr_eval(expr, &ctx);
    printf("При x = 5: %g\n", result);
    
    return 0;
}
```
### Результаты работы
![Результат выполнения](practice-6/screenshots/task-6-3.png)
---
## Задание 6.4: Система сообщений (Protocol Buffers style)
### Постановка задачи
Реализуйте систему обмена сообщениями различных типов.
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| msg | Message | Сообщение |
| type | MessageType | Тип сообщения |
### Код программы
```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Структуры

typedef enum {
    MSG_TEXT,
    MSG_IMAGE,
    MSG_FILE,
    MSG_LOCATION,
    MSG_AUDIO
} MessageType;

typedef struct {
    char content[500];
    int is_edited;
} TextMessage;

typedef struct {
    char filename[100];
    int width;
    int height;
    size_t size_bytes;
} ImageMessage;

typedef struct {
    char filename[100];
    char mime_type[50];
    size_t size_bytes;
} FileMessage;

typedef struct {
    double latitude;
    double longitude;
    char address[200];
} LocationMessage;

typedef struct {
    char filename[100];
    int duration_seconds;
    size_t size_bytes;
} AudioMessage;

typedef struct {
    unsigned int id;
    unsigned int sender_id;
    unsigned int chat_id;
    time_t timestamp;
    MessageType type;
    union {
        TextMessage text;
        ImageMessage image;
        FileMessage file;
        LocationMessage location;
        AudioMessage audio;
    } content;
} Message;

// Создание сообщений

Message create_text_message(unsigned int sender, unsigned int chat, const char *text) {
    Message msg;
    msg.id = rand();
    msg.sender_id = sender;
    msg.chat_id = chat;
    msg.timestamp = time(NULL);
    msg.type = MSG_TEXT;
    strncpy(msg.content.text.content, text, 499);
    msg.content.text.content[499] = '\0';
    msg.content.text.is_edited = 0;
    return msg;
}

Message create_image_message(unsigned int sender, unsigned int chat, 
                              const char *filename, int w, int h, size_t size) {
    Message msg;
    msg.id = rand();
    msg.sender_id = sender;
    msg.chat_id = chat;
    msg.timestamp = time(NULL);
    msg.type = MSG_IMAGE;
    strncpy(msg.content.image.filename, filename, 99);
    msg.content.image.filename[99] = '\0';
    msg.content.image.width = w;
    msg.content.image.height = h;
    msg.content.image.size_bytes = size;
    return msg;
}

Message create_location_message(unsigned int sender, unsigned int chat,
                                 double lat, double lon, const char *address) {
    Message msg;
    msg.id = rand();
    msg.sender_id = sender;
    msg.chat_id = chat;
    msg.timestamp = time(NULL);
    msg.type = MSG_LOCATION;
    msg.content.location.latitude = lat;
    msg.content.location.longitude = lon;
    strncpy(msg.content.location.address, address, 199);
    msg.content.location.address[199] = '\0';
    return msg;
}

// Вывод

const char* message_type_to_string(MessageType type) {
    switch (type) {
        case MSG_TEXT:     return "Текст";
        case MSG_IMAGE:    return "Изображение";
        case MSG_FILE:     return "Файл";
        case MSG_LOCATION: return "Локация";
        case MSG_AUDIO:    return "Аудио";
        default:           return "Неизвестно";
    }
}

void message_print(const Message *msg) {
    if (msg == NULL) return;
    
    struct tm *tm_info = localtime(&msg->timestamp);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%H:%M", tm_info);
    
    printf("[%s] User#%u: ", time_str, msg->sender_id);
    
    switch (msg->type) {
        case MSG_TEXT:
            printf("%s\n", msg->content.text.content);
            break;
            
        case MSG_IMAGE:
            printf("[Изображение: %s (%dx%d, %.1f KB)]\n", 
                   msg->content.image.filename,
                   msg->content.image.width, msg->content.image.height,
                   msg->content.image.size_bytes / 1024.0);
            break;
            
        case MSG_LOCATION:
            printf("[Локация: %.4f, %.4f - %s]\n",
                   msg->content.location.latitude,
                   msg->content.location.longitude,
                   msg->content.location.address);
            break;
            
        default:
            printf("[Сообщение типа %s]\n", message_type_to_string(msg->type));
            break;
    }
}

// Фильтрация массива сообщений

size_t filter_messages_by_type(const Message *messages, size_t count, 
                                MessageType type, Message *result) {
    size_t found = 0;
    for (size_t i = 0; i < count; i++) {
        if (messages[i].type == type) {
            result[found++] = messages[i];
        }
    }
    return found;
}

size_t filter_messages_by_sender(const Message *messages, size_t count,
                                  unsigned int sender_id, Message *result) {
    size_t found = 0;
    for (size_t i = 0; i < count; i++) {
        if (messages[i].sender_id == sender_id) {
            result[found++] = messages[i];
        }
    }
    return found;
}

// Функция MAIN

int main(void) {
    srand(time(NULL));
    
    Message chat[100];
    int msg_count = 0;
    
    chat[msg_count++] = create_text_message(1, 42, "Привет! Как дела?");
    chat[msg_count++] = create_text_message(2, 42, "Отлично! Смотри что нашёл:");
    chat[msg_count++] = create_image_message(2, 42, "photo.jpg", 1920, 1080, 2500000);
    chat[msg_count++] = create_text_message(1, 42, "Круто! Где это?");
    chat[msg_count++] = create_location_message(2, 42, 55.7558, 37.6173, "Москва, Красная площадь");
    
    printf("=== Чат #42 ===\n");
    for (int i = 0; i < msg_count; i++) {
        message_print(&chat[i]);
    }
    
    // Фильтрация текстовых сообщений
    Message text_msgs[100];
    size_t text_count = filter_messages_by_type(chat, msg_count, MSG_TEXT, text_msgs);
    
    printf("\n=== Текстовые сообщения ===\n");
    for (size_t i = 0; i < text_count; i++) {
        message_print(&text_msgs[i]);
    }
    
    return 0;
}
```
### Результаты работы
![Результат выполнения](practice-6/screenshots/task-6-4.png)
---
## Задание 6.5: Битовые флаги
### Постановка задачи
Реализуйте систему прав доступа с использованием битовых флагов.
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| p | Permission | Битовая маска прав |
| user | User | Пользователь |
| resource | Resource | Ресурс |
### Код программы
```c
#include <stdio.h>
#include <string.h>

// Вариант 1: битовые флаги через enum

typedef enum {
    PERM_NONE    = 0,
    PERM_READ    = 1 << 0,  // 0001 
    PERM_WRITE   = 1 << 1,  // 0010 
    PERM_EXECUTE = 1 << 2,  // 0100 
    PERM_DELETE  = 1 << 3,  // 1000 
    PERM_ADMIN   = 1 << 4,  // 10000 
    PERM_ALL     = PERM_READ | PERM_WRITE | PERM_EXECUTE | PERM_DELETE | PERM_ADMIN
} Permission;

// Вариант 2: битовые поля 

typedef struct {
    unsigned int read    : 1;
    unsigned int write   : 1;
    unsigned int execute : 1;
    unsigned int delete  : 1;
    unsigned int admin   : 1;
    unsigned int reserved : 27;  // Резерв
} PermissionBits;

typedef struct {
    unsigned int id;
    char name[50];
    Permission permissions;
} User;

typedef struct {
    unsigned int id;
    char name[100];
    Permission required_permissions;
} Resource;

// Работа с правами

// Добавить право
void permission_add(Permission *p, Permission flag) {
    if (p == NULL) return;
    *p |= flag;
}

// Удалить право
void permission_remove(Permission *p, Permission flag) {
    if (p == NULL) return;
    *p &= ~flag;
}

// Проверить наличие права
int permission_has(Permission p, Permission flag) {
    return (p & flag) == flag;
}

// Проверить наличие всех прав
int permission_has_all(Permission p, Permission flags) {
    return (p & flags) == flags;
}

// Проверить наличие хотя бы одного права
int permission_has_any(Permission p, Permission flags) {
    return (p & flags) != 0;
}

// Преобразование прав в строку (формат: rwxda)
void permission_to_string(Permission p, char *buffer, size_t size) {
    if (buffer == NULL || size == 0) return;
    
    memset(buffer, 0, size);
    int pos = 0;
    
    if (pos < (int)size - 1) buffer[pos++] = permission_has(p, PERM_READ) ? 'r' : '-';
    if (pos < (int)size - 1) buffer[pos++] = permission_has(p, PERM_WRITE) ? 'w' : '-';
    if (pos < (int)size - 1) buffer[pos++] = permission_has(p, PERM_EXECUTE) ? 'x' : '-';
    if (pos < (int)size - 1) buffer[pos++] = permission_has(p, PERM_DELETE) ? 'd' : '-';
    if (pos < (int)size - 1) buffer[pos++] = permission_has(p, PERM_ADMIN) ? 'a' : '-';
    buffer[pos] = '\0';
}

// Преобразование строки в права
Permission string_to_permission(const char *str) {
    Permission p = PERM_NONE;
    if (str == NULL) return p;
    
    for (int i = 0; str[i] != '\0'; i++) {
        switch (str[i]) {
            case 'r': p |= PERM_READ; break;
            case 'w': p |= PERM_WRITE; break;
            case 'x': p |= PERM_EXECUTE; break;
            case 'd': p |= PERM_DELETE; break;
            case 'a': p |= PERM_ADMIN; break;
        }
    }
    return p;
}

// Проверка доступа

int user_can_access(const User *user, const Resource *resource) {
    if (user == NULL || resource == NULL) return 0;
    
    // Администратор имеет все права
    if (permission_has(user->permissions, PERM_ADMIN)) {
        return 1;
    }
    
    return permission_has_all(user->permissions, resource->required_permissions);
}

// Вывод

void user_print(const User *user) {
    if (user == NULL) return;
    
    char perm_str[10];
    permission_to_string(user->permissions, perm_str, sizeof(perm_str));
    
    printf("Пользователь: %s (ID: %u)\n", user->name, user->id);
    printf("  Права: %s\n", perm_str);
}

void resource_print(const Resource *resource) {
    if (resource == NULL) return;
    
    char perm_str[10];
    permission_to_string(resource->required_permissions, perm_str, sizeof(perm_str));
    
    printf("Ресурс: %s (ID: %u)\n", resource->name, resource->id);
    printf("  Требуемые права: %s\n", perm_str);
}

int main(void) {
    printf("========== ЗАДАНИЕ 6.5: БИТОВЫЕ ФЛАГИ ==========\n\n");
    
    // Создание пользователей
    User admin = {1, "admin", PERM_ADMIN | PERM_READ | PERM_WRITE | PERM_EXECUTE | PERM_DELETE};
    User editor = {2, "editor", PERM_READ | PERM_WRITE};
    User viewer = {3, "viewer", PERM_READ};
    
    // Создание ресурса
    Resource secret_file = {100, "/data/secret.txt", PERM_READ | PERM_WRITE};
    
    // Вывод информации
    printf("=== ИНФОРМАЦИЯ ===\n");
    user_print(&admin);
    user_print(&editor);
    user_print(&viewer);
    printf("\n");
    resource_print(&secret_file);
    
    printf("\n=== ПРОВЕРКА ДОСТУПА ===\n");
    printf("Ресурс: %s (требует rw-)\n\n", secret_file.name);
    
    // Проверка доступа
    printf("admin -> %s\n", user_can_access(&admin, &secret_file) ? "РАЗРЕШЁН" : "ЗАПРЕЩЁН");
    printf("editor -> %s\n", user_can_access(&editor, &secret_file) ? "РАЗРЕШЁН" : "ЗАПРЕЩЁН");
    printf("viewer -> %s (нет права на запись)\n", 
           user_can_access(&viewer, &secret_file) ? "РАЗРЕШЁН" : "ЗАПРЕЩЁН");
    
    // Демонстрация работы с правами
    printf("\n=== ДЕМОНСТРАЦИЯ ОПЕРАЦИЙ С ПРАВАМИ ===\n");
    
    Permission p = PERM_READ | PERM_WRITE;
    char buf[10];
    
    permission_to_string(p, buf, sizeof(buf));
    printf("Начальные права: %s\n", buf);
    
    permission_add(&p, PERM_EXECUTE);
    permission_to_string(p, buf, sizeof(buf));
    printf("После добавления execute: %s\n", buf);
    
    permission_remove(&p, PERM_WRITE);
    permission_to_string(p, buf, sizeof(buf));
    printf("После удаления write: %s\n", buf);
    
    printf("\nПроверки:\n");
    printf("  has_read: %s\n", permission_has(p, PERM_READ) ? "да" : "нет");
    printf("  has_write: %s\n", permission_has(p, PERM_WRITE) ? "да" : "нет");
    printf("  has_execute: %s\n", permission_has(p, PERM_EXECUTE) ? "да" : "нет");
    printf("  has_all(rwx): %s\n", permission_has_all(p, PERM_READ | PERM_WRITE | PERM_EXECUTE) ? "да" : "нет");
    printf("  has_any(rwd): %s\n", permission_has_any(p, PERM_READ | PERM_WRITE | PERM_DELETE) ? "да" : "нет");
    
    // Демонстрация битовых полей
    printf("\n=== БИТОВЫЕ ПОЛЯ ===\n");
    printf("Размер структуры PermissionBits: %zu байт\n", sizeof(PermissionBits));
    printf("Размер перечисления Permission: %zu байт\n", sizeof(Permission));
    
    PermissionBits bits = {1, 1, 0, 0, 0, 0};  // read=1, write=1
    printf("Битовые поля: read=%u, write=%u, execute=%u, delete=%u, admin=%u\n",
           bits.read, bits.write, bits.execute, bits.delete, bits.admin);
    
    return 0;
}
```
### Результаты работы
![Результат выполнения](practice-6/screenshots/task-6-5.png)
---
## Задание 6.6: Конечный автомат.
### Постановка задачи
Реализуйте конечный автомат для разбора простого языка.
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| lex | Lexer | Лексер |
| token | Token | Токен |
### Код программы
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Структуры

typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    union {
        double number;
        char identifier[64];
        char operator;
    } value;
    int line;
    int column;
} Token;

typedef enum {
    STATE_START,
    STATE_IN_NUMBER,
    STATE_IN_IDENTIFIER,
    STATE_IN_OPERATOR,
    STATE_ERROR,
    STATE_DONE
} LexerState;

typedef struct {
    const char *input;
    size_t pos;
    int line;
    int column;
    LexerState state;
} Lexer;

// Инициализация лексера

void lexer_init(Lexer *lex, const char *input) {
    if (lex == NULL) return;
    lex->input = input;
    lex->pos = 0;
    lex->line = 1;
    lex->column = 1;
    lex->state = STATE_START;
}

// Пропустить пробельные символы

void lexer_skip_whitespace(Lexer *lex) {
    if (lex == NULL) return;
    
    while (lex->input[lex->pos] != '\0') {
        char c = lex->input[lex->pos];
        if (c == ' ' || c == '\t') {
            lex->pos++;
            lex->column++;
        } else if (c == '\n') {
            lex->pos++;
            lex->line++;
            lex->column = 1;
        } else {
            break;
        }
    }
}

// Получить следующий токен

Token lexer_next_token(Lexer *lex) {
    Token token;
    token.type = TOKEN_ERROR;
    token.line = lex->line;
    token.column = lex->column;
    
    if (lex == NULL) return token;
    
    // Пропускаем пробелы
    lexer_skip_whitespace(lex);
    
    token.line = lex->line;
    token.column = lex->column;
    
    char c = lex->input[lex->pos];
    
    // Конец файла
    if (c == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    // Числа
    if (isdigit(c) || (c == '.' && isdigit(lex->input[lex->pos + 1]))) {
        lex->state = STATE_IN_NUMBER;
        
        char num_str[64];
        int i = 0;
        
        // Целая часть
        while (isdigit(lex->input[lex->pos])) {
            num_str[i++] = lex->input[lex->pos];
            lex->pos++;
            lex->column++;
        }
        
        // Дробная часть
        if (lex->input[lex->pos] == '.') {
            num_str[i++] = '.';
            lex->pos++;
            lex->column++;
            
            while (isdigit(lex->input[lex->pos])) {
                num_str[i++] = lex->input[lex->pos];
                lex->pos++;
                lex->column++;
            }
        }
        
        num_str[i] = '\0';
        token.type = TOKEN_NUMBER;
        token.value.number = atof(num_str);
        
        return token;
    }
    
    // Идентификаторы (буквы)
    if (isalpha(c)) {
        lex->state = STATE_IN_IDENTIFIER;
        
        char ident[64];
        int i = 0;
        
        while (isalnum(lex->input[lex->pos])) {
            ident[i++] = lex->input[lex->pos];
            lex->pos++;
            lex->column++;
        }
        
        ident[i] = '\0';
        token.type = TOKEN_IDENTIFIER;
        strcpy(token.value.identifier, ident);
        
        return token;
    }
    
    // Операторы и скобки
    lex->state = STATE_IN_OPERATOR;
    lex->pos++;
    lex->column++;
    
    switch (c) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '=':
            token.type = TOKEN_OPERATOR;
            token.value.operator = c;
            break;
        case '(':
            token.type = TOKEN_LPAREN;
            break;
        case ')':
            token.type = TOKEN_RPAREN;
            break;
        default:
            token.type = TOKEN_ERROR;
            break;
    }
    
    return token;
}

// Посмотреть следующий токен без извлечения

Token lexer_peek(Lexer *lex) {
    if (lex == NULL) {
        Token error = {TOKEN_ERROR, .line = 0, .column = 0};
        return error;
    }
    
    // Сохраняем состояние
    size_t saved_pos = lex->pos;
    int saved_line = lex->line;
    int saved_column = lex->column;
    LexerState saved_state = lex->state;
    
    // Получаем следующий токен
    Token token = lexer_next_token(lex);
    
    // Восстанавливаем состояние
    lex->pos = saved_pos;
    lex->line = saved_line;
    lex->column = saved_column;
    lex->state = saved_state;
    
    return token;
}

// Вывод токена

void token_print(const Token *tok) {
    if (tok == NULL) return;
    
    const char *type_str;
    switch (tok->type) {
        case TOKEN_NUMBER:     type_str = "NUMBER"; break;
        case TOKEN_IDENTIFIER: type_str = "IDENTIFIER"; break;
        case TOKEN_OPERATOR:   type_str = "OPERATOR"; break;
        case TOKEN_LPAREN:     type_str = "LPAREN"; break;
        case TOKEN_RPAREN:     type_str = "RPAREN"; break;
        case TOKEN_EOF:        type_str = "EOF"; break;
        default:               type_str = "ERROR"; break;
    }
    
    printf("[%s] ", type_str);
    
    switch (tok->type) {
        case TOKEN_NUMBER:
            printf("%g", tok->value.number);
            break;
        case TOKEN_IDENTIFIER:
            printf("%s", tok->value.identifier);
            break;
        case TOKEN_OPERATOR:
            printf("%c", tok->value.operator);
            break;
        case TOKEN_LPAREN:
            printf("(");
            break;
        case TOKEN_RPAREN:
            printf(")");
            break;
        default:
            break;
    }
    
    printf(" (line %d, col %d)\n", tok->line, tok->column);
}

// Токенизация всей строки

size_t tokenize(const char *input, Token *tokens, size_t max_tokens) {
    if (input == NULL || tokens == NULL) return 0;
    
    Lexer lex;
    lexer_init(&lex, input);
    
    size_t count = 0;
    
    while (count < max_tokens) {
        Token tok = lexer_next_token(&lex);
        tokens[count] = tok;
        count++;
        
        if (tok.type == TOKEN_EOF || tok.type == TOKEN_ERROR) {
            break;
        }
    }
    
    return count;
}

int main(void) {
    
    // Тестовые выражения
    const char *expressions[] = {
        "x = 3.14 * (y + 2)",
        "result = a + b - c * d / e",
        "sqrt(25) + pow(2, 3)",
        ""
    };
    
    for (int expr_idx = 0; expressions[expr_idx][0] != '\0'; expr_idx++) {
        printf("Вход: \"%s\"\n", expressions[expr_idx]);
        printf("Токены:\n");
        
        Lexer lex;
        lexer_init(&lex, expressions[expr_idx]);
        
        Token token;
        do {
            token = lexer_next_token(&lex);
            token_print(&token);
        } while (token.type != TOKEN_EOF && token.type != TOKEN_ERROR);
        
        printf("\n");
    }
    
    
    return 0;
}
```
### Результаты работы
![Результат выполнения](practice-6/screenshots/task-6-6.png)
