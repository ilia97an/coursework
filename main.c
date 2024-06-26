#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include "interface.h"
//this program must be compiled in -std=c99 or -std=gnu99 mode

int main(){
    int menu = 1;
	init();
    setlocale(LC_CTYPE, "Rus");
    //system("mode 80, 30");   //количество столбцов и строк окна консоли

    //меню выбора задания
    while(menu){
        system("cls");
        fflush(stdin);

        printf("\n Выберите номер задачи, 0 для выхода\n\n");
        printf("\tЗадача 1. Вычисление определенного интеграла.\n");
        printf("\tЗадача 2. Нахождение приближенного значения функции.\n");
        printf("\tЗадача 3. Решить задачу Коши для дифференциального уравнения.\n");
        printf("\tЗадача 4. Найти решение краевой задачи для ДУ второго порядка.\n\n > ");
        menu = intFromTo(0, 4);    //ввод числа от 0 до 4

        //выполнение выбранного задания
        switch(menu){
            case 1:
                startTask(task1);  //задание 1
                break;
            case 2:
                startTask(task2);  //задание 2
                break;
            case 3:
                startTask(task3);  //задание 3
                break;
            case 4:
                startTask(task4);  //задание 4
                break;
        }

    }
    system("cls");
    return 0;
}
