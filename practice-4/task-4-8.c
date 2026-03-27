#include <stdio.h>

// Функция копирования строки
void my_strcpy(char *dest, char *src) {
    while (*src != '\0') {
        *dest = *src;  // копируем символ
        dest++;
        src++;
    }
    *dest = '\0';  // добавляем завершающий нуль
}

int main(void) {
    char source[] = "Hello, World!";
    char destination[50];
    
    my_strcpy(destination, source);
    
    printf(" %s\n", destination);
    
    return 0;
}
