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
