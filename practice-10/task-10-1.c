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
