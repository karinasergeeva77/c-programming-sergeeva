#include <stdio.h>

// Рекурсивная функция вычисления факториала
long long factorial_rec(int n) {
    // База рекурсии
    if (n <= 1) {
        return 1;
    }
    // Рекурсивный вызов
    return n * factorial_rec(n - 1);
}

int main(void) {
    int n;
    
    printf("Enter number: ");
    scanf("%d", &n);
    
    printf("%d! = %lld\n", n, factorial_rec(n));
    
    return 0;
}
