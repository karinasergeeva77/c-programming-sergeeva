#include <stdio.h>
#include <math.h>
// Подынтегральная функция
double f(double x) {
return exp(x + 2); // e^(x+2)
}
int main(void) {
double a = 0, b = 1; // Границы интегрирования
int n = 1000; // Количество разбиений
double h = (b - a) / n; // Шаг
// Сумма по методу трапеций
double sum = (f(a) + f(b)) / 2.0; // Первое и последнее значения с коэф. 1/2
for (int i = 1; i < n; i++) {
sum += f(a + i * h); // Промежуточные значения
}double integral = h * sum;
printf("Integral of e^(x+2) from %.1f to %.1f:\n", a, b);
printf("Numerical result: %.6f\n", integral);
// Аналитический ответ: e^(x+2) от a до b = e^(b+2) - e^(a+2)
double exact = exp(b + 2) - exp(a + 2);
printf("Exact result: %.6f\n"
, exact);
printf("Error: %.6f\n"
, fabs(integral - exact));
return 0;
}
