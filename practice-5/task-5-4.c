#include <stdio.h>
#include <stdlib.h>

typedef int (*Transform)(int);
typedef int (*Predicate)(int);
typedef int (*Reducer)(int, int);
typedef void (*Action)(int, size_t);

// Map – применить функцию к каждому элементу
void array_map(int *arr, size_t size, Transform func) {
    for (size_t i = 0; i < size; i++) {
        arr[i] = func(arr[i]);
    }
}

// Filter – отобрать элементы по условию
// Возвращает количество отобранных элементов
size_t array_filter(const int *src, size_t size, int *dst, Predicate pred) {
    size_t count = 0;
    for (size_t i = 0; i < size; i++) {
        if (pred(src[i])) {
            dst[count++] = src[i];
        }
    }
    return count;
}

// Reduce – свернуть массив в одно значение
int array_reduce(const int *arr, size_t size, int initial, Reducer func) {
    int result = initial;
    for (size_t i = 0; i < size; i++) {
        result = func(result, arr[i]);
    }
    return result;
}

// ForEach – выполнить действие для каждого элемента
typedef void (*Action)(int, size_t); // (element, index)
void array_foreach(const int *arr, size_t size, Action act) {
    for (size_t i = 0; i < size; i++) {
        act(arr[i], i);
    }
}

// Функции-трансформеры
int square(int x) { return x * x; }
int negate(int x) { return -x; }
int double_val(int x) { return x * 2; }
int abs_val(int x) { return x < 0 ? -x : x; }

// Предикаты
int is_even(int x) { return x % 2 == 0; }
int is_positive(int x) { return x > 0; }
int is_greater_than_10(int x) { return x > 10; }

// Редьюсеры
int sum(int a, int b) { return a + b; }
int product(int a, int b) { return a * b; }
int max(int a, int b) { return a > b ? a : b; }
int min(int a, int b) { return a < b ? a : b; }

// Действия для ForEach
void print_element(int element, size_t index) {
    printf("[%zu] = %d\n", index, element);
}

void print_summary(int element, size_t index) {
    if (index == 0) {
        printf("Элементы массива:\n");
    }
    printf("  %d\n", element);
}

int main(void) {
    int arr[] = {1, -2, 3, -4, 5, -6, 7, -8, 9, -10};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    
    printf("Исходный массив: ");
    for (size_t i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n\n");
    
    // Тестирование map
    int squared[10];
    for (size_t i = 0; i < size; i++) squared[i] = arr[i];
    array_map(squared, size, square);
    printf("После map(square): ");
    for (size_t i = 0; i < size; i++) {
        printf("%d ", squared[i]);
    }
    printf("\n");
    
    // Тестирование filter
    int positive[10];
    size_t pos_count = array_filter(arr, size, positive, is_positive);
    printf("После filter(is_positive): ");
    for (size_t i = 0; i < pos_count; i++) {
        printf("%d ", positive[i]);
    }
    printf("\n");
    
    // Тестирование reduce
    int sum_result = array_reduce(positive, pos_count, 0, sum);
    printf("Reduce(sum, 0): %d\n", sum_result);
    
    int product_result = array_reduce(positive, pos_count, 1, product);
    printf("Reduce(product, 1): %d\n", product_result);
    
    // Тестирование forEach
    printf("\nForEach:\n");
    array_foreach(positive, pos_count, print_element);
    
    // Дополнительные тесты
    int numbers[] = {3, 7, 2, 9, 4, 6, 1, 8, 5};
    size_t num_size = sizeof(numbers) / sizeof(numbers[0]);
    
    int max_val = array_reduce(numbers, num_size, numbers[0], max);
    int min_val = array_reduce(numbers, num_size, numbers[0], min);
    
    printf("\nМаксимум: %d\n", max_val);
    printf("Минимум: %d\n", min_val);
    
    return 0;
}
