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
