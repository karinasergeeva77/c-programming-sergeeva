#include <stdio.h>

int main(void) {
    int number;
    
    printf("Enter number: ");
    scanf("%d", &number);
    
    // Проверяем остаток от деления на 2
    if (number % 2 == 0) {
        printf("%d is even\n", number);
    } else {
        printf("%d is odd\n", number);
    }
    
    return 0;
}
