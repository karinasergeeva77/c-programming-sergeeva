# Практическая работа 2: Типы данных, арифметика, условные операторы
**Студент:** Сергеева Карина Алексеевна
**Группа:** 1зб_ИВТ(ускор.)/25
**Дата:** 26 марта 2026 г.
---
## Задание 2.1: Вычисление формулы
### Постановка задачи
Напишите программу, которая вычислит значение выражения.
для значений: a = 0.12, b = 3.5, c = 2.4, x = 1.4
### Математическая модель
Выражение состоит из двух дробей, которые вычитаются:
Первая дробь: $\frac{x-a}{\sqrt[3]{x^2+a^2}}$
Вторая дробь: $\frac{\sqrt[4]{(x^2+b^2)^3}}{2+a+b+\sqrt[3]{(x-c)^2}}$
Результат: $h = -\text{первая дробь} - \text{вторая дробь}$
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| a | double | Константа a = 0.12 |
| b | double | Константа b = 3.5 |
| c | double | Константа c = 2.4 |
| x | double | Переменная x = 1.4 |
| num1 | double | Числитель первой дроби |
| denom1 | double | Знаменатель первой дроби |
| term1 | double | Значение первой дроби|
| num2 | double | Числитель второй дроби |
| denom2 | double | Знаменатель второй дроби |
| term2 | double | Значение второй дроби |
| h | double | Результат вычисления |
### Код программы
```c
#include <stdio.h>
#include <math.h>

int main(void) {
    // Шаг 1: Объявляем константы
    double a = 0.12, b = 3.5, c = 2.4, x = 1.4;
    
    // Шаг 2: Вычисляем ПЕРВУЮ дробь
    double num1 = x - a;  // числитель: (x - a)
    double denom1 = pow(x*x + a*a, 1.0/3.0);  // знаменатель: ∛(x² + a²)
    double term1 = num1 / denom1;  // первая дробь
    
    // Шаг 3: Вычисляем ВТОРУЮ дробь
    double num2 = pow(x*x + b*b, 3.0/4.0);  // числитель: ∜((x² + b²)³)
    double denom2 = 2 + a + b + pow(pow(x-c,2),1.0/3.0);  // знаменатель
    double term2 = num2 / denom2;  // вторая дробь
    
    // Шаг 4: Собираем результат
    double h = -term1 - term2;
    
    // Шаг 5: Выводим результат
    printf("h(%.2f) = %.6f\n", x, h);
    
    return 0;
}
```
### Результаты работы
![Результат вычисления]([practice-2/screenshots/task-2-1.png](https://github.com/karinasergeeva77/c-programming-sergeeva/blob/61183702ba7387d8b3701b425b56db94fbe2c8df/practice-2/screenshots/task-2-1.png))
---
## Задание 2.2: Чётное или нечётное
### Постановка задачи
Напишите программу, которая определяет, является ли введённое число чётным или нечётным.
### Математическая модель
Число является чётным, если остаток от деления на 2 равен 0:
nmod2=0
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| number | int | Введённое число |
### Код программы
```c
#include <stdio.h>

int main(void) {
    int number;
    
    printf("Enter number: ");
    scanf("%d", &number);
    
    // Проверяем остаток от деления на 2
    if (number % 2 == 0) {
        printf("%d is even\n", number);
    } else {
        printf("%d is odd\n", number);
    }
    
    return 0;
}
```
### Результаты работы
![Результат]([practice-2/screenshots/task-2-2-1.png](https://github.com/karinasergeeva77/c-programming-sergeeva/blob/61183702ba7387d8b3701b425b56db94fbe2c8df/practice-2/screenshots/task-2-2-1.png))(https://github.com/karinasergeeva77/c-programming-sergeeva/blob/61183702ba7387d8b3701b425b56db94fbe2c8df/practice-2/screenshots/task-2-2-2.png)
---
## Задание 2.3: Максимум из трёх
### Постановка задачи
Напишите программу, которая находит максимальное из трёх введённых чисел.
### Математическая модель
Максимум из трёх чисел:
max(a,b,c)= 
a,если a≥b и a≥c
b,если b≥a и b≥c
c,в остальных случаях
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| a | int | Первое число |
| b | int | Второе число |
| c | int | Третье число |
| max | int | Максимальное значение |
### Код программы
```c
#include <stdio.h>

int main(void) {
    int a, b, c, max;
    
    printf("Enter three numbers: ");
    scanf("%d %d %d", &a, &b, &c);
    
    // Находим максимальное
    max = a;
    if (b > max) {
        max = b;
    }
    if (c > max) {
        max = c;
    }
    
    printf("Maximum: %d\n", max);
    
    return 0;
}
```
### Результаты работы
![Результат](https://github.com/karinasergeeva77/c-programming-sergeeva/blob/61183702ba7387d8b3701b425b56db94fbe2c8df/practice-2/screenshots/task-2-3.png)
---
## Задание 2.4: Тип треугольника
### Постановка задачи
Напишите программу, которая по трём сторонам определяет тип треугольника:
Равносторонний — все стороны равны
Равнобедренный — две стороны равны
Разносторонний — все стороны разные
Не существует — если сумма двух сторон меньше или равна третьей
### Математическая модель
Условие существования треугольника:
a+b>c и a+c>b и b+c>a
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| a | double | Первая сторона |
| b | double | Вторая сторона |
| c | double | Третья сторона |
### Код программы
```c
#include <stdio.h>

int main(void) {
    double a, b, c;
    
    printf("Enter sides a, b, c: ");
    scanf("%lf %lf %lf", &a, &b, &c);
    
    // Проверка существования треугольника
    if (a + b <= c || a + c <= b || b + c <= a) {
        printf("Triangle does not exist\n");
    }
    // Проверка равностороннего
    else if (a == b && b == c) {
        printf("Equilateral triangle\n");
    }
    // Проверка равнобедренного
    else if (a == b || a == c || b == c) {
        printf("Isosceles triangle\n");
    }
    // Разносторонний
    else {
        printf("Scalene triangle\n");
    }
    
    return 0;
}
```
### Результаты работы
![Результат](https://github.com/karinasergeeva77/c-programming-sergeeva/blob/61183702ba7387d8b3701b425b56db94fbe2c8df/practice-2/screenshots/task-2-4-1.png)(https://github.com/karinasergeeva77/c-programming-sergeeva/blob/61183702ba7387d8b3701b425b56db94fbe2c8df/practice-2/screenshots/task-2-4-4.png)
---
## Задание 2.5: Квадратное уравнение
### Постановка задачи
Напишите программу для решения квадратного уравнения $ax^2 + bx + c = 0$:
Вычислить дискриминант: $D = b^2 - 4ac$
Если $D > 0$: два корня $x_{1,2} = \frac{-b \pm \sqrt{D}}{2a}$
Если $D = 0$: один корень $x = \frac{-b}{2a}$
Если $D < 0$: корней нет
### Математическая модель
D=b² −4ac
x1,2=-b+-sqrt(D), D>0
x=-b/2a, D=0
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| a | double | Коэффициент при x² |
| b | double | Коэффициент при x |
| c | double | Свободный член |
| D | double | Дискриминант |
| x1 | double | Первый корень |
| x2 | double | Второй корень |
### Код программы
```c
#include <stdio.h>
#include <math.h>

int main(void) {
    double a, b, c, D, x1, x2;
    
    printf("Enter a, b, c: ");
    scanf("%lf %lf %lf", &a, &b, &c);
    
    // Проверка, что это квадратное уравнение
    if (a == 0) {
        printf("This is not a quadratic equation (a = 0)\n");
        return 1;
    }
    
    // Вычисляем дискриминант
    D = b*b - 4*a*c;
    
    printf("D = %.2f\n", D);
    
    if (D > 0) {
        // Два корня
        x1 = (-b + sqrt(D)) / (2*a);
        x2 = (-b - sqrt(D)) / (2*a);
        printf("x1 = %.2f\n", x1);
        printf("x2 = %.2f\n", x2);
    }
    else if (D == 0) {
        // Один корень
        x1 = -b / (2*a);
        printf("x = %.2f\n", x1);
    }
    else {
        // Нет корней
        printf("No real roots\n");
    }
    
    return 0;
}
```
### Результаты работы
![Результат] (https://github.com/karinasergeeva77/c-programming-sergeeva/blob/61183702ba7387d8b3701b425b56db94fbe2c8df/practice-2/screenshots/task-2-5-1.png)
---
## Задание 2.6: Калькулятор
### Постановка задачи
Напишите программу-калькулятор, которая:
Запрашивает два числа
Запрашивает операцию (+, -, *, /)
Выводит результат
Обрабатывает деление на ноль
### Математическая модель
a+b,a−b,a×b,a÷b (при b!=0)
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| num1 | double | Первое число |
| num2 | double | Второе число |
| result | double | Результат операции |
| operation | char | Знак операции (+, -, *, /) |
### Код программы
```c
#include <stdio.h>

int main(void) {
    double num1, num2, result;
    char operation;
    
    printf("Enter first number: ");
    scanf("%lf", &num1);
    
    printf("Enter second number: ");
    scanf("%lf", &num2);
    
    printf("Enter operation (+, -, *, /): ");
    scanf(" %c", &operation);  // пробел перед %c для очистки буфера
    
    switch (operation) {
        case '+':
            result = num1 + num2;
            printf("Result: %.2f + %.2f = %.2f\n", num1, num2, result);
            break;
        case '-':
            result = num1 - num2;
            printf("Result: %.2f - %.2f = %.2f\n", num1, num2, result);
            break;
        case '*':
            result = num1 * num2;
            printf("Result: %.2f * %.2f = %.2f\n", num1, num2, result);
            break;
        case '/':
            if (num2 != 0) {
                result = num1 / num2;
                printf("Result: %.2f / %.2f = %.2f\n", num1, num2, result);
            } else {
                printf("Error: Division by zero!\n");
            }
            break;
        default:
            printf("Error: Invalid operation!\n");
    }
    
    return 0;
}
```
### Результаты работы
![Результат](https://github.com/karinasergeeva77/c-programming-sergeeva/blob/61183702ba7387d8b3701b425b56db94fbe2c8df/practice-2/screenshots/task-2-6-1.png)
---
## Задание 2.7: Високосный год
### Постановка задачи
Напишите программу, определяющую, является ли год високосным по правилам григорианского календаря:
Год делится на 4 — високосный
НО: год делится на 100 — не високосный
НО: год делится на 400 — високосный
### Математическая модель
високосный=(годmod4=0 и годmod100!=0) или (годmod400=0)
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| year | int | Проверяемый год |
### Код программы
```c
#include <stdio.h>

int main(void) {
    int year;
    
    printf("Enter year: ");
    scanf("%d", &year);
    
    // Проверка по правилам григорианского календаря
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        printf("%d is a leap year\n", year);
    } else {
        printf("%d is not a leap year\n", year);
    }
    
    return 0;
}
```
### Результаты работы
![Результат](https://github.com/karinasergeeva77/c-programming-sergeeva/blob/61183702ba7387d8b3701b425b56db94fbe2c8df/practice-2/screenshots/task-2-7-1.png)(https://github.com/karinasergeeva77/c-programming-sergeeva/blob/61183702ba7387d8b3701b425b56db94fbe2c8df/practice-2/screenshots/task-2-7-2.png)
---
## Задание 2.8: Интеграл методом трапеций (e^(x+2))
### Постановка задачи
Напишите программу, которая вычислит определённый интеграл методом трапеций
### Математическая модель
f(x)dx=h(f(a)+f(b)/2+sum(n-1,i=1)f(a+i*h)
где $h = \frac{b-a}{n}$, $n$ — количество разбиений.
f(0,1)e^x+2 dx=e^1+2 - e^0+2 =e^3 -e^2
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| a | double | Нижний предел (0) |
| b | double | Верхний предел (1) |
| n | int | Количество разбиений (1000) |
| h | double | Шаг интегрирования |
| sum | double | Промежуточная сумма|
| integral | double | Приближённое значение интеграла |
| exact | double | Точное значение интеграла |
### Код программы
```c
#include <stdio.h>
#include <math.h>
// Подынтегральная функция
double f(double x) {
return exp(x + 2); // e^(x+2)
}
int main(void) {
double a = 0, b = 1; // Границы интегрирования
int n = 1000; // Количество разбиений
double h = (b - a) / n; // Шаг
// Сумма по методу трапеций
double sum = (f(a) + f(b)) / 2.0; // Первое и последнее значения с коэф. 1/2
for (int i = 1; i < n; i++) {
sum += f(a + i * h); // Промежуточные значения
}double integral = h * sum;
printf("Integral of e^(x+2) from %.1f to %.1f:\n", a, b);
printf("Numerical result: %.6f\n", integral);
// Аналитический ответ: e^(x+2) от a до b = e^(b+2) - e^(a+2)
double exact = exp(b + 2) - exp(a + 2);
printf("Exact result: %.6f\n"
, exact);
printf("Error: %.6f\n"
, fabs(integral - exact));
return 0;
}
```
### Результаты работы
![Результат](https://github.com/karinasergeeva77/c-programming-sergeeva/blob/61183702ba7387d8b3701b425b56db94fbe2c8df/practice-2/screenshots/task-2-8.png)
---
## Задание 2.9: Интеграл синуса методом трапеций
### Постановка задачи
Напишите программу, которая вычислит определённый интеграл методом трапеций
fsin(x)dx
### Математическая модель
fsin(x)dx=h(sin(0)+sin(π)/2 + sum(n-1,i=1)sin(i*h)
где $h = \frac{\π}{n}$, $n$ — количество разбиений.
fsin(x)dx=[-cos(x)](π, 0)=-cos(π)-(-cos(0))=1+1=2
### Список идентификаторов
| Имя | Тип | Описание |
|-----|-----|----------|
| a | double | Нижний предел (0) |
| b | double | Верхний предел (π) |
| n | int | Количество разбиений (1000) |
| h | double | Шаг интегрирования |
| sum | double | Промежуточная сумма|
| integral | double | Приближённое значение интеграла |
| exact | double | Точное значение интеграла |
### Код программы
```c
#include <stdio.h>
#include <math.h>

// Подынтегральная функция
double f(double x) {
    return sin(x);  // sin(x)
}

int main(void) {
    double a = 0, b = M_PI;  // Границы интегрирования: от 0 до π
    int n = 1000;            // Количество разбиений
    double h = (b - a) / n;  // Шаг
    
    // Сумма по методу трапеций
    double sum = (f(a) + f(b)) / 2.0;  // Первое и последнее значения с коэф. 1/2
    
    for (int i = 1; i < n; i++) {
        sum += f(a + i * h);  // Промежуточные значения
    }
    
    double integral = h * sum;
    
    printf("Integral of sin(x) from %.1f to %.1f:\n", a, b);
    printf("Numerical result: %.6f\n", integral);
    
    // Аналитический ответ: ∫ sin(x) dx от 0 до π = 2
    double exact = 2.0;  // cos(0) - cos(π) = 1 - (-1) = 2
    printf("Exact result: %.6f\n", exact);
    printf("Error: %.6f\n", fabs(integral - exact));
    
    return 0;
}
```
### Результаты работы
![Результат](https://github.com/karinasergeeva77/c-programming-sergeeva/blob/61183702ba7387d8b3701b425b56db94fbe2c8df/practice-2/screenshots/task-2-9.png)
