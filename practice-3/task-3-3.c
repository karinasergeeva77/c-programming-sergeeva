#include <stdio.h>

int main(void) {
    int n;
    
    printf("Enter N: ");
    scanf("%d", &n);
    
    if (n <= 0) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Fibonacci: ");
    
    if (n >= 1) {
        printf("0 ");
    }
    if (n >= 2) {
        printf("1 ");
    }
    
    long long prev2 = 0, prev1 = 1, current;
    
    for (int i = 3; i <= n; i++) {
        current = prev1 + prev2;
        printf("%lld ", current);
        prev2 = prev1;
        prev1 = current;
    }
    
    printf("\n");
    
    return 0;
}
