#include <iostream>
#include <ctime>
#include <windows.h>
#include "Menu.h"

int main()
{
    setlocale(LC_ALL, "Russian");
    setlocale(0, "");
    //Кодировка для консоли
    SetConsoleCP(1251);
    //Случаеные значения от текущего времени
    srand(clock());
    Menu menu;
    //Вызов меню
    menu.Call();
}