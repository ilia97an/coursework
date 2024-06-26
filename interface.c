#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"

char * errMsgNoFile = "Такой файл не найден\n";           //размещение в памяти строк с пояснениями ошибок
char * errMsgNoNumber = "Ошибка чтения размера таблицы\n";
char * errMsgBigNumber = "Слишком большой размер матрицы\n";
void init(){
    coreInit();
}
typedef struct{
    int unsigned n;
    FILE * file;
    bool error;
    char * errorMsg;
} tableType;

//ввод чисел типа double
double doubleIn(char * str){
    double n;

    printf(" %s: ", str);
    fflush(stdin);
    while(scanf("%lf", &n) != 1){
        fflush(stdin);
        printf(" Ошибка ввода числа.\n %s: ", str);
    }
    fflush(stdin);

    return n;
}
//начало выполнения задания, повторное выполнение
void startTask(void (* foo)()){
    char repeat = 'r';
    while(repeat == 'r'){
        system("cls");
        foo();
        printf("\n Введите 'r' для повторного выполнения задания ");
        fflush(stdin);
        scanf("%c", &repeat);
    }
}
//выбор номера от 1 до n
int intFromTo(int a, int b){
    int i;  //введенное число
    fflush(stdin);
    while(scanf("%d", &i) != 1 || i < a || i > b){
        fflush(stdin);
        printf(" Введите число от %d до %d\n > ", a, b);
    }
    fflush(stdin);
    return i;
}

void task1(){
    integral result;    //значение определенного интеграла и флаг ошибки
    double a, b;   //пределы интегрирования
    double e;  //погрешность
    int i; //номер функции

    //вывод трех доступных функций
    printf("\n Введите номер функции для интегрирования, 0 для главного меню\n");
    printf("\n 1) f1 = x / (x^2 + 6x + 1)\n");
    printf("\n 2) f2 = 10^(-0.01x) * cos(0.125x)\n");
    printf("\n 3) f3 = arccos((3x + 25) / 100)\n");
    printf("\n > ");

    //ввод номера функции для интегрирования
    i = intFromTo(0, 3);   //ввод целого числа от 0 до 3

    //0 - возврат в главное меню
    if(i != 0){
        //ввод пределов интегрирования и точности
        puts("\n Введите пределы интегрирования и точность вычисления");
        a = doubleIn("Нижний предел");
        b = doubleIn("Верхний предел");
        printf(" Точность вычисления: ");
        fflush(stdin);
        while(scanf("%lf", &e) != 1 || (e <= 0)){
            fflush(stdin);
            printf(" Неверный ввод, введите число больше нуля.\n "
                   "Точность вычисления: ");
        }

        //вычисление определенного интеграла методом трапеций
        result = task1core(a, b, e, i);

        //вывод результатов вычисления
        if(result.error){
            printf("\n Задданная точность не может быть достигнута или функция "
                   "f%d имеет\n разрыв на отрезке [%.2f; %.2f]\n", i, a, b);
        }
        else{
            printf("\n Определенный интеграл для f%d от %.3f до %.3f = %.6f\n",
                   i, a, b, result.value);
        }
    }
}

//открытие файла
tableType validateTableF(char * str){
    tableType table;    //размещение в памяти структуры с флагом ошибки, указателем на файл, указателем на сообщение об ошибке
    table.error = false; //инициализация флага ошибки
    table.file = fopen(str, "r"); //открытие файла
    if (table.file != NULL){ 
        if (fscanf(table.file, "%d", &table.n) != 1){ //если открытие прошло успешно, считать размер матрицы из файла
            table.error = true;     //если прочитать размер таблицы не удалось, устанавливается соответствующая ошибка                    
            table.errorMsg = errMsgNoNumber;
        }
        else{
            if (table.n > TASK2_TABLE_MAX_SIZE){  //если считать размер таблицы удалось, но ее размер слишком велик, выдать ошибку
                table.error = true;
                table.errorMsg = errMsgBigNumber;
            }
        }
    }
    else{
        table.error = true;     //если открыть файл не удалось, выдать ошибку
        table.errorMsg = errMsgNoFile;
    }
    return table;
}
void task2(){
    double x, y;   //значение функции y для указанного x
    coorTable g; //размещение в памяти структуры с указателем на таблицу x, y; размером таблицы
    char tableName[128];   //имя файла с табличными данными
    tableType table;   //структура с файлом и количеством точек
    table.error = false; //инициализация флага ошибки
    //вывод задания
    printf("\n Вычислить приближенное значение функции, заданной таблично,\n"
           " используя вторую интерполяционную формулу Ньютона для заданного\n"
           " значения аргумента.\n");

    printf("\n Введите имя файла с табличной функцией\n\n > ");
    scanf("%s", tableName);
    table = validateTableF(tableName);
    if(!table.error){
        //создаем два массива для хранения x и y
        g.n = table.n;
        g.table = calloc(g.n, sizeof(coordinate));
        if (fscanf(table.file, "%lf", &x) != 1){
            table.error = true;
            printf("\n Ошибка при чтении указанного x (первая строка)\n");
        }
        //заполнение таблицы данными из файла
        for(int i = 0; i < g.n && !table.error; i++){
            if (fscanf(table.file, "%lf", &g.table[i].x) != 1){
                table.error = true;
                printf("\n Ошибка при чтении x (%d строка)\n", i + 2);
            }
            if (!table.error){
                if (fscanf(table.file, "%lf", &g.table[i].y) != 1){
                    table.error = true;
                    printf("\n Ошибка при чтении y (%d строка)\n", i + 2);
                }
            }
        }
        if (!table.error){
            //вывод содержимого массивов (таблица (x, y))
            printf("\nx % 10f\n", x);
            printf("\n%8sx    |%5sy\n", "", "");
            printf(" -----------------------\n");
            for(int i = 0; i < g.n; i++)
                printf("% 10f | %-10f\n", g.table[i].x, g.table[i].y);

            y = task2core(x, &g);

            //вывод результата
            printf("\n y(%.4f) = %f\n", x, y);

            //освобождение выделенной памяти
            free(g.table);
        }
    }
    else{
        //вывод сообщения об ошибке
        printf("\n %s \n", table.errorMsg);
    }

    fclose(table.file);
}

#define Y0 1.1 //начальные условия
#define X_START 0.2 //начальное значение аргумента
#define X_END 1.2  //конечное значение аргумента
#define H 0.1   //шаг изменения аргумента
void task3(){
    //вывод задания
    printf("\n Решить задачу Коши для дифференциального уравнения");
    printf("\n  y' = x + sin(y/1.5);");
    printf("\n  x = [%.1f; %.1f];", X_START, X_END);
    printf("\n  y(%.1f) = %.1f;", X_START, Y0);
    printf("\n  h = %.1f.\n", H);
    //вычисление расчетной таблицы
    coorTable tab = task3core(Y0, X_START, X_END, H);
    //вывод результатов
    printf("\n |   x   |    y\n -------------------\n");
    for(int i = 0; i < tab.n; i++){
        printf(" | %+.2f | %+f\n", tab.table[i].x, tab.table[i].y);
    }
    free(tab.table);
}
#undef Y0       //удаление макро
#undef X_START
#undef X_END
#undef H


#define ALPHA0 1.5  //ввод заранее заданных параметров
#define ALPHA1 -1
#define A 0.6
#define X0 1.3
#define X3 1.6
#define H 0.1
#define BETA0 2
#define B 0.3
void task4() {
    printf("\
       y'    2y     x\n\
y'' - --- + ---- = ---\n\
       4     x      2\n");
    printf("\n\
 /\n\
 ¦ 1,5 * y(1,3)  -   1  *  y'(1,3) = 0,6\n\
_¦\n\
 ¦ 2  *  y(1,6)  = 0,3\n\
 \\\n"); //г¦L¦//L¦+-
    coorTable coorTab = task4core(ALPHA0, ALPHA1, A, X0, X3, H, BETA0, B);
    printf("\n%d\n", coorTab.n);
    printf("\nx\t");
    for (int i = 0; i < coorTab.n; i++){
        coordinate * c = &coorTab.table[i];
        printf("% .3f\t", c->x);
    }
    printf("\ny\t");
    for (int i = 0; i < coorTab.n; i++){
        coordinate * c = &coorTab.table[i];
        printf("% .3f\t", c->y);
    }
    free(coorTab.table);
}
#undef ALPHA0   //освобождение макро идентефикаторов
#undef ALPHA1
#undef A
#undef X0
#undef X3
#undef H
#undef BETA0
#undef B
