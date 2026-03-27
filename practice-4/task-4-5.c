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
