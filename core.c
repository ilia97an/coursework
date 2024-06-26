#include <math.h>
#include <stdlib.h>
#include "core.h"

int unsigned factorial[TASK2_TABLE_MAX_SIZE];
void initFactorialTable();
void functionsInit();
void coreInit(){ //������������� ������� ������� � �����������
    functionsInit();
    initFactorialTable();
}



//������� ��� ��������������
double function1(double x){
    double y = x*(x + 1.5);
           y = y/(x + 0.6);
    return fabs(y);
}
double function2(double x){
    return sqrt(x*x - 5*x + 50);
}
double function3(double x){
    double y = x*x - x - 1;
           y = y/exp(x);
    return fabs(y);
}
function f[3]; //���������� � ������ ������������������ ���������� �� �������
void functionsInit(){ //��������� ��������� ������ ��� �������� �������
	f[0] = function1;
    f[1] = function2;
    f[2] = function3;	
}
double getNextStair(double stair){
    return stair/2.0; //�������� ��� ���������� ����� ���������� ���������
}
double estimateAccuracy(double oldIntegral, double recentIntegral){ 
    return fabs(oldIntegral - recentIntegral)/3.0; //��������� �������� ����������� ��������� �� ������ ������� ���������
}
double integrateWithStair(double a, double b, double stair, double (*requestedFunction)(double)){ //������� �������� � �������� ���������
    double x_0 = a, x_2 = x_0 + stair;//a, b -- ������� ��������������, stair -- ��� requestedFunction(double) -- ������������� �������
    double I = 0, trapezoid = 0; //I -- ���������� ��� ��������������� ���������, trapezoid -- ���������� ��� �������� ������� ��������
    while (x_2 <= b){
        trapezoid = requestedFunction(x_0) + requestedFunction(x_2); //��� ���������� ������� �������� ��������� ������� ��������������
        trapezoid *= stair/2.0;                                      //�������� ������� � ���� ��������� ���� �� ������ ������
        I += trapezoid;     //����� ������� ������������� �� ���� ��������
        x_0 = x_2;   //� ���� � ��������� ��������� ���������� ����� x
        x_2 += stair;
    }
    return I;
}
//���������� ������������� ��������� ������� �������� � �������� ���������
//������� �������������� -- a, b, �������� requestedAccuracy, ��� ������� ��� ������� fIndex
integral task1core(double a, double b, double requestedAccuracy, int fIndex){
    fIndex--; //������ ��������� ������� � C ���������� � 0, � �� � 1
    integral res; //������ ��������� � ���� ������
    res.value = 0; //������������� ��������� ���������
    double stair = getNextStair(fabs(a - b)); //��� ��������� ��������
    double I_old = integrateWithStair(a, b, stair, f[fIndex]); //��������� ������� ���������
    double recentAccuracy = 0; //��������
    int steps = 1;
    do{
        steps *= 2; //��������� ���������� �����
        stair = getNextStair(stair); //���������� ����
        res.value = integrateWithStair(a, b, stair, f[fIndex]); //���������� ������ ���������
        recentAccuracy = estimateAccuracy(I_old, res.value); //�������� ��������
        I_old = res.value; //������������� �������� ���������
        if (steps > 3e7)
            res.error = true;   //������������� ���� ������
    }while (!res.error && recentAccuracy > requestedAccuracy); //����� �� �����
    return res;
}



void initFactorialTable(){ //��������� ������� ����������� ���� ��� ����� ������� ���������, ����� �� ��������� ������ ��� ������
    factorial[0] = 1;
    int i = 0;
    for (i = 1; i < TASK2_TABLE_MAX_SIZE; i++)
        factorial[i] = factorial[i - 1] * i;
}
void initDeltaTable(double tab[TASK2_TABLE_MAX_SIZE][TASK2_TABLE_MAX_SIZE], coorTable * givenTab){ //������������� ������� ��������
    int i = 0, j = 0;         //������� ��������� ������ ���������� �������
    for (i = 0; i < givenTab->n; i++){  //����� ������� ������� -- � ���� �������� �������� �������
        for (j = 0; j < givenTab->n; j++)
            tab[i][j] = 0;
        tab[0][i] = givenTab->table[i].y; //� ������ ������� �������� ��������� �������� �������
    }
}
void fillDeltaTable(double tab[TASK2_TABLE_MAX_SIZE][TASK2_TABLE_MAX_SIZE], coorTable * givenTab){ //���������� ������� ����������
    initDeltaTable(tab, givenTab); //������� ��������� ������������� �������, ����� ���������� ����������
    int i = 0, j = 0; //������������� ���������� �����
    for (i = 1; i < givenTab->n; i++){                    //���� �������� � ������� ���������
        double * recentRow = tab[i - 1];
        for (j = 0; j < givenTab->n - i; j++)
            tab[i][j] = recentRow[j + 1] - recentRow[j]; //��������� ������
    }
}
double findY(coorTable * g, double t, //g �������� ��������� �� ������� �������� �������� ������� � ���������
double delta[TASK2_TABLE_MAX_SIZE][TASK2_TABLE_MAX_SIZE]){
    double y = g->table[g->n - 1].y;   //������������� ���������� ��� y
    double overlappingMultiplier = t;  //������������� ���������, ������� ����������� �� ������ ����
    int i = 0;
//    printf("%-12s%-12s%-12s%-12s%-12s\n", "i", "ovlpMplier", "fact", "recAd", "delta");
    for (i = 1; i < g->n; i++){
        double recentAddendum = overlappingMultiplier/factorial[i]; //��������� ������ ����������
        recentAddendum *= delta[i][g->n - i - 1];
//        printf("%-12i% -12Lf%-12u% -12Lf% -12Lf\n", i, overlappingMultiplier, factorial[i], recentAddendum, delta[i][g->n - i - 1]);
        y += recentAddendum;    //���������� ���������� � ����������
        overlappingMultiplier *= t + i;    //���������� ���������, ������� ����������� �� ������ ����
    }
    return y;
}
//���������� ������������� y, ��������� ������ ���������������� ������� �������
//��������� g � �������� ������� � ���������� �� ������� �� ���������� x, y � �������� ��������� x
double task2core(double x, coorTable * g){
    double h = g->table[1].x - g->table[0].x; //������������� ���� ���������
    double t = (x - g->table[g->n - 1].x)/h; //x -- ���������� ��� x �� ����������, t -- ��������� �������� ���������� x � ���������� ���������� x � ����
    double deltaTable[TASK2_TABLE_MAX_SIZE][TASK2_TABLE_MAX_SIZE]; //���������� � ������ ������� ��������
    fillDeltaTable(deltaTable, g); //���������� �������� (����������) �������
    int i = 0, j = 0; //���������� ��� �����
//    puts("������� ���������");
//    for (i = 0; i < g->n; i++){ //����� ���������� ������� ���������
//        for (j = 0; j < g->n - i; j++)
//            printf("% -13.9Lf", deltaTable[j][i]);
//        puts("\n");
//    }
    return findY(g, t, deltaTable); //deltaTable -- ������� ���������; g �������� ��������� �� ������� ��������� �������� x, y; n 
}



double t3fv2(double x, double y){ //Task3 ���������� �������
    return (x + sin(y/sqrt(1.5)));
}
#define f t3fv2
coorTable task3core(double y0, double x0, double x2, double h){ //
    coorTable t;
    int n = t.n = (x2 - x0)/h + 1;
    coordinate * table = t.table = calloc(n, sizeof(coordinate));
    table[0].x = x0, table[0].y = y0;
    int i = 0;
    for (i = 0; i < n - 1; i++){
        coordinate * row0 = &table[i];
        coordinate * row2 = &table[i + 1];
        row2->x = row0->x + h;
        double f0 = f(row0->x, row0->y);
        double y_star = row0->y + h * f0;
        row2->y = row0->y + h * f0 + f(row2->x, y_star);
    }
    return t; //����� ����� ������������� ���������� ����������, ����� ������� �� ������ t.table
}
#undef f



double pX(){
    return -1.0/4.0;
}
double qX(double x){
    return 2.0/x;
}
double fX(double x){
    return x/2.0;
}
double mX(double p_i_half, double q_i, double h, double h_square){
    double result = 2 - q_i * h_square;
    result /= 1 + p_i_half * h;
    return result;
}
double nX(double p_i_half, double h){
    double result = 1 - p_i_half * h;
    result /= 1 + p_i_half * h;
    return result;
}
double f2X(double f_i, double p_i_half, double h){
    double result = f_i;
    result /= 1 + p_i_half * h;
    return result;
}
double cX(double m_i, double n_i, double old_c_i){
    double result = 1;
    result /= m_i - n_i * old_c_i;
    return result;
}
double dX(double f2_i, double n_i, double old_c_i, double old_d_i, double h_square){
    double result = f2_i * h_square;
    result -= n_i * old_c_i * old_d_i;
    return result;
}
double c_0(double alpha0, double alpha1, double h){
    double result = alpha1;
    result /= alpha0*h - alpha1;
    return result;
}
double d_0(double A, double h, double alpha1){
    double result = A*h;
    result /= alpha1;
    return result;
}
void independentCoefficientsEstimate(coefficients * k, double x, double h, double h_square){
    k->p_half = pX()/2.0;
    k->q = qX(x);
    k->f = fX(x);
    k->m = mX(k->p_half, k->q, h, h_square);
    k->n = nX(k->p_half, h);
    k->f2 = f2X(k->f, k->p_half, h);
}
void findCoefficients(coorTable * coorTab, coefficients * k,
double alpha0, double alpha1, double A,
double x0, double h){
    coordinate * table = coorTab->table;
    double x = x0;
    table[0].x = x;
    double h_square = h * h;
    independentCoefficientsEstimate(&k[0], x, h, h_square);
    k[0].c = c_0(alpha0, alpha1, h);
    k[0].d = d_0(A, h, alpha1); //������������� ��������� ����������
    int i = 1;
    for (i = 1; i < coorTab->n; i++){
        x += h;
        table[i].x = x;
        independentCoefficientsEstimate(&k[i], x, h, h_square);
        k[i].c = cX(k[i].m, k[i].n, k[i - 1].c);
        k[i].d = dX(k[i].f2, k[i].n, k[i - 1].c, k[i - 1].d, h_square);
    }
}
void findYs(coorTable * table, coefficients * k, double beta0, double B){
    int n = table->n;
    coordinate * t = table->table;
    t[n - 1].y = B/beta0;
    int i = n - 2;
    for (i = n - 2; i > 0; i--)
        t[i].y = k[i].c * (k[i].d - t[i + 1].y);
    t[0].y = k[0].c * (k[0].d - t[1].y);
}
coorTable task4core(double alpha0, double alpha1, double A,
double x0, double x3, double h,
double beta0, double B){
    coorTable coorTab;
    coorTab.n = (x3 - x0)/h + 1;
    coefficients * coefs = calloc(coorTab.n, sizeof(coefficients));
    coorTab.table = calloc(coorTab.n, sizeof(coordinate));
    findCoefficients(&coorTab, coefs, alpha0, alpha1, A, x0, h);
//    printf(" p\t q\t f\t m\t n\t f2\t c\t d\n");
    int i = 0;
//    for (i = 0; i < coorTab.n; i++){
//        coefficients * c = &coefs[i];
//        printf("% .2f\t% .3f\t% .3f\t% .3f\t% .3f\t% .3f\t% .3f\t% .3f\t\n", c->p_half*2.0, c->q, c->f, c->m, c->n, c->f2, c->c, c->d);
//    }
    findYs(&coorTab, coefs, beta0, B);
//    printf("\nx\t");
//    for (i = 0; i < coorTab.n; i++){
//        coordinate * c = &coorTab.table[i];
//        printf("% .3Lf\t", c->x);
//    }
//    printf("\ny\t");
//    for (i = 0; i < coorTab.n; i++){
//        coordinate * c = &coorTab.table[i];
//        printf("% .3Lf\t", c->y);
//    }
//    printf("\n");
    free(coefs); //������������ ����� � ������, ������� �������� ������������
    return coorTab;
}
