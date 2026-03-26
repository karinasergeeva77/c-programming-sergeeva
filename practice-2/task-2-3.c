#include <stdio.h>

int main(void) {
    int a, b, c, max;
    
    printf("Enter three numbers: ");
    scanf("%d %d %d", &a, &b, &c);
    
    // Находим максимальное
    max = a;
    if (b > max) {
        max = b;
    }
    if (c > max) {
        max = c;
    }
    
    printf("Maximum: %d\n", max);
    
    return 0;
}
