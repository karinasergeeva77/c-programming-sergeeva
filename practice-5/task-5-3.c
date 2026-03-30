#include <stdio.h>
#include <string.h>

typedef int (*Comparator)(const void*, const void*);

// Сортировка пузырьком с компаратором
void bubble_sort(void *base, size_t count, size_t size, Comparator cmp) {
    for (size_t i = 0; i < count - 1; i++) {
        for (size_t j = 0; j < count - 1 - i; j++) {
            char *a = (char*)base + j * size;
            char *b = (char*)base + (j + 1) * size;
            
            if (cmp(a, b) > 0) {
                // Обмен местами
                char temp[size];
                memcpy(temp, a, size);
                memcpy(a, b, size);
                memcpy(b, temp, size);
            }
        }
    }
}

// Для int
int compare_int_asc(const void *a, const void *b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return ia - ib;
}

int compare_int_desc(const void *a, const void *b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return ib - ia;
}

// Для строк
int compare_str_asc(const void *a, const void *b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

int compare_str_length(const void *a, const void *b) {
    size_t len_a = strlen(*(const char**)a);
    size_t len_b = strlen(*(const char**)b);
    return len_a - len_b;
}

// Для структур
typedef struct {
    char name[50];
    int age;
    double salary;
} Employee;

int compare_employee_by_name(const void *a, const void *b) {
    return strcmp(((const Employee*)a)->name, ((const Employee*)b)->name);
}

int compare_employee_by_age(const void *a, const void *b) {
    return ((const Employee*)a)->age - ((const Employee*)b)->age;
}

int compare_employee_by_salary(const void *a, const void *b) {
    if (((const Employee*)a)->salary > ((const Employee*)b)->salary) return 1;
    if (((const Employee*)a)->salary < ((const Employee*)b)->salary) return -1;
    return 0;
}

// Функция вывода массива int
void print_int_array(int *arr, size_t n) {
    for (size_t i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Функция вывода массива строк
void print_str_array(char **arr, size_t n) {
    for (size_t i = 0; i < n; i++) {
        printf("%s\n", arr[i]);
    }
}

// Функция вывода массива сотрудников
void print_employees(Employee *emp, size_t n) {
    for (size_t i = 0; i < n; i++) {
        printf("%s, %d, %.2f\n", emp[i].name, emp[i].age, emp[i].salary);
    }
}

int main(void) {
    // Тестирование с int
    int numbers[] = {5, 2, 8, 1, 9};
    size_t n = sizeof(numbers) / sizeof(numbers[0]);
    
    printf("Исходный массив: ");
    print_int_array(numbers, n);
    
    bubble_sort(numbers, n, sizeof(int), compare_int_asc);
    printf("По возрастанию: ");
    print_int_array(numbers, n);
    
    bubble_sort(numbers, n, sizeof(int), compare_int_desc);
    printf("По убыванию: ");
    print_int_array(numbers, n);
    
    printf("\n");
    
    // Тестирование со строками
    char *fruits[] = {"banana", "apple", "cherry", "date", "elderberry"};
    size_t fruit_count = sizeof(fruits) / sizeof(fruits[0]);
    
    printf("Исходные строки:\n");
    print_str_array(fruits, fruit_count);
    
    bubble_sort(fruits, fruit_count, sizeof(char*), compare_str_asc);
    printf("\nПо алфавиту:\n");
    print_str_array(fruits, fruit_count);
    
    bubble_sort(fruits, fruit_count, sizeof(char*), compare_str_length);
    printf("\nПо длине:\n");
    print_str_array(fruits, fruit_count);
    
    printf("\n");
    
    // Тестирование со структурой Employee
    Employee employees[] = {
        {"Иван", 25, 45000.00},
        {"Мария", 30, 55000.00},
        {"Пётр", 28, 60000.00},
        {"Анна", 35, 50000.00}
    };
    size_t emp_count = sizeof(employees) / sizeof(employees[0]);
    
    printf("Исходные сотрудники:\n");
    print_employees(employees, emp_count);
    
    bubble_sort(employees, emp_count, sizeof(Employee), compare_employee_by_salary);
    printf("\nСотрудники по зарплате:\n");
    print_employees(employees, emp_count);
    
    bubble_sort(employees, emp_count, sizeof(Employee), compare_employee_by_name);
    printf("\nСотрудники по имени:\n");
    print_employees(employees, emp_count);
    
    return 0;
}
