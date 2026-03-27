#include <stdio.h>

// Функция вычисления длины строки (без strlen)
int my_strlen(char *str) {
    int length = 0;
    while (*str != '\0') {  // пока не встретили конец строки
        length++;
        str++;  // переходим к следующему символу
    }
    return length;
}

int main(void) {
    char str1[] = "Hello";
    char str2[] = "";
    
    printf("length \"%s\": %d\n", str1, my_strlen(str1));  // 5
    printf("length \"%s\": %d\n", str2, my_strlen(str2));  // 0
    
    return 0;
}
