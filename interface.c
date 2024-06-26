#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"

char * errMsgNoFile = "����� ���� �� ������\n";           //���������� � ������ ����� � ����������� ������
char * errMsgNoNumber = "������ ������ ������� �������\n";
char * errMsgBigNumber = "������� ������� ������ �������\n";
void init(){
    coreInit();
}
typedef struct{
    int unsigned n;
    FILE * file;
    bool error;
    char * errorMsg;
} tableType;

//���� ����� ���� double
double doubleIn(char * str){
    double n;

    printf(" %s: ", str);
    fflush(stdin);
    while(scanf("%lf", &n) != 1){
        fflush(stdin);
        printf(" ������ ����� �����.\n %s: ", str);
    }
    fflush(stdin);

    return n;
}
//������ ���������� �������, ��������� ����������
void startTask(void (* foo)()){
    char repeat = 'r';
    while(repeat == 'r'){
        system("cls");
        foo();
        printf("\n ������� 'r' ��� ���������� ���������� ������� ");
        fflush(stdin);
        scanf("%c", &repeat);
    }
}
//����� ������ �� 1 �� n
int intFromTo(int a, int b){
    int i;  //��������� �����
    fflush(stdin);
    while(scanf("%d", &i) != 1 || i < a || i > b){
        fflush(stdin);
        printf(" ������� ����� �� %d �� %d\n > ", a, b);
    }
    fflush(stdin);
    return i;
}

void task1(){
    integral result;    //�������� ������������� ��������� � ���� ������
    double a, b;   //������� ��������������
    double e;  //�����������
    int i; //����� �������

    //����� ���� ��������� �������
    printf("\n ������� ����� ������� ��� ��������������, 0 ��� �������� ����\n");
    printf("\n 1) f1 = x / (x^2 + 6x + 1)\n");
    printf("\n 2) f2 = 10^(-0.01x) * cos(0.125x)\n");
    printf("\n 3) f3 = arccos((3x + 25) / 100)\n");
    printf("\n > ");

    //���� ������ ������� ��� ��������������
    i = intFromTo(0, 3);   //���� ������ ����� �� 0 �� 3

    //0 - ������� � ������� ����
    if(i != 0){
        //���� �������� �������������� � ��������
        puts("\n ������� ������� �������������� � �������� ����������");
        a = doubleIn("������ ������");
        b = doubleIn("������� ������");
        printf(" �������� ����������: ");
        fflush(stdin);
        while(scanf("%lf", &e) != 1 || (e <= 0)){
            fflush(stdin);
            printf(" �������� ����, ������� ����� ������ ����.\n "
                   "�������� ����������: ");
        }

        //���������� ������������� ��������� ������� ��������
        result = task1core(a, b, e, i);

        //����� ����������� ����������
        if(result.error){
            printf("\n ��������� �������� �� ����� ���� ���������� ��� ������� "
                   "f%d �����\n ������ �� ������� [%.2f; %.2f]\n", i, a, b);
        }
        else{
            printf("\n ������������ �������� ��� f%d �� %.3f �� %.3f = %.6f\n",
                   i, a, b, result.value);
        }
    }
}

//�������� �����
tableType validateTableF(char * str){
    tableType table;    //���������� � ������ ��������� � ������ ������, ���������� �� ����, ���������� �� ��������� �� ������
    table.error = false; //������������� ����� ������
    table.file = fopen(str, "r"); //�������� �����
    if (table.file != NULL){ 
        if (fscanf(table.file, "%d", &table.n) != 1){ //���� �������� ������ �������, ������� ������ ������� �� �����
            table.error = true;     //���� ��������� ������ ������� �� �������, ��������������� ��������������� ������                    
            table.errorMsg = errMsgNoNumber;
        }
        else{
            if (table.n > TASK2_TABLE_MAX_SIZE){  //���� ������� ������ ������� �������, �� �� ������ ������� �����, ������ ������
                table.error = true;
                table.errorMsg = errMsgBigNumber;
            }
        }
    }
    else{
        table.error = true;     //���� ������� ���� �� �������, ������ ������
        table.errorMsg = errMsgNoFile;
    }
    return table;
}
void task2(){
    double x, y;   //�������� ������� y ��� ���������� x
    coorTable g; //���������� � ������ ��������� � ���������� �� ������� x, y; �������� �������
    char tableName[128];   //��� ����� � ���������� �������
    tableType table;   //��������� � ������ � ����������� �����
    table.error = false; //������������� ����� ������
    //����� �������
    printf("\n ��������� ������������ �������� �������, �������� ��������,\n"
           " ��������� ������ ���������������� ������� ������� ��� ���������\n"
           " �������� ���������.\n");

    printf("\n ������� ��� ����� � ��������� ��������\n\n > ");
    scanf("%s", tableName);
    table = validateTableF(tableName);
    if(!table.error){
        //������� ��� ������� ��� �������� x � y
        g.n = table.n;
        g.table = calloc(g.n, sizeof(coordinate));
        if (fscanf(table.file, "%lf", &x) != 1){
            table.error = true;
            printf("\n ������ ��� ������ ���������� x (������ ������)\n");
        }
        //���������� ������� ������� �� �����
        for(int i = 0; i < g.n && !table.error; i++){
            if (fscanf(table.file, "%lf", &g.table[i].x) != 1){
                table.error = true;
                printf("\n ������ ��� ������ x (%d ������)\n", i + 2);
            }
            if (!table.error){
                if (fscanf(table.file, "%lf", &g.table[i].y) != 1){
                    table.error = true;
                    printf("\n ������ ��� ������ y (%d ������)\n", i + 2);
                }
            }
        }
        if (!table.error){
            //����� ����������� �������� (������� (x, y))
            printf("\nx % 10f\n", x);
            printf("\n%8sx    |%5sy\n", "", "");
            printf(" -----------------------\n");
            for(int i = 0; i < g.n; i++)
                printf("% 10f | %-10f\n", g.table[i].x, g.table[i].y);

            y = task2core(x, &g);

            //����� ����������
            printf("\n y(%.4f) = %f\n", x, y);

            //������������ ���������� ������
            free(g.table);
        }
    }
    else{
        //����� ��������� �� ������
        printf("\n %s \n", table.errorMsg);
    }

    fclose(table.file);
}

#define Y0 1.1 //��������� �������
#define X_START 0.2 //��������� �������� ���������
#define X_END 1.2  //�������� �������� ���������
#define H 0.1   //��� ��������� ���������
void task3(){
    //����� �������
    printf("\n ������ ������ ���� ��� ����������������� ���������");
    printf("\n  y' = x + sin(y/1.5);");
    printf("\n  x = [%.1f; %.1f];", X_START, X_END);
    printf("\n  y(%.1f) = %.1f;", X_START, Y0);
    printf("\n  h = %.1f.\n", H);
    //���������� ��������� �������
    coorTable tab = task3core(Y0, X_START, X_END, H);
    //����� �����������
    printf("\n |   x   |    y\n -------------------\n");
    for(int i = 0; i < tab.n; i++){
        printf(" | %+.2f | %+f\n", tab.table[i].x, tab.table[i].y);
    }
    free(tab.table);
}
#undef Y0       //�������� �����
#undef X_START
#undef X_END
#undef H


#define ALPHA0 1.5  //���� ������� �������� ����������
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
 � 1,5 * y(1,3)  -   1  *  y'(1,3) = 0,6\n\
_�\n\
 � 2  *  y(1,6)  = 0,3\n\
 \\\n"); //�L�//L�+-
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
#undef ALPHA0   //������������ ����� ���������������
#undef ALPHA1
#undef A
#undef X0
#undef X3
#undef H
#undef BETA0
#undef B
