#include <iostream>
#include <ctime>
#include <windows.h>
#include "Menu.h"

int main()
{
    setlocale(LC_ALL, "Russian");
    setlocale(0, "");
    //��������� ��� �������
    SetConsoleCP(1251);
    //��������� �������� �� �������� �������
    srand(clock());
    Menu menu;
    //����� ����
    menu.Call();
}