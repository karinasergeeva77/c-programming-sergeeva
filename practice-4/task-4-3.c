#include <stdio.h>

// Функция возведения в целую степень (включая отрицательную)
double power(double base, int exp) {
    if (exp == 0) return 1.0;
    
    double result = 1.0;
    int abs_exp = exp > 0 ? exp : -exp;
    
    for (int i = 0; i < abs_exp; i++) {
        result *= base;
    }
    
    // Если степень отрицательная, возвращаем 1/result
    return exp > 0 ? result : 1.0 / result;
}

int main(void) {
    printf("2^10 = %.0lf\n", power(2, 10));    // 1024
    printf("2^(-3) = %.3lf\n", power(2, -3));  // 0.125
    printf("5^0 = %.0lf\n", power(5, 0));      // 1
    
    return 0;
}
