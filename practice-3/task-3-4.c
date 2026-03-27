#include <stdio.h>

int main(void) {
    int arr[5];
    int sum = 0;
    
    printf("Enter 5 numbers: ");
    
    for (int i = 0; i < 5; i++) {
        scanf("%d", &arr[i]);
        sum += arr[i];
    }
    
    double average = (double)sum / 5;
    
    printf("Average: %.2f\n", average);
    
    return 0;
}
