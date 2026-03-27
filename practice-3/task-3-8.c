#include <stdio.h>
#include <stdbool.h> // для типа bool
int main(void) {
int n;
printf("Enter N: ");
scanf("%d"
, &n);
// Массив: is_prime[i] = true, если i простое
bool is_prime[n + 1];
// Шаг 1: Считаем все числа простыми
for (int i = 0; i <= n; i++) {
is_prime[i] = true;
}
is_prime[0] = is_prime[1] = false; // 0 и 1 не простые
// Шаг 2: Вычёркиваем составные
for (int i = 2; i * i <= n; i++) { // i до √n
if (is_prime[i]) {
    // Вычёркиваем все кратные i
for (int j = i * i; j <= n; j += i) {
is_prime[j] = false;
}
}
}
// Шаг 3: Выводим оставшиеся
printf("Primes: ");
for (int i = 2; i <= n; i++) {
if (is_prime[i]) {
printf("%d "
, i);
}
}
printf("\n");
return 0;
}
