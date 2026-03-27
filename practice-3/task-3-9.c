#include <stdio.h>
int main(void) {
int arr[] = {64, 34, 25, 12, 22};
int n = 5;
// Внешний цикл: количество проходов
for (int i = 0; i < n - 1; i++) {
// Внутренний цикл: сравнение соседей
// (n - 1 - i) потому что последние i элементов уже на месте
for (int j = 0; j < n - 1 - i; j++) {
if (arr[j] > arr[j + 1]) {
// Меняем местами
int temp = arr[j];
arr[j] = arr[j + 1];
arr[j + 1] = temp;
}
}
}
// Вывод результата
printf("Sorted: ");
for (int i = 0; i < n; i++) {
printf("%d "
, arr[i]);
}
printf("\n");
return 0;
}
