#include <stdio.h>
#include <math.h>

// Подынтегральная функция
double f(double x) {
    return sin(x);  // sin(x)
}

int main(void) {
    double a = 0, b = M_PI;  // Границы интегрирования: от 0 до π
    int n = 1000;            // Количество разбиений
    double h = (b - a) / n;  // Шаг
    
    // Сумма по методу трапеций
    double sum = (f(a) + f(b)) / 2.0;  // Первое и последнее значения с коэф. 1/2
    
    for (int i = 1; i < n; i++) {
        sum += f(a + i * h);  // Промежуточные значения
    }
    
    double integral = h * sum;
    
    printf("Integral of sin(x) from %.1f to %.1f:\n", a, b);
    printf("Numerical result: %.6f\n", integral);
    
    // Аналитический ответ: ∫ sin(x) dx от 0 до π = 2
    double exact = 2.0;  // cos(0) - cos(π) = 1 - (-1) = 2
    printf("Exact result: %.6f\n", exact);
    printf("Error: %.6f\n", fabs(integral - exact));
    
    return 0;
}
