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
