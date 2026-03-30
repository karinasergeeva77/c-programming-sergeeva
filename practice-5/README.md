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
