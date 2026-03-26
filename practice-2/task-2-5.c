#include <stdio.h>
#include <math.h>

int main(void) {
    double a, b, c, D, x1, x2;
    
    printf("Enter a, b, c: ");
    scanf("%lf %lf %lf", &a, &b, &c);
    
    // Проверка, что это квадратное уравнение
    if (a == 0) {
        printf("This is not a quadratic equation (a = 0)\n");
        return 1;
    }
    
    // Вычисляем дискриминант
    D = b*b - 4*a*c;
    
    printf("D = %.2f\n", D);
    
    if (D > 0) {
        // Два корня
        x1 = (-b + sqrt(D)) / (2*a);
        x2 = (-b - sqrt(D)) / (2*a);
        printf("x1 = %.2f\n", x1);
        printf("x2 = %.2f\n", x2);
    }
    else if (D == 0) {
        // Один корень
        x1 = -b / (2*a);
        printf("x = %.2f\n", x1);
    }
    else {
        // Нет корней
        printf("No real roots\n");
    }
    
    return 0;
}
