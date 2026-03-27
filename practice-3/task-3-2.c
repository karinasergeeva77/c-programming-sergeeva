#include <stdio.h>

int main(void) {
    int n;
    double sum = 0.0;
    
    printf("Enter n: ");
    scanf("%d", &n);
    
    for (int i = 1; i <= n; i++) {
        sum += 1.0 / i;
    }
    
    printf("S = 1 + 1/2 + 1/3 + ... + 1/%d = %.6f\n", n, sum);
    
    return 0;
}
