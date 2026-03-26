#include <stdio.h>

int main(void) {
    double radius, area;
    const double PI = 3.14159;
    
    printf("Enter radius: ");
    scanf("%lf", &radius);
    
    area = PI * radius * radius;
    
    printf("Area: %.2lf\n", area);
    
    return 0;
}
