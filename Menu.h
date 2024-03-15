#pragma once
#include <iostream>
#include "File.h"

using namespace std;

class Menu
{
private:
	//�������� ��� ������
	char binaryFile[9] = "tree.bin", txtFile[9] = "tree.txt";

public:
	//������ �����
	void Header();
	//�������� ������ ��� ����������
	char* CreateString();
	//�������� ����� ��� ����������
	int CreateNumber();
	//��������� �������� ������
	char* GenerateString(int);
	//��������� ����� �� ��������� �����
	void GenerateFile(int, File*);
	//����� ����
	void Call();
};