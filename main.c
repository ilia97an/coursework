#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include "interface.h"
//this program must be compiled in -std=c99 or -std=gnu99 mode

int main(){
    int menu = 1;
	init();
    setlocale(LC_CTYPE, "Rus");
    //system("mode 80, 30");   //���������� �������� � ����� ���� �������

    //���� ������ �������
    while(menu){
        system("cls");
        fflush(stdin);

        printf("\n �������� ����� ������, 0 ��� ������\n\n");
        printf("\t������ 1. ���������� ������������� ���������.\n");
        printf("\t������ 2. ���������� ������������� �������� �������.\n");
        printf("\t������ 3. ������ ������ ���� ��� ����������������� ���������.\n");
        printf("\t������ 4. ����� ������� ������� ������ ��� �� ������� �������.\n\n > ");
        menu = intFromTo(0, 4);    //���� ����� �� 0 �� 4

        //���������� ���������� �������
        switch(menu){
            case 1:
                startTask(task1);  //������� 1
                break;
            case 2:
                startTask(task2);  //������� 2
                break;
            case 3:
                startTask(task3);  //������� 3
                break;
            case 4:
                startTask(task4);  //������� 4
                break;
        }

    }
    system("cls");
    return 0;
}
