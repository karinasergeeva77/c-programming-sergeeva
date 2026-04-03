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
