# Практическая работа 12: Многопоточное программирование
**Студент:** Сергеева Карина Алексеевна
**Группа:** 1зб_ИВТ(ускор.)/25
**Дата:** 03 апреля 2026 г.
---
## Задание 1. Hello World с потоками
### Постановка задачи
Напишите программу, которая создаёт три потока. Каждый поток выводит своё приветствие.
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| threads | pthread_t[3] | Массив идентификаторов потоков |
| ids | int[3] | Массив идентификаторов для передачи в потоки |
| thread_function | void* (*)(void*) | Функция, выполняемая в потоке |
### Код программы
```c
#include <stdio.h>
#include <pthread.h>

void *thread_function(void *arg) {
    int id = *(int *)arg;
    printf("Привет из потока %d!\n", id);
    return NULL;
}

int main(void) {
    pthread_t threads[3];
    int ids[3] = {1, 2, 3};
    
    // Создание трёх потоков
    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, thread_function, &ids[i]);
    }
    
    // Ожидание завершения всех потоков
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Все потоки завершились.\n");
    return 0;
}
```
### Результаты работы
![Результат выполнения]
---
## Задание 2. Передача структуры в поток
### Постановка задачи
Напишите программу, где каждый поток получает структуру с двумя числами и вычисляет их сумму.
### Математическая модель
result=a+b
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| ThreadArgs | struct | Структура с двумя числами и результатом |
| sum_thread | void* (*)(void*) | Функция вычисления суммы |
| args | ThreadArgs[4] | Массив аргументов для потоков |
### Код программы
```c
#include <stdio.h>
#include <pthread.h>

typedef struct {
    int a;
    int b;
    int result;
} ThreadArgs;

void *sum_thread(void *arg) {
    ThreadArgs *data = (ThreadArgs *)arg;
    data->result = data->a + data->b;
    printf("Поток: %d + %d = %d\n", data->a, data->b, data->result);
    return NULL;
}

int main(void) {
    pthread_t threads[4];
    ThreadArgs args[4] = {
        {10, 20, 0},
        {5, 15, 0},
        {100, 200, 0},
        {7, 3, 0}
    };
    
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, sum_thread, &args[i]);
    }
    
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }
    
    return 0;
}
```
### Результаты работы
![Результат выполнения]
---
## Задание 3. Наблюдение race condition
### Постановка задачи
Создайте программу, демонстрирующую состояние race condition.
### Математическая модель
counter ожидаемое =2×100000=200000
Из-за race condition фактическое значение меньше ожидаемого.
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| counter | int | Глобальный счётчик |
| increment | void* (*)(void*) | Функция увеличения счётчика |
### Код программы
```c
#include <stdio.h>
#include <pthread.h>

int counter = 0;

void *increment(void *arg) {
    for (int i = 0; i < 100000; i++) {
        counter++;
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    
    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    printf("Ожидаемое значение: 200000\n");
    printf("Фактическое значение: %d\n", counter);
    
    return 0;
}
```
### Результаты работы
![Результат выполнения]
---
## Задание 4. Исправление race condition с mutex
### Постановка задачи
Исправьте программу из задания 3, добавив mutex для защиты общей переменной counter.
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| counter | int | Глобальный счётчик |
| mutex | pthread_mutex_t | Мьютекс для синхронизации |
| increment | void* (*)(void*) | Функция увеличения счётчика с защитой |
### Код программы
```c
#include <stdio.h>
#include <pthread.h>

int counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *increment(void *arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    
    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    printf("Ожидаемое значение: 200000\n");
    printf("Фактическое значение: %d\n", counter);
    
    pthread_mutex_destroy(&mutex);
    return 0;
}
```
### Результаты работы
![Результат выполнения]
---
## Задание 5. Параллельное суммирование массива
### Постановка задачи
Напишите программу для параллельного вычисления суммы элементов массива.
### Математическая модель
Сумма арифметической прогрессии (формула Гаусса):
S=n×(n+1)/2	 
Для n = 1 000 000:
S=1000000×1000001/2=500000500000
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| ThreadData | struct | Данные для потока (массив, границы, частичная сумма) |
| array | int* | Указатель на массив |
| partial_sum | long long | Частичная сумма диапазона |
### Код программы
```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARRAY_SIZE 1000000
#define NUM_THREADS 4

typedef struct {
    int *array;
    int start;
    int end;
    long long partial_sum;
} ThreadData;

void *sum_range(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    data->partial_sum = 0;
    
    for (int i = data->start; i < data->end; i++) {
        data->partial_sum += data->array[i];
    }
    
    return NULL;
}

int main(void) {
    // Заполнение массива
    int *array = malloc(ARRAY_SIZE * sizeof(int));
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i + 1;  // числа от 1 до 1000000
    }
    
    pthread_t threads[NUM_THREADS];
    ThreadData data[NUM_THREADS];
    int chunk_size = ARRAY_SIZE / NUM_THREADS;
    
    for (int i = 0; i < NUM_THREADS; i++) {
        data[i].array = array;
        data[i].start = i * chunk_size;
        data[i].end = (i + 1) * chunk_size;
        if (i == NUM_THREADS - 1) {
            data[i].end = ARRAY_SIZE;
        }
        pthread_create(&threads[i], NULL, sum_range, &data[i]);
    }
    
    long long total_sum = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        printf("Поток %d: сумма [%d, %d) = %lld\n", 
               i, data[i].start, data[i].end, data[i].partial_sum);
        total_sum += data[i].partial_sum;
    }
    
    long long expected_sum = (long long)ARRAY_SIZE * (ARRAY_SIZE + 1) / 2;
    
    printf("\nОбщая сумма: %lld\n", total_sum);
    printf("Проверка: %lld\n", expected_sum);
    printf("Результат: %s\n", total_sum == expected_sum ? "ВЕРНО" : "ОШИБКА");
    
    free(array);
    return 0;
}
```
### Результаты работы
![Результат выполнения]
---
## Задание 6. Счётчик с несколькими потоками
### Постановка задачи
Создайте программу со счётчиком, который увеличивается несколькими потоками.
### Математическая модель
counter ожидаемое	 =10×10000=100000
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| counter | int | Общий счётчик |
| mutex | pthread_mutex_t | Мьютекс для защиты |
| NUM_THREADS | #define | Количество потоков (10) |
| INCREMENTS_PER_THREAD | #define | Количество увеличений на поток (10000) |
### Код программы
```c
#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 10
#define INCREMENTS_PER_THREAD 10000

int counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *increment_counter(void *arg) {
    int thread_id = *(int *)arg;
    
    for (int i = 0; i < INCREMENTS_PER_THREAD; i++) {
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
    }
    
    printf("Поток %d завершил работу\n", thread_id);
    return NULL;
}

int main(void) {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];
    clock_t start_time, end_time;
    
    start_time = clock();
    
    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, increment_counter, &ids[i]);
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    end_time = clock();
    
    int expected = NUM_THREADS * INCREMENTS_PER_THREAD;
    printf("\nИтоговое значение счётчика: %d\n", counter);
    printf("Ожидаемое значение: %d\n", expected);
    printf("Результат: %s\n", counter == expected ? "ВЕРНО" : "ОШИБКА");
    printf("Время выполнения: %.2f секунд\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
    
    pthread_mutex_destroy(&mutex);
    return 0;
}
```
### Результаты работы
![Результат выполнения]
---
## Задание 7. fork()
### Постановка задачи
Написать программу, демонстрирующую создание процесса.
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| pid | pid_t | Идентификатор процесса (PID) |
| fork() | pid_t| Системный вызов для создания нового процесса |
| wait() | pid_t | Ожидание завершения дочернего процесса |
### Код программы
```c
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    printf("Родитель: PID = %d\n", getpid());
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // Дочерний процесс
        printf("Ребёнок: PID = %d, родитель = %d\n", getpid(), getppid());
        sleep(1);
        printf("Ребёнок завершается\n");
    } else if (pid > 0) {
        // Родительский процесс
        printf("Родитель: создан ребёнок с PID = %d\n", pid);
        wait(NULL);
        printf("Родитель: ребёнок завершился\n");
    } else {
        perror("fork");
        return 1;
    }
    
    return 0;
}
```
### Результаты работы
![Результат выполнения]
