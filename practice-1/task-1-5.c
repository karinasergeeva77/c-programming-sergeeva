#include <stdio.h>

int main(void) {
    double celsius, fahrenheit;
    
    printf("Enter temperature in Celsius: ");
    scanf("%lf", &celsius);
    
    fahrenheit = (celsius * 9 / 5) + 32;
    
    printf("Temperature in Fahrenheit: %.2lf\n", fahrenheit);
    
    return 0;
}
