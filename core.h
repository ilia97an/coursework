#ifndef CORE_H
#define CORE_H
#include <stdbool.h>


typedef double (* function)(double);
typedef struct{
    double value;
    bool error;
} integral; //задание 1
typedef struct{
    double x, y;
} coordinate; //задание 2, 3, 4
typedef struct{
    int n;
    coordinate * table;
} coorTable; //задание 3, 4
typedef struct
{
    double p_half, q, f, m, n, f2, c, d;
} coefficients; //задание 4
typedef struct{
    int n;
    double * y;
    bool error;
} boundary; 



void coreInit();
integral task1core(double a, double b, double requestedAccuracy, int fIndex);
#define TASK2_TABLE_MAX_SIZE 13
double task2core(double x, coorTable * givenTab);
coorTable task3core(double y0, double x0, double x2, double h);
coorTable task4core(double alpha0, double alpha1, double A,
                    double x0, double x3, double h,
                    double beta0, double B);



#endif
