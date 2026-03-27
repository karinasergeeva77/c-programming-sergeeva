#include <stdio.h>
#include <math.h>

int main(void) {
    double pi = 3.14159;
    double step = 0.1;
    
    printf("Table of sin(x) from 0 to 2π:\n");
    printf("=============================\n");
    
    for (double x = 0; x <= 2 * pi + step/2; x += step) {
        printf("x = %.2f, sin(x) = %.4f\n", x, sin(x));
    }
    
    return 0;
}
