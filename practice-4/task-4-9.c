#include <stdio.h>

// Функция переворота строки "на месте"
void reverse(char *str) {
    // Находим конец строки
    char *end = str;
    while (*end != '\0') {
        end++;
    }
    end--;  // указываем на последний символ
    
    // Меняем местами символы с начала и конца
    while (str < end) {
        char temp = *str;
        *str = *end;
        *end = temp;
        str++;
        end--;
    }
}

int main(void) {
    char str[] = "Hello";
    
    reverse(str);
    printf(" %s\n", str);  // olleH
    
    return 0;
}
