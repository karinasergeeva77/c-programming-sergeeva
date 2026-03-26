#include <stdio.h>

int main(void) {
    double a, b, c;
    
    printf("Enter sides a, b, c: ");
    scanf("%lf %lf %lf", &a, &b, &c);
    
    // Проверка существования треугольника
    if (a + b <= c || a + c <= b || b + c <= a) {
        printf("Triangle does not exist\n");
    }
    // Проверка равностороннего
    else if (a == b && b == c) {
        printf("Equilateral triangle\n");
    }
    // Проверка равнобедренного
    else if (a == b || a == c || b == c) {
        printf("Isosceles triangle\n");
    }
    // Разносторонний
    else {
        printf("Scalene triangle\n");
    }
    
    return 0;
}
