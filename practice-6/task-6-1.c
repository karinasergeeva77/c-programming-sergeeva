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
