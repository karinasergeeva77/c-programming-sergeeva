#include <stdio.h>
#include <math.h>

int main(void) {
    // Шаг 1: Объявляем константы
    double a = 0.12, b = 3.5, c = 2.4, x = 1.4;
    
    // Шаг 2: Вычисляем ПЕРВУЮ дробь
    double num1 = x - a;  // числитель: (x - a)
    double denom1 = pow(x*x + a*a, 1.0/3.0);  // знаменатель: ∛(x² + a²)
    double term1 = num1 / denom1;  // первая дробь
    
    // Шаг 3: Вычисляем ВТОРУЮ дробь
    double num2 = pow(x*x + b*b, 3.0/4.0);  // числитель: ∜((x² + b²)³)
    double denom2 = 2 + a + b + pow(pow(x-c,2),1.0/3.0);  // знаменатель
    double term2 = num2 / denom2;  // вторая дробь
    
    // Шаг 4: Собираем результат
    double h = -term1 - term2;
    
    // Шаг 5: Выводим результат
    printf("h(%.2f) = %.6f\n", x, h);
    
    return 0;
}
