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
