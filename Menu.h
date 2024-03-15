#pragma once
#include <iostream>
#include "File.h"

using namespace std;

class Menu
{
private:
	//Названия для файлов
	char binaryFile[9] = "tree.bin", txtFile[9] = "tree.txt";

public:
	//Список опций
	void Header();
	//Создание строки для добавления
	char* CreateString();
	//Создание числа для добавления
	int CreateNumber();
	//Генерация случайно строки
	char* GenerateString(int);
	//Генерация файла из случайных строк
	void GenerateFile(int, File*);
	//Вызов меню
	void Call();
};