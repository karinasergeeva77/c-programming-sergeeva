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
