# Практическая работа 5: Функции и массивы (продвинутый уровень)
**Студент:** Сергеева Карина Алексеевна
**Группа:** 1зб_ИВТ(ускор.)/25
**Дата:** 29 марта 2026 г.
---
## Задание 5.1: Статистика массива
### Постановка задачи
Напишите функцию, которая вычисляет статистику массива чисел с плавающей точкой.
### Математическая модель
Среднее арифметическое:
x=1/n∑(i=1,n) xi 	 
Дисперсия:
σ^2=1/n∑(i=1,n) (xi−x)^2
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| arr | const double* | Указатель на массив чисел |
| size | size_t | Размер массива |
| stats | ArrayStats | Структура с результатами |
### Код программы
```c
#include <stdio.h>
#include <math.h>

typedef struct {
    double min;
    double max;
    double sum;
    double average;
    double variance;  // Дисперсия
} ArrayStats;

// Функция вычисления статистики
ArrayStats calculate_stats(const double *arr, size_t size) {
    ArrayStats stats = {0, 0, 0, 0, 0};
    
    // Обработка пустого массива
    if (size == 0 || arr == NULL) {
        return stats;
    }
    
    // Инициализация min и max первым элементом
    stats.min = arr[0];
    stats.max = arr[0];
    stats.sum = 0;
    
    // Вычисление min, max, sum
    for (size_t i = 0; i < size; i++) {
        if (arr[i] < stats.min) {
            stats.min = arr[i];
        }
        if (arr[i] > stats.max) {
            stats.max = arr[i];
        }
        stats.sum += arr[i];
    }
    
    // Среднее арифметическое
    stats.average = stats.sum / size;
    
    // Дисперсия
    double variance_sum = 0;
    for (size_t i = 0; i < size; i++) {
        double diff = arr[i] - stats.average;
        variance_sum += diff * diff;
    }
    stats.variance = variance_sum / size;
    
    return stats;
}

int main(void) {
    double arr[] = {2.5, 4.0, 1.5, 3.0, 5.0};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    
    ArrayStats stats = calculate_stats(arr, size);
    
    printf("Массив: ");
    for (size_t i = 0; i < size; i++) {
        printf("%.2f ", arr[i]);
    }
    printf("\n\n");
    
    printf("Min: %.2f\n", stats.min);
    printf("Max: %.2f\n", stats.max);
    printf("Sum: %.2f\n", stats.sum);
    printf("Average: %.2f\n", stats.average);
    printf("Variance: %.2f\n", stats.variance);
    
    return 0;
}
```
### Результаты работы
![Результат выполнения]
---
## Задание 5.2: Операции с матрицами (VLA)
### Постановка задачи
Реализуйте набор функций для работы с матрицами произвольного размера.
### Математическая модель
Транспонирование: $(A^T){ij} = A{ji}$
Умножение матриц: $(AB){ij} = \sum{k=1}^{n} A_{ik} B_{kj}$
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| rows, cols | int | Размеры матрицы |
| m | int[rows][cols] | Матрица |
| min, max | int | Границы случайных чисел |
### Код программы
```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Заполнение матрицы случайными числами
void matrix_random(int rows, int cols, int m[rows][cols], int min, int max) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m[i][j] = min + rand() % (max - min + 1);
        }
    }
}

// Вывод матрицы
void matrix_print(int rows, int cols, int m[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d ", m[i][j]);
        }
        printf("\n");
    }
}

// Транспонирование матрицы
void matrix_transpose(int rows, int cols, int src[rows][cols], int dst[cols][rows]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dst[j][i] = src[i][j];
        }
    }
}

// Умножение матриц
int matrix_multiply(int r1, int c1, int a[r1][c1], 
                    int r2, int c2, int b[r2][c2], 
                    int result[r1][c2]) {
    // Проверка совместимости размеров
    if (c1 != r2) {
        return -1;  // Несовместимые размеры
    }
    
    // Обнуляем результат
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            result[i][j] = 0;
        }
    }
    
    // Умножение
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            for (int k = 0; k < c1; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return 0;
}

// Сумма элементов главной диагонали
int matrix_trace(int n, int m[n][n]) {
    int trace = 0;
    for (int i = 0; i < n; i++) {
        trace += m[i][i];
    }
    return trace;
}

int main(void) {
    srand(time(NULL));
    
    // Матрица A (2x3)
    int A[2][3] = {{1, 2, 3}, {4, 5, 6}};
    // Матрица B (3x2)
    int B[3][2] = {{7, 8}, {9, 10}, {11, 12}};
    // Результат (2x2)
    int C[2][2];
    
    printf("Матрица A (2x3):\n");
    matrix_print(2, 3, A);
    
    printf("\nМатрица B (3x2):\n");
    matrix_print(3, 2, B);
    
    matrix_multiply(2, 3, A, 3, 2, B, C);
    
    printf("\nA × B (2x2):\n");
    matrix_print(2, 2, C);
    
}
```
### Результаты работы
![Результат выполнения]
---
## Задание 5.3: Сортировка с компаратором
### Постановка задачи
Реализуйте универсальную функцию сортировки с callback-компаратором.
### Математическая модель
Сортировка пузырьком с пользовательским компаратором:
aj и aj+1 меняются местами, если cmp(aj,aj+1)>0
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| base | void* | Указатель на массив |
| count | size_t | Количество элементов |
| size | size_t | Размер элемента |
| cmp | Comparator | Функция сравнения |
### Код программы
```c
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
```
### Результаты работы
![Результат выполнения]
---
## Задание 5.4: Функциональные операции над массивами
### Постановка задачи
Реализуйте функции высшего порядка для работы с массивами.
### Математическая модель
Map: $y_i = f(x_i)$
Filter: ${x_i \mid P(x_i)}$
Reduce: $r = f(...f(f(r_0, x_0), x_1)...)$
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| arr | int* | Указатель на массив |
| size | size_t | Размер массива |
| func | Transform | Функция преобразования |
### Код программы
```c
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
```
### Результаты работы
![Результат выполнения]
---
## Задание 5.5: Поиск в массиве
### Постановка задачи
Реализуйте различные алгоритмы поиска.
### Математическая модель
Линейный поиск: $O(n)$
Бинарный поиск: $O(\log n)$ (требует отсортированный массив)
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| arr | const int* | Указатель на массив |
| size | size_t | Размер массива |
| target | int | Искомое значение |
| pred | Predicate | Функция-предикат |
### Код программы
```c
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
```
### Результаты работы
![Результат выполнения]
---
## Задание 5.6: Работа с двумерными массивами
### Постановка задачи
Реализуйте функции для обработки изображений (матрица яркости пикселей).
### Математическая модель
Инвертирование: $I' = 255 - I$
Пороговая фильтрация: $I' = \begin{cases} 255 & \text{если } I > T \ 0 & \text{иначе} \end{cases}$
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| rows, cols | int | Размеры изображения |
| img | int[rows][cols]| Изображение |
| threshold | int | Порог бинаризации |
### Код программы
```c
#include <stdio.h>

#define MAX_BRIGHTNESS 255

// Инвертирование (негатив)
void image_invert(int rows, int cols, int img[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            img[i][j] = MAX_BRIGHTNESS - img[i][j];
        }
    }
}

// Увеличение яркости
void image_brighten(int rows, int cols, int img[rows][cols], int delta) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            img[i][j] += delta;
            if (img[i][j] > MAX_BRIGHTNESS) {
                img[i][j] = MAX_BRIGHTNESS;
            }
            if (img[i][j] < 0) {
                img[i][j] = 0;
            }
        }
    }
}

// Пороговая фильтрация (бинаризация)
void image_threshold(int rows, int cols, int img[rows][cols], int threshold) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            img[i][j] = (img[i][j] > threshold) ? MAX_BRIGHTNESS : 0;
        }
    }
}

// Размытие (усреднение с соседями)
void image_blur(int rows, int cols, int src[rows][cols], int dst[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int sum = 0;
            int count = 0;
            
            // Проходим по соседям (3x3)
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    int ni = i + di;
                    int nj = j + dj;
                    
                    // Проверяем границы
                    if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
                        sum += src[ni][nj];
                        count++;
                    }
                }
            }
            
            dst[i][j] = sum / count;
        }
    }
}

// Поворот на 90° по часовой стрелке
void image_rotate_90(int rows, int cols, int src[rows][cols], int dst[cols][rows]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dst[j][rows - 1 - i] = src[i][j];
        }
    }
}

// Отражение по горизонтали
void image_flip_horizontal(int rows, int cols, int img[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols / 2; j++) {
            int temp = img[i][j];
            img[i][j] = img[i][cols - 1 - j];
            img[i][cols - 1 - j] = temp;
        }
    }
}

// Отражение по вертикали 
void image_flip_vertical(int rows, int cols, int img[rows][cols]) {
    for (int i = 0; i < rows / 2; i++) {
        for (int j = 0; j < cols; j++) {
            int temp = img[i][j];
            img[i][j] = img[rows - 1 - i][j];
            img[rows - 1 - i][j] = temp;
        }
    }
}

// Вывод изображения
void image_print(int rows, int cols, int img[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d ", img[i][j]);
        }
        printf("\n");
    }
}

// Копирование изображения
void image_copy(int rows, int cols, int src[rows][cols], int dst[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dst[i][j] = src[i][j];
        }
    }
}

int main(void) {
    // Исходное изображение 4x4
    int img[4][4] = {
        {100, 150, 200, 250},
        {50, 100, 150, 200},
        {0, 50, 100, 150},
        {25, 75, 125, 175}
    };
    
    printf("Исходное \"изображение\" 4x4:\n");
    image_print(4, 4, img);
    
    // Тестирование инвертирования
    int inverted[4][4];
    image_copy(4, 4, img, inverted);
    image_invert(4, 4, inverted);
    printf("\nПосле invert:\n");
    image_print(4, 4, inverted);
    
    // Тестирование пороговой фильтрации (threshold = 100)
    int thresholded[4][4];
    image_copy(4, 4, img, thresholded);
    image_threshold(4, 4, thresholded, 100);
    printf("\nПосле threshold(100):\n");
    image_print(4, 4, thresholded);
    
    return 0;
}
```
### Результаты работы
![Результат выполнения]
