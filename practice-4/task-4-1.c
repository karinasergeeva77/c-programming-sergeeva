#include <stdio.h>

// Функция вычисления факториала (итеративно)
long long factorial(int n) {
    if (n < 0) return 0;
    
    long long result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main(void) {
    int n;
    
    printf("Enter number: ");
    scanf("%d", &n);
    
    printf("%d! = %lld\n", n, factorial(n));
    
    return 0;
}
