#include <stdio.h>

int main(void) {
    int M;
    
    printf("Enter M (number of Padovan numbers): ");
    scanf("%d", &M);
    
    if (M <= 0) {
        printf("Please enter a positive integer\n");
        return 1;
    }
    
    printf("Padovan numbers (first %d): ", M);
    
    // Первые три числа: 1, 1, 1
    if (M >= 1) {
        printf("1 ");
    }
    if (M >= 2) {
        printf("1 ");
    }
    if (M >= 3) {
        printf("1 ");
    }
    
    long long p0 = 1;  // P(n-3)
    long long p1 = 1;  // P(n-2)
    long long p2 = 1;  // P(n-1)
    long long p3;      // P(n)
    
    // Начинаем с 4-го числа (индекс 3)
    for (int i = 4; i <= M; i++) {
        p3 = p0 + p1;           // P(n) = P(n-2) + P(n-3)
        printf("%lld ", p3);
        p0 = p1;                // Сдвиг для следующей итерации
        p1 = p2;
        p2 = p3;
    }
    
    printf("\n");
    
    return 0;
}
