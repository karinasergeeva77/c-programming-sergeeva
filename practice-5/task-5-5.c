#include <stdio.h>
#include <stdbool.h>

// Определение типа Predicate
typedef int (*Predicate)(int);

// Линейный поиск
// Возвращает индекс или -1
int linear_search(const int *arr, size_t size, int target) {
    for (size_t i = 0; i < size; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

// Бинарный поиск (массив должен быть отсортирован)
int binary_search(const int *arr, size_t size, int target) {
    int left = 0;
    int right = size - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return mid;
        }
        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

// Поиск с предикатом — найти первый элемент, удовлетворяющий условию
int find_if(const int *arr, size_t size, Predicate pred) {
    for (size_t i = 0; i < size; i++) {
        if (pred(arr[i])) {
            return i;
        }
    }
    return -1;
}

// Подсчёт элементов, удовлетворяющих условию
size_t count_if(const int *arr, size_t size, Predicate pred) {
    size_t count = 0;
    for (size_t i = 0; i < size; i++) {
        if (pred(arr[i])) {
            count++;
        }
    }
    return count;
}

// Проверка: все ли элементы удовлетворяют условию
int all_of(const int *arr, size_t size, Predicate pred) {
    for (size_t i = 0; i < size; i++) {
        if (!pred(arr[i])) {
            return 0;  // false
        }
    }
    return 1;  // true
}

// Проверка: есть ли хотя бы один элемент, удовлетворяющий условию
int any_of(const int *arr, size_t size, Predicate pred) {
    for (size_t i = 0; i < size; i++) {
        if (pred(arr[i])) {
            return 1;  // true
        }
    }
    return 0;  // false
}

// Проверка: ни один элемент не удовлетворяет условию
int none_of(const int *arr, size_t size, Predicate pred) {
    for (size_t i = 0; i < size; i++) {
        if (pred(arr[i])) {
            return 0;  // false (есть хотя бы один)
        }
    }
    return 1;  // true (ни одного нет)
}


int is_even(int x) {
    return x % 2 == 0;
}

int is_odd(int x) {
    return x % 2 != 0;
}

int is_positive(int x) {
    return x > 0;
}

int is_negative(int x) {
    return x < 0;
}

int is_greater_than_10(int x) {
    return x > 10;
}

int is_less_than_10(int x) {
    return x < 10;
}

// Вывод массива

void print_array(const int *arr, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("\n");
}

int main(void) {
    // Тестовый массив (чётные числа от 2 до 20)
    int arr[] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    
    // Вывод массива
    printf("Массив: ");
    print_array(arr, size);
    
    // 1. Линейный поиск числа 10
    int target = 10;
    int index = linear_search(arr, size, target);
    printf("linear_search(%d): найден на позиции %d\n", target, index);
    
    // 2. Бинарный поиск числа 14
    target = 14;
    index = binary_search(arr, size, target);
    printf("binary_search(%d): найден на позиции %d\n", target, index);
    
    // 3. Подсчёт элементов, больших 10
    size_t count = count_if(arr, size, is_greater_than_10);
    printf("count_if(is_greater_than_10): %zu\n", count);
    
    // 4. Проверка: все ли элементы чётные
    int all_even = all_of(arr, size, is_even);
    printf("all_of(is_even): %s\n", all_even ? "true" : "false");
    
    // 5. Проверка: есть ли хотя бы один отрицательный элемент
    int has_negative = any_of(arr, size, is_negative);
    printf("any_of(is_negative): %s\n", has_negative ? "true" : "false");
    
    return 0;
}
