#include <stdio.h>

int main(void) {
    int a, b, product;
    
    printf("Enter first number: ");
    scanf("%d", &a);
    
    printf("Enter second number: ");
    scanf("%d", &b);
    
    product = a * b;
    
    printf("Product: %d * %d = %d\n", a, b, product);
    
    return 0;
}
