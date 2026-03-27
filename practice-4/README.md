# Практическая работа 4: Функции и указатели
**Студент:** Сергеева Карина Алексеевна
**Группа:** 1зб_ИВТ(ускор.)/25
**Дата:** 27 марта 2026 г.
---
## Задание 4.1: Факториал (итеративно)
### Постановка задачи
Напишите функцию long long factorial(int n), которая вычисляет факториал числа с помощью цикла.
### Математическая модель
n!=1*2*3*...*n, 0!=1
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| n | int | Число для вычисления факториала |
| result | long long | Результат вычисления |
### Код программы
```c
#include <stdio.h>

// Функция вычисления факториала (итеративно)
long long factorial(int n) {
    if (n < 0) return 0;
    
    long long result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main(void) {
    int n;
    
    printf("Enter number: ");
    scanf("%d", &n);
    
    printf("%d! = %lld\n", n, factorial(n));
    
    return 0;
}
```
### Результаты работы
![Результат выполнения](practice-4/screenshots/task-4-1.png)
---
## Задание 4.2: Факториал (рекурсивно)
### Постановка задачи
Напишите рекурсивную функцию long long factorial_rec(int n).
### Математическая модель
n!=1, n<=1; n*(n-1)!, n>1
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| n | int | Число для вычисления факториала |
### Код программы
```c
#include <stdio.h>

// Рекурсивная функция вычисления факториала
long long factorial_rec(int n) {
    // База рекурсии
    if (n <= 1) {
        return 1;
    }
    // Рекурсивный вызов
    return n * factorial_rec(n - 1);
}

int main(void) {
    int n;
    
    printf("Enter number: ");
    scanf("%d", &n);
    
    printf("%d! = %lld\n", n, factorial_rec(n));
    
    return 0;
}
```
### Результаты работы
![Результат выполнения](practice-4/screenshots/task-4-2.png)
---
## Задание 4.3: Степень числа
### Постановка задачи
Напишите функцию double power(double base, int exp), которая возводит число в целую степень (включая отрицательную).
### Математическая модель
a^n=1, n=0; a*a^n-1, n>0; 1/a^-n, n<0
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| base | double | Основание степени |
| exp | int | Показатель степени |
| abs_exp | int | Абсолютное значение показателя |
### Код программы
```c
#include <stdio.h>

// Функция возведения в целую степень (включая отрицательную)
double power(double base, int exp) {
    if (exp == 0) return 1.0;
    
    double result = 1.0;
    int abs_exp = exp > 0 ? exp : -exp;
    
    for (int i = 0; i < abs_exp; i++) {
        result *= base;
    }
    
    // Если степень отрицательная, возвращаем 1/result
    return exp > 0 ? result : 1.0 / result;
}

int main(void) {
    printf("2^10 = %.0lf\n", power(2, 10));    // 1024
    printf("2^(-3) = %.3lf\n", power(2, -3));  // 0.125
    printf("5^0 = %.0lf\n", power(5, 0));      // 1
    
    return 0;
}
```
### Результаты работы
![Результат выполнения](practice-4/screenshots/task-4-3.png)
---
## Задание 4.4: Swap
### Постановка задачи
Напишите функцию void swap(int *a, int *b), которая меняет местами два числа через указатели.
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| a | int* | Указатель на первое число |
| b | int* | Указатель на второе число |
| temp | int | Временная переменная |
### Код программы
```c
#include <stdio.h>

// Функция обмена значениями через указатели
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main(void) {
    int x = 5, y = 10;
    
    printf("До swap: x = %d, y = %d\n", x, y);
    swap(&x, &y);
    printf("После swap: x = %d, y = %d\n", x, y);
    
    return 0;
}
```
### Результаты работы
![Результат выполнения](practice-4/screenshots/task-4-4.png)
---
## Задание 4.5: Максимум в массиве
### Постановка задачи
Напишите функцию int findMax(int *arr, int size), которая находит максимальный элемент массива.
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| arr | int* | Указатель на массив |
| size | int | Размер массива |
| max | int | Максимальный элемент |
### Код программы
```c
#include <stdio.h>

// Функция поиска максимального элемента
int findMax(int *arr, int size) {
    if (size <= 0) return 0;
    
    int max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

int main(void) {
    int arr[] = {5, 2, 9, 1, 7};
    int size = sizeof(arr) / sizeof(arr[0]);
    
    printf("Max: %d\n", findMax(arr, size));  // 9
    
    return 0;
}
```
### Результаты работы
![Результат выполнения](practice-4/screenshots/task-4-5.png)
---
## Задание 4.6: Индекс максимума
### Постановка задачи
Напишите функцию int findMaxIndex(int *arr, int size), которая возвращает индекс максимального элемента.
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| arr | int* | Указатель на массив |
| size | int | Размер массива |
| maxIndex | int | Индекс максимального элемента |
### Код программы
```c
#include <stdio.h>

// Функция поиска индекса максимального элемента
int findMaxIndex(int *arr, int size) {
    if (size <= 0) return -1;
    
    int maxIndex = 0;
    for (int i = 1; i < size; i++) {
        if (arr[i] > arr[maxIndex]) {
            maxIndex = i;
        }
    }
    return maxIndex;
}

int main(void) {
    int arr[] = {5, 2, 9, 1, 7};
    int size = sizeof(arr) / sizeof(arr[0]);
    
    printf("Max index: %d\n", findMaxIndex(arr, size));  // 2

    
    return 0;
}
```
### Результаты работы
![Результат выполнения](practice-4/screenshots/task-4-6.png)
---
## Задание 4.7: Длина строки
### Постановка задачи
Напишите функцию int my_strlen(char *str), которая возвращает длину строки (без использования strlen).
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| str | char* | Указатель на строку |
| length | int | Длина строки |
### Код программы
```c
#include <stdio.h>

// Функция вычисления длины строки (без strlen)
int my_strlen(char *str) {
    int length = 0;
    while (*str != '\0') {  // пока не встретили конец строки
        length++;
        str++;  // переходим к следующему символу
    }
    return length;
}

int main(void) {
    char str1[] = "Hello";
    char str2[] = "";
    
    printf("length \"%s\": %d\n", str1, my_strlen(str1));  // 5
    printf("length \"%s\": %d\n", str2, my_strlen(str2));  // 0
    
    return 0;
}
```
### Результаты работы
![Результат выполнения](practice-4/screenshots/task-4-7.png)
---
## Задание 4.8: Копирование строки
### Постановка задачи
Напишите функцию void my_strcpy(char *dest, char *src), которая копирует строку src в dest.
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| dest | char* | Указатель на строку-приёмник |
| src | char* | Указатель на исходную строку |
### Код программы
```c
#include <stdio.h>

// Функция копирования строки
void my_strcpy(char *dest, char *src) {
    while (*src != '\0') {
        *dest = *src;  // копируем символ
        dest++;
        src++;
    }
    *dest = '\0';  // добавляем завершающий нуль
}

int main(void) {
    char source[] = "Hello, World!";
    char destination[50];
    
    my_strcpy(destination, source);
    
    printf(" %s\n", destination);
    
    return 0;
}
```
### Результаты работы
![Результат выполнения](practice-4/screenshots/task-4-8.png)
---
## Задание 4.9: Реверс строки
### Постановка задачи
Напишите функцию void reverse(char *str), которая переворачивает строку "на месте".
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| str | char* | Указатель на строку |
| end | char* | Указатель на конец строки |
| temp | char | Временная переменная для обмена |
### Код программы
```c
#include <stdio.h>

// Функция переворота строки "на месте"
void reverse(char *str) {
    // Находим конец строки
    char *end = str;
    while (*end != '\0') {
        end++;
    }
    end--;  // указываем на последний символ
    
    // Меняем местами символы с начала и конца
    while (str < end) {
        char temp = *str;
        *str = *end;
        *end = temp;
        str++;
        end--;
    }
}

int main(void) {
    char str[] = "Hello";
    
    reverse(str);
    printf(" %s\n", str);  // olleH
    
    return 0;
}
```
### Результаты работы
![Результат выполнения](practice-4/screenshots/task-4-9.png)
---
## Задание 4.10: Минимум и максимум через указатели
### Постановка задачи
Напишите функцию void findMinMax(int *arr, int size, int *min, int *max), которая находит минимум и максимум одновременно и записывает их по указателям.
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| arr | int* | Указатель на массив |
| size | int | Размер массива |
| min | int* | Указатель для записи минимума |
| max | int* | Указатель для записи максимума |
### Код программы
```c
#include <stdio.h>

// Функция находит минимум и максимум одновременно
void findMinMax(int *arr, int size, int *min, int *max) {
    if (size <= 0) return;
    
    *min = arr[0];
    *max = arr[0];
    
    for (int i = 1; i < size; i++) {
        if (arr[i] < *min) {
            *min = arr[i];
        }
        if (arr[i] > *max) {
            *max = arr[i];
        }
    }
}

int main(void) {
    int arr[] = {5, 2, 9, 1, 7};
    int size = sizeof(arr) / sizeof(arr[0]);
    int min, max;
    
    findMinMax(arr, size, &min, &max);
    
    printf("Min: %d\n, Max: %d\n", min, max);  // 1, 9
    
    return 0;
}
```
### Результаты работы
![Результат выполнения](practice-4/screenshots/task-4-10.png)
---
## Задание 4.11: Сортировка массива
### Постановка задачи
Напишите функцию void sortArray(int *arr, int size), которая сортирует массив по возрастанию (любым методом).
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| arr | int* | Указатель на массив |
| size | int | Размер массива |
| temp | int | Временная переменная для обмена |
### Код программы
```c
#include <stdio.h>

// Функция сортировки массива (пузырьком)
void sortArray(int *arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    int arr[] = {64, 34, 25, 12, 22};
    int size = sizeof(arr) / sizeof(arr[0]);
    
    printf("До сортировки: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    sortArray(arr, size);
    
    printf("После сортировки: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    return 0;
}
```
### Результаты работы
![Результат выполнения](practice-4/screenshots/task4-11.png)
