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
