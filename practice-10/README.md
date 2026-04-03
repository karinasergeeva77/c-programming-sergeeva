# Практическая работа 10: Создание библиотек
**Студент:** Сергеева Карина Алексеевна
**Группа:** 1зб_ИВТ(ускор.)/25
**Дата:** 01 апреля 2026 г.
---
## Задание 10.1: Библиотека математических функций 
### Постановка задачи
Создать библиотеку mathutils с набором математических функций.
### Математическая модель
Факториал:
n!=1×2×3×...×n,0!=1
Числа Фибоначчи:
F(0)=0, F(1)=1, F(n)=F(n−1)+F(n−2)
НОД (алгоритм Евклида):
gcd(a,b)=gcd(b,amodb)
НОК:
lcm(a,b)= ∣a⋅b∣ / gcd(a,b)
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| factorial | int (*)(int) | Вычисление факториала |
| fibonacci | int (*)(int) | Вычисление n-го числа Фибоначчи |
| gcd | int (*)(int, int) | Наибольший общий делитель |
| lcm | int (*)(int, int) | Наименьшее общее кратное |
| is_prime | int (*)(int) | Проверка числа на простоту |
### Код программы
mathutils.h
```c
#ifndef MATHUTILS_H
#define MATHUTILS_H

#ifdef _WIN32
    #ifdef MATHUTILS_EXPORTS
        #define MATHUTILS_API __declspec(dllexport)
    #else
        #define MATHUTILS_API __declspec(dllimport)
    #endif
#else
    #define MATHUTILS_API
#endif

MATHUTILS_API int factorial(int n);
MATHUTILS_API int fibonacci(int n);
MATHUTILS_API int gcd(int a, int b);
MATHUTILS_API int lcm(int a, int b);
MATHUTILS_API int is_prime(int n);

#endif
```
mathutils.c
```c
#include "mathutils.h"

int factorial(int n) {
    if (n < 0) return 0;
    if (n <= 1) return 1;
    
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

int fibonacci(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    
    int a = 0, b = 1, c;
    for (int i = 2; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

int gcd(int a, int b) {
    a = (a < 0) ? -a : a;
    b = (b < 0) ? -b : b;
    
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int lcm(int a, int b) {
    if (a == 0 || b == 0) return 0;
    return a / gcd(a, b) * b;
}

int is_prime(int n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
    }
    return 1;
}
```
main.c 
```c
#include <stdio.h>
#include "mathutils.h"

int main(void) {
    printf("=== Тестирование mathutils ===\n\n");
    
    printf("factorial(5) = %d\n", factorial(5));
    printf("factorial(10) = %d\n", factorial(10));
    printf("factorial(0) = %d\n", factorial(0));
    
    printf("\nfibonacci(10) = %d\n", fibonacci(10));
    printf("fibonacci(1) = %d\n", fibonacci(1));
    printf("fibonacci(20) = %d\n", fibonacci(20));
    
    printf("\ngcd(48, 18) = %d\n", gcd(48, 18));
    printf("gcd(17, 19) = %d\n", gcd(17, 19));
    printf("gcd(100, 25) = %d\n", gcd(100, 25));
    
    printf("\nlcm(4, 6) = %d\n", lcm(4, 6));
    printf("lcm(12, 18) = %d\n", lcm(12, 18));
    
    printf("\nis_prime(17) = %d\n", is_prime(17));
    printf("is_prime(18) = %d\n", is_prime(18));
    printf("is_prime(97) = %d\n", is_prime(97));
    
    return 0;
}
```
Makefile
```c
CC = gcc
CFLAGS = -Wall -Wextra -fPIC
LDFLAGS = -lm

# Определение ОС
ifeq ($(OS),Windows_NT)
    STATIC_LIB = libmathutils.a
    SHARED_LIB = mathutils.dll
    IMPORT_LIB = libmathutils.dll.a
    EXE_STATIC = program_static.exe
    EXE_SHARED = program_shared.exe
    SHARED_FLAGS = -shared -Wl,--out-implib,$(IMPORT_LIB)
    EXPORT_DEF = -DMATHUTILS_EXPORTS
    RM = del /Q
else
    STATIC_LIB = libmathutils.a
    SHARED_LIB = libmathutils.so
    IMPORT_LIB = 
    EXE_STATIC = program_static
    EXE_SHARED = program_shared
    SHARED_FLAGS = -shared
    EXPORT_DEF = 
    RM = rm -f
endif

.PHONY: all static shared clean

all: static shared

# Статическая библиотека
static: $(STATIC_LIB) $(EXE_STATIC)

mathutils_static.o: mathutils.c mathutils.h
	$(CC) $(CFLAGS) -c mathutils.c -o mathutils_static.o

$(STATIC_LIB): mathutils_static.o
	ar rcs $@ $^

$(EXE_STATIC): main.c $(STATIC_LIB)
	$(CC) $(CFLAGS) main.c -L. -lmathutils $(LDFLAGS) -o $@

# Динамическая библиотека
shared: $(SHARED_LIB) $(EXE_SHARED)

mathutils_shared.o: mathutils.c mathutils.h
	$(CC) $(CFLAGS) $(EXPORT_DEF) -c mathutils.c -o mathutils_shared.o

$(SHARED_LIB): mathutils_shared.o
	$(CC) $(SHARED_FLAGS) $^ -o $@

$(EXE_SHARED): main.c $(SHARED_LIB)
	$(CC) $(CFLAGS) main.c -L. -lmathutils $(LDFLAGS) -o $@

clean:
	$(RM) *.o *.a *.so *.dll $(EXE_STATIC) $(EXE_SHARED) 2>/dev/null || true
```

### Результаты работы
![Результат выполнения](practice-10/screenshots/task-10-1.png)
---
## Задание 10.2: Библиотека для работы со строками
### Постановка задачи
Создать библиотеку strutils с функциями обработки строк.
### Математическая модель
Реверс строки: $S'[i] = S[n-1-i]$ для $i = 0..n-1$
Подсчёт слов: количество переходов из пробела в непробельный символ
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| str_reverse | char* (*)(const char*) | Переворот строки |
| str_to_upper | char* (*)(const char*) | В верхний регистр |
| str_to_lower | char* (*)(const char*) | В нижний регистр |
| str_trim | char* (*)(const char*) | Удаление пробелов по краям |
| str_word_count | int (*)(const char*) | Подсчёт слов |
### Код программы
strutils.h
```c
#ifndef STRUTILS_H
#define STRUTILS_H

#include <stddef.h>

#ifdef _WIN32
    #ifdef STRUTILS_EXPORTS
        #define STRUTILS_API __declspec(dllexport)
    #else
        #define STRUTILS_API __declspec(dllimport)
    #endif
#else
    #define STRUTILS_API
#endif

STRUTILS_API char *str_reverse(const char *s);
STRUTILS_API char *str_to_upper(const char *s);
STRUTILS_API char *str_to_lower(const char *s);
STRUTILS_API char *str_trim(const char *s);
STRUTILS_API int str_word_count(const char *s);

#endif
```
strutils.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "strutils.h"

char *str_reverse(const char *s) {
    if (s == NULL) return NULL;
    
    size_t len = strlen(s);
    char *result = malloc(len + 1);
    if (result == NULL) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = s[len - 1 - i];
    }
    result[len] = '\0';
    return result;
}

char *str_to_upper(const char *s) {
    if (s == NULL) return NULL;
    
    size_t len = strlen(s);
    char *result = malloc(len + 1);
    if (result == NULL) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = toupper((unsigned char)s[i]);
    }
    result[len] = '\0';
    return result;
}

char *str_to_lower(const char *s) {
    if (s == NULL) return NULL;
    
    size_t len = strlen(s);
    char *result = malloc(len + 1);
    if (result == NULL) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower((unsigned char)s[i]);
    }
    result[len] = '\0';
    return result;
}

char *str_trim(const char *s) {
    if (s == NULL) return NULL;
    
    size_t len = strlen(s);
    size_t start = 0;
    size_t end = len;
    
    while (start < len && isspace((unsigned char)s[start])) start++;
    while (end > start && isspace((unsigned char)s[end - 1])) end--;
    
    size_t new_len = end - start;
    char *result = malloc(new_len + 1);
    if (result == NULL) return NULL;
    
    memcpy(result, s + start, new_len);
    result[new_len] = '\0';
    return result;
}

int str_word_count(const char *s) {
    if (s == NULL) return 0;
    
    int count = 0;
    int in_word = 0;
    
    while (*s) {
        if (isspace((unsigned char)*s)) {
            in_word = 0;
        } else {
            if (!in_word) {
                count++;
                in_word = 1;
            }
        }
        s++;
    }
    return count;
}
```
main.c 
```c
#include <stdio.h>
#include <stdlib.h>
#include "strutils.h"

int main(void) {
    const char *test = "   Hello   World   ";
    
    printf("=== Тестирование strutils ===\n\n");
    printf("Original: \"%s\"\n", test);
    
    char *reversed = str_reverse(test);
    printf("Reversed: \"%s\"\n", reversed);
    free(reversed);
    
    char *upper = str_to_upper(test);
    printf("Upper:    \"%s\"\n", upper);
    free(upper);
    
    char *lower = str_to_lower(test);
    printf("Lower:    \"%s\"\n", lower);
    free(lower);
    
    char *trimmed = str_trim(test);
    printf("Trimmed:  \"%s\"\n", trimmed);
    free(trimmed);
    
    int words = str_word_count(test);
    printf("Words:    %d\n", words);
    
    return 0;
}
```
Makefile
```c
CC = gcc
CFLAGS = -Wall -Wextra -fPIC

# Определение ОС
ifeq ($(OS),Windows_NT)
    STATIC_LIB = libstrutils.a
    SHARED_LIB = strutils.dll
    IMPORT_LIB = libstrutils.dll.a
    EXE_STATIC = program_static.exe
    EXE_SHARED = program_shared.exe
    SHARED_FLAGS = -shared -Wl,--out-implib,$(IMPORT_LIB)
    EXPORT_DEF = -DSTRUTILS_EXPORTS
    RM = del /Q
else
    STATIC_LIB = libstrutils.a
    SHARED_LIB = libstrutils.so
    IMPORT_LIB = 
    EXE_STATIC = program_static
    EXE_SHARED = program_shared
    SHARED_FLAGS = -shared
    EXPORT_DEF = 
    RM = rm -f
endif

.PHONY: all static shared clean

all: static shared

# Статическая библиотека
static: $(STATIC_LIB) $(EXE_STATIC)

strutils_static.o: strutils.c strutils.h
	$(CC) $(CFLAGS) -c strutils.c -o strutils_static.o

$(STATIC_LIB): strutils_static.o
	ar rcs $@ $^

$(EXE_STATIC): main.c $(STATIC_LIB)
	$(CC) $(CFLAGS) main.c -L. -lstrutils -o $@

# Динамическая библиотека
shared: $(SHARED_LIB) $(EXE_SHARED)

strutils_shared.o: strutils.c strutils.h
	$(CC) $(CFLAGS) $(EXPORT_DEF) -c strutils.c -o strutils_shared.o

$(SHARED_LIB): strutils_shared.o
	$(CC) $(SHARED_FLAGS) $^ -o $@

$(EXE_SHARED): main.c $(SHARED_LIB)
	$(CC) $(CFLAGS) main.c -L. -lstrutils -o $@

clean:
	$(RM) *.o *.a *.so *.dll $(EXE_STATIC) $(EXE_SHARED) 2>/dev/null || true

show-static: $(STATIC_LIB)
	ar -t $(STATIC_LIB)

size:
	ls -lh $(EXE_STATIC) $(EXE_SHARED) 2>/dev/null

```

### Результаты работы
![Результат выполнения](practice-10/screenshots/task-10-2.png)
---
## Задание 10.3: Makefile для кроссплатформенной сборки
### Постановка задачи
Написать универстальный Makefile
### Код программы
Makefile
```c
CC = gcc
CFLAGS = -Wall -Wextra -fPIC
LDFLAGS = -lm

# Определение ОС
ifeq ($(OS),Windows_NT)
    STATIC_LIB = libmathutils.a
    SHARED_LIB = mathutils.dll
    IMPORT_LIB = libmathutils.dll.a
    EXE_STATIC = program_static.exe
    EXE_SHARED = program_shared.exe
    SHARED_FLAGS = -shared -Wl,--out-implib,$(IMPORT_LIB)
    EXPORT_DEF = -DMATHUTILS_EXPORTS
    RM = del /Q
    SEP = \\
else
    STATIC_LIB = libmathutils.a
    SHARED_LIB = libmathutils.so
    IMPORT_LIB = 
    EXE_STATIC = program_static
    EXE_SHARED = program_shared
    SHARED_FLAGS = -shared
    EXPORT_DEF = 
    RM = rm -f
    SEP = /
endif

.PHONY: all static shared clean info size

all: static shared

# Статическая библиотека
static: $(STATIC_LIB) $(EXE_STATIC)

mathutils_static.o: mathutils.c mathutils.h
	$(CC) $(CFLAGS) -c mathutils.c -o mathutils_static.o

$(STATIC_LIB): mathutils_static.o
	ar rcs $@ $^

$(EXE_STATIC): main.c $(STATIC_LIB)
	$(CC) $(CFLAGS) main.c -L. -lmathutils $(LDFLAGS) -o $@

# Динамическая библиотека
shared: $(SHARED_LIB) $(EXE_SHARED)

mathutils_shared.o: mathutils.c mathutils.h
	$(CC) $(CFLAGS) $(EXPORT_DEF) -c mathutils.c -o mathutils_shared.o

$(SHARED_LIB): mathutils_shared.o
	$(CC) $(SHARED_FLAGS) $^ -o $@

$(EXE_SHARED): main.c $(SHARED_LIB)
	$(CC) $(CFLAGS) main.c -L. -lmathutils $(LDFLAGS) -o $@

info:
	@echo "OS: $(OS)"
	@echo "Static: $(STATIC_LIB)"
	@echo "Shared: $(SHARED_LIB)"

size:
	ls -lh $(EXE_STATIC) $(EXE_SHARED) 2>/dev/null

clean:
	$(RM) *.o *.a *.so *.dll $(EXE_STATIC) $(EXE_SHARED) 2>/dev/null || true
```
### Результаты работы
![Результат выполнения](practice-10/screenshots/task-10-3.png)
---
## Задание 10.4: Версионирование библиотеки
### Постановка задачи
Научиться создавать версионированные shared libraries.
### Математическая модель
Версионирование: libcalc.so.1.0.0 → libcalc.so.1 → libcalc.so
install_name: идентификатор библиотеки, записываемый внутрь .so файла
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| add | int (*)(int, int) | Сложение |
| sub | int (*)(int, int) | Вычитание |
| mul | int (*)(int, int) | Умножение |
| div | int (*)(int, int) | Деление |
### Код программы
calc.h
```c
#ifndef CALC_H
#define CALC_H

int add(int a, int b);
int sub(int a, int b);
int mul(int a, int b);
int div(int a, int b);

#endif
```
calc.c
```c
#include "calc.h"

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int div(int a, int b) { return b != 0 ? a / b : 0; }
```
main.c 
```c
#include <stdio.h>
#include "calc.h"

int main(void) {
    printf("=== Тестирование библиотеки calc ===\n\n");
    printf("10 + 5 = %d\n", add(10, 5));
    printf("10 - 5 = %d\n", sub(10, 5));
    printf("10 * 5 = %d\n", mul(10, 5));
    printf("10 / 5 = %d\n", div(10, 5));
    return 0;
}
```
Makefile
```c
CC = gcc
CFLAGS = -Wall -Wextra -fPIC
VERSION = 1.0.0
SONAME = libcalc.so.1
REAL_NAME = libcalc.so.$(VERSION)

.PHONY: all clean test run info

all: $(REAL_NAME) $(SONAME) libcalc.so test

calc.o: calc.c calc.h
	$(CC) $(CFLAGS) -c calc.c -o calc.o

# Создание версионированной библиотеки
$(REAL_NAME): calc.o
	$(CC) -shared -Wl,-install_name,$(SONAME) calc.o -o $(REAL_NAME)

# Символическая ссылка для soname (нужна для запуска)
$(SONAME): $(REAL_NAME)
	ln -sf $(REAL_NAME) $(SONAME)

# Символическая ссылка для компиляции
libcalc.so: $(REAL_NAME)
	ln -sf $(REAL_NAME) libcalc.so

test: main.c libcalc.so
	$(CC) main.c -L. -lcalc -o test_program

run: test $(SONAME)
	./test_program

info: $(REAL_NAME)
	@echo "=== Структура библиотеки ==="
	ls -la libcalc*
	@echo ""
	@echo "=== install_name из библиотеки ==="
	otool -D $(REAL_NAME)
	@echo ""
	@echo "=== Зависимости test_program ==="
	otool -L test_program

clean:
	rm -f *.o test_program

distclean: clean
	rm -f libcalc.so* test_program

```

### Результаты работы
[Результат выполнения](practice-10/screenshots/task-10-4.png)
