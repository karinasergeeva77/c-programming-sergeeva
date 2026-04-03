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
