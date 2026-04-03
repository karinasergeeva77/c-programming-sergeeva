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
