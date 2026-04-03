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
