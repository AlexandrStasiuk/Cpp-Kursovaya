#include <iostream>
#include <fstream>
#include "File.h"
#include <string>
#include <cstdio>

using namespace std;
//Конструктор для открытия файла
File::File(char* nameFile)
{
	//Запись имени файла
	this->nameFile = nameFile;
	//Открытие файла с аттрибутами
	open(nameFile, fstream::binary | fstream::in | fstream::out);
	if (!is_open())
	{
		//Если файл не открылся, то необходимо создать
		ofstream f(nameFile, fstream::binary);
		f.close();
		//Повторное открытие файла с аттрибутами
		open(nameFile, fstream::binary | fstream::in | fstream::out);
		//Позиция записи в начало файла
		seekp(0, ios_base::beg);
		//Запись корневой вершины
		write((char*)&pointHead, sizeof(int));
		//Запись количества элементов
		write((char*)&count, sizeof(int));
	}
	//Чтение корневой вершины
	seekg(0, ios_base::beg);
	read((char*)&pointHead, sizeof(int));
	//Чтение количества конечных вершин
	seekg(4, ios_base::beg);
	read((char*)&count, sizeof(int));
}
//Деструктор для закрытия файла
File::~File()
{
	//Закрытие файла
	close();
}
//Чтение и запись элемента в узел
void File::readNode(int cursor, Node* node)
{
	//Смещение указателя на указанный
	seekg(cursor, ios_base::beg);
	//Чтение данных и запись в узел
	read((char*)&node->countNodes, sizeof(int));
	read((char*)&node->left, sizeof(int));
	read((char*)&node->right, sizeof(int));
	int len = 0;
	read((char*)&len, sizeof(int));
	char* strRead = new char[len + 1];
	read(strRead, len);
	strRead[len] = '\0';
	node->str = strRead;
	close();
	open(nameFile, fstream::binary | fstream::in | fstream::out);
}
//Запись элемента из узла
void File::writeNode(int cursor, Node* node)
{
	//Смещение указателя на указанный
	seekp(cursor, ios_base::beg);
	//Запись данных из узла в файл
	write((char*)&node->countNodes, sizeof(int));
	write((char*)&node->left, sizeof(int));
	write((char*)&node->right, sizeof(int));
	int len = strlen(node->str);
	write((char*)&len, sizeof(int));
	write(node->str, len);
	close();
	open(nameFile, fstream::binary | fstream::in | fstream::out);
}
//Вставка узла в файл
void File::insertFileNode(int cursor, Node* node) {
	//Получение размера файла в байтах
	seekg(0, ios::end);
	streampos fileSize = tellg();
	seekg(0, ios::beg);
	char* buffer = new char[fileSize];
	//Запись данных в строку
	read(buffer, fileSize);
	//Создание нового файла
	char tempName[6] = "1.bin";
	ofstream tempFile(tempName, fstream::binary | fstream::out);
	//Запись данных до нового элемента
	tempFile.write(buffer, cursor);
	//Запись нового элемента
	tempFile.write((char*)&node->countNodes, sizeof(int));
	tempFile.write((char*)&node->left, sizeof(int));
	tempFile.write((char*)&node->right, sizeof(int));
	int len = strlen(node->str);
	tempFile.write((char*)&len, sizeof(int));
	tempFile.write(node->str, len);
	//Запись данных после нового элемента
	tempFile.write(buffer + cursor, fileSize - static_cast<fstream::pos_type>(cursor));
	//Очистка выделенной памяти
	delete[] buffer;
	//Закрытие файлов
	tempFile.close();
	close();
	//Удаление старого бинарного файла
	remove(nameFile);
	//Изменение имени нового бинарного файла
	rename(tempName, nameFile);
	//Открытие нового бинарного файла
	open(nameFile, fstream::binary | fstream::in | fstream::out);
}
//Удаление строки из промежуточной вершины
void File::DeleteNodeStr(int cursor, int strLenth) {
	//Получение размера файла в байтах
	seekg(0, ios::end);
	streampos fileSize = tellg();
	seekg(0, std::ios::beg);
	char* buffer = new char[fileSize];
	//Запись данных в строку
	read(buffer, fileSize);
	//Создание нового файла
	char tempName[6] = "1.bin";
	ofstream tempFile(tempName, fstream::binary | fstream::out);
	//Запись данных до удаляемой строки
	tempFile.write(buffer, cursor);
	//Запись данных после удаляемой строки
	tempFile.write(buffer + cursor + strLenth, fileSize - static_cast<fstream::pos_type>(cursor + strLenth));
	//Очистка выделенной памяти
	delete[] buffer;
	//Закрытие файлов
	tempFile.close();
	close();
	//Удаление старого бинарного файла
	remove(nameFile);
	//Изменение имени нового бинарного файла
	rename(tempName, nameFile);
	//Открытие нового бинарного файла
	open(nameFile, fstream::binary | fstream::in | fstream::out);
}
//Получение указателя позиционирования по номеру узла
int File::GetCursorNode(int beginCursor, int countNodes) {
	//Счетчик указателя
	int countFileStep = beginCursor;
	//Перебор элементов по указателю
	for (int i = 0; i < countNodes; i++) {
		countFileStep += sizeof(int) * 3;
		seekg(countFileStep, ios_base::beg);
		int len = 0;
		read((char*)&len, sizeof(int));
		countFileStep += sizeof(int) + len;
	}
	return countFileStep;
}
//Получение указателя на узел по номеру элемента
int File::GetNode(int number) {
	int stepNodes = 8;
	string step = "";
	//Перебор шагов для перехода к нужному элементу
	while (number > 1)
	{
		if (number % 2 == 0)
			step += "1";
		else
			step += "2";
		number /= 2;
	}
	//Переход к нужному узлу
	for (int i = step.length() - 1; i >= 0; --i)
	{
		//Если шаг равен 1, то текущий узел становится левым
		if (step[i] == '1')
		{
			stepNodes = GetCursorNode(stepNodes, 1);
		}
		else
		{
			//Иначе правым
			Node* left = new Node;
			int cursorLeft = GetCursorNode(stepNodes, 1);
			readNode(cursorLeft, left);
			stepNodes = GetCursorNode(stepNodes, left->countNodes + 2);
		}
	}
	return stepNodes;
}
//Изменение количества потомков
void File::ChangeCountNodes(int number, int value)
{
	Node* node = new Node;
	readNode(8, node);
	int fileStep = 0;
	int countFileStep = 8;
	//Переменная для поиска нужного узла
	string step = "";
	//Вычисление шагов, которые необходимы для перехода к узлу
	while (number > 1)
	{
		if (number % 2 == 0)
			step += "1";
		else
			step += "2";

		number /= 2;
	}
	//Переход к нужному узлу
	for (int i = step.length() - 1; i >= 0; --i)
	{
		if (node == nullptr)
			break;
		//Изменение количества потомков на указанное значение
		node->countNodes += value;
		seekp(countFileStep, ios_base::beg);
		write((char*)&node->countNodes, sizeof(int));
		//Если шаг равен 1, то текущий узел становится левым
		if (step[i] == '1')
		{
			countFileStep = GetCursorNode(countFileStep, 1);
		}
		else
		{
			//Иначе правым
			Node* left = new Node;
			int cursorLeft = GetCursorNode(countFileStep, 1);
			readNode(cursorLeft, left);
			countFileStep = GetCursorNode(countFileStep, left->countNodes + 2);
		}
		//Чтение следующего узла
		readNode(countFileStep, node);
	}
}
//Получение свободного узла
int File::GetFreeNode(Node* node, int cursor)
{
	//Текущий узел
	Node* current = node;
	int currentCursor = cursor;
	//Проверка на пустых потомков
	if (current->left == 0 || current->right == 0)
		return cursor;
	int stepLeft = GetCursorNode(cursor, 1);
	Node* currentLeft = new Node;
	readNode(stepLeft, currentLeft);
	Node* currentRight = new Node;
	int stepRight = GetCursorNode(cursor, currentLeft->countNodes + 2);
	readNode(stepRight, currentRight);
	//Сравнение количества потомков и выбор между левым и правым узлом
	if (currentLeft->countNodes <= currentRight->countNodes || currentLeft->countNodes + 1 < pow(2, (int)log2(current->countNodes + 1)) - 1)
	{
		currentCursor = GetFreeNode(currentLeft, stepLeft);
	}
	else if (currentLeft->countNodes > currentRight->countNodes)
	{
		currentCursor = GetFreeNode(currentRight, stepRight);
	}
	return currentCursor;
}
//Расчет количества конечных вершин
int File::CountEndPoint(int number) {
	//Расчет количества конечных вершин
	int countEndPoint = 0;
	if (number % 2 == 0)
	{
		countEndPoint = number / 2;
	}
	else
	{
		countEndPoint = (number + 1) / 2;
	}
	return countEndPoint;
}
//Вставка элемента
void File::Insert(char* str)
{
	//Вставка в первый узел
	if (pointHead == 0)
	{
		firstNode = new Node;
		firstNode->str = str;
		count++;
		//Перезапись корневой вершины
		seekp(0, ios_base::beg);
		pointHead = 4;
		write((char*)&pointHead, sizeof(int));
		//Запись количества конечных вершин
		seekp(4, ios_base::beg);
		write((char*)&count, sizeof(int));
		//Запись строки и ее длины
		writeNode(8, firstNode);
	}
	else {
		firstNode = new Node;
		readNode(8, firstNode);
		//Получение свободного узла
		int freeNodeCursor = GetFreeNode(firstNode, 8);
		Node* freeNode = new Node;
		readNode(freeNodeCursor, freeNode);
		//Создание нового узла с передаваемым значением
		Node* newNode = new Node;
		newNode->str = str;
		if (freeNode->left == 0)
		{
			//Создание левого узла
			Node* leftNode = new Node;
			//Добавление в него значения
			leftNode->str = freeNode->str;
			//Очистка строки
			freeNode->str = nullptr;
			//Создание левого узла в свободном узле 
			freeNode->left = 1;
			seekp(freeNodeCursor + sizeof(int), ios_base::beg);
			write((char*)&freeNode->left, sizeof(int));
			int len = 0;
			seekp(freeNodeCursor + sizeof(int) * 3, ios_base::beg);
			write((char*)&len, sizeof(int));
			//Удаление строки из промежуточной вершины
			DeleteNodeStr(freeNodeCursor + sizeof(int) * 4, strlen(leftNode->str));
			int insertCursor = freeNodeCursor + sizeof(int) * 4 + len;
			//Вставка элемента в файл
			insertFileNode(insertCursor, leftNode);
			//Запись нового узла в правый узел свободного
			freeNode->right = 1;
			count += 2;
			seekp(freeNodeCursor + sizeof(int) * 2, ios_base::beg);
			write((char*)&freeNode->right, sizeof(int));
			insertCursor += sizeof(int) * 4 + strlen(leftNode->str);
			//Вставка элемента в файл
			insertFileNode(insertCursor, newNode);
			//Изменение количества потомков
			ChangeCountNodes(count, 2);
		}
		else
		{
			//Добавление нового значения в правый узел
			freeNode->right = 1;
			count++;
			seekp(freeNodeCursor + sizeof(int) * 2, ios_base::beg);
			write((char*)&freeNode->right, sizeof(int));
			int insertCursor = GetCursorNode(freeNodeCursor, freeNode->countNodes + 1);
			//Вставка элемента в файл
			insertFileNode(insertCursor, newNode);
			//Изменение количества потомков
			ChangeCountNodes(count, 1);
		}
		seekp(4, ios_base::beg);
		//Запись количества конечных вершин
		write((char*)&count, sizeof(int));
	}
}
//Удаление элемента
void File::DeleteLast()
{
	if (count == 0) {
		cout << "Файл не содержит элементов" << endl;
		return;
	}
	//Если в дереве только 1 элемент, то удаление корневой вершины
	if (count == 1)
	{
		//Перезапись корневой вершины
		char tempName[6] = "1.bin";
		//Создание нового файла без элементов
		ofstream tempFile(tempName, fstream::binary | fstream::out);
		tempFile.seekp(0, ios_base::beg);
		pointHead = 0;
		tempFile.write((char*)&pointHead, sizeof(int));
		count = 0;
		tempFile.write((char*)&count, sizeof(int));
		//Закрытие файлов
		tempFile.close();
		close();
		//Удаление старого бинарного файла
		remove(nameFile);
		//Изменение имени нового бинарного файла
		rename(tempName, nameFile);
		//Открытие нового бинарного файла
		open(nameFile, fstream::binary | fstream::in | fstream::out);
	}
	else {
		//Получение родительской вершины у удаляемой
		Node* parentLastNode = new Node;
		int parentLastNodeCursor = GetNode(count / 2);
		readNode(parentLastNodeCursor, parentLastNode);
		//Изменение количества потомков
		ChangeCountNodes(count / 2, -2);
		//Если общее количество элементов четное, то удаляется левый, иначе правый
		int deletedNode = 0;
		if (this->count % 2 == 0 && this->count != 2) {
			Node* leftNode = new Node;
			deletedNode = GetCursorNode(parentLastNodeCursor, 1);
			readNode(deletedNode, leftNode);
			parentLastNode->left = 0;
			parentLastNode->right = 0;
			parentLastNode->str = leftNode->str;
			parentLastNode->countNodes--;
			count--;
			deletedNode = GetCursorNode(deletedNode, 1);
		}
		else {
			Node* leftNode = new Node;
			int leftDeletedNode = GetCursorNode(parentLastNodeCursor, 1);
			readNode(leftDeletedNode, leftNode);
			deletedNode = GetCursorNode(parentLastNodeCursor, leftNode->countNodes + 2);
			parentLastNode->right = 0;
			parentLastNode->left = 0;
			parentLastNode->str = leftNode->str;
			parentLastNode->countNodes -= 2;
			count -= 2;
			deletedNode = GetCursorNode(deletedNode, 1);
		}
		seekg(4, std::ios::beg);
		write((char*)&count, sizeof(int));
		//Создание нового бинарного файла
		seekg(0, ios::end);
		streampos fileSize = tellg();
		seekg(0, std::ios::beg);
		char* buffer = new char[fileSize];
		read(buffer, fileSize);
		char tempName[6] = "1.bin";
		ofstream tempFile(tempName, fstream::binary | fstream::out);
		//Запись до удаляемого элемента
		tempFile.write(buffer, parentLastNodeCursor);
		tempFile.write((char*)&parentLastNode->countNodes, sizeof(int));
		tempFile.write((char*)&parentLastNode->left, sizeof(int));
		tempFile.write((char*)&parentLastNode->right, sizeof(int));
		int len = strlen(parentLastNode->str);
		tempFile.write((char*)&len, sizeof(int));
		tempFile.write(parentLastNode->str, len);
		//Запись после удаляемого элемента
		tempFile.write(buffer + deletedNode, fileSize - static_cast<fstream::pos_type>(deletedNode));
		//Очистка памяти
		delete[] buffer;
		//Закрытие файлов
		tempFile.close();
		close();
		//Удаление старого бинарного файла
		remove(nameFile);
		//Изменение имени нового бинарного файла
		rename(tempName, nameFile);
		//Открытие нового бинарного файла
		open(nameFile, fstream::binary | fstream::in | fstream::out);
	}
}
//Редактирование элемента
void File::Edit(int index, char* str)
{
	//Проверка на наличие элементов
	if (count == 0) {
		cout << "Файл не содержит элементов" << endl;
		return;
	}
	//Проверка на существование элемента
	if (index < CountEndPoint(count) && index >= 0) {

		Node* editNode = new Node;
		int editNodeCursor = 8;
		int countEndNodes = -1;
		//Перебор элементов и поиск нужного
		for (int i = 0; i < count; i++) {
			Node* tempNode = new Node;
			readNode(editNodeCursor, tempNode);
			if (strlen(tempNode->str) != 0) {
				countEndNodes++;
			}
			if (countEndNodes == index) {
				editNode = tempNode;
				break;
			}
			int len = strlen(tempNode->str);
			editNodeCursor += sizeof(int) * 4 + len;
		}
		//Изменение значений элемента
		int oldStrLen = strlen(editNode->str);
		int nodeSize = sizeof(int) * 4 + oldStrLen;
		editNode->str = str;
		seekg(0, ios::end);
		streampos fileSize = tellg();
		seekg(0, std::ios::beg);
		char* buffer = new char[fileSize];
		read(buffer, fileSize);
		//Создание нового бинарного файла
		char tempName[6] = "1.bin";
		ofstream tempFile(tempName, fstream::binary | fstream::out);
		tempFile.write(buffer, editNodeCursor);
		//Запись отредактируемого элемента
		tempFile.write((char*)&editNode->countNodes, sizeof(int));
		tempFile.write((char*)&editNode->left, sizeof(int));
		tempFile.write((char*)&editNode->right, sizeof(int));
		int len = strlen(editNode->str);
		tempFile.write((char*)&len, sizeof(int));
		tempFile.write(editNode->str, len);
		tempFile.write(buffer + editNodeCursor + nodeSize, fileSize - static_cast<fstream::pos_type>(editNodeCursor + nodeSize));
		//Очистка памяти
		delete[] buffer;
		//Закрытие файлов
		tempFile.close();
		close();
		//Удаление старого бинарного файла
		remove(nameFile);
		//Изменение имени нового бинарного файла
		rename(tempName, nameFile);
		//Открытие нового бинарного файла
		open(nameFile, fstream::binary | fstream::in | fstream::out);
	}
	else {
		cout << "Ошибка: данный элемент не существует" << endl;
	}
}
//Вывод файла в консоль
void File::Print(int cursor, int level)
{
	Node* node = new Node;
	readNode(cursor, node);
	if (pointHead != 0) {
		if (node->right != 0 || node->left != 0 || strlen(node->str) != 0) {
			//Проверка на наличие правого потомка
			if (node->right != 0) {
				Node* left = new Node;
				int cursorLeft = GetCursorNode(cursor, 1);
				readNode(cursorLeft, left);
				int cursorRight = GetCursorNode(cursor, left->countNodes + 2);
				Print(cursorRight, level + 1);
			}
			//Отступы для вывода
			for (int i = 0; i < level; i++)
			{
				printf("%*c", 5, ' ');
			}
			//Вывод строки или промежуточной вершины
			if (strlen(node->str) != 0)
				cout << node->str << endl;
			else
				cout << "Вершина" << endl;
			//Проверка на наличие левого потомка
			if (node->left != 0) {
				int cursorLeft = GetCursorNode(cursor, 1);
				Print(cursorLeft, level + 1);
			}
		}
	}
}
//Поиск элемента по строке
bool File::Search(char* str)
{
	int cursor = 8;
	//Перебор элементов и сравнение со строкой
	for (int i = 0; i < count; i++) {
		Node* tempNode = new Node;
		readNode(cursor, tempNode);
		if (strlen(tempNode->str) != 0 && strcmp(tempNode->str, str)) {
			return true;
		}
		int len = strlen(tempNode->str);
		cursor += sizeof(int) * 4 + len;
	}
	return false;
}
//Получение количества элементов в дереве
int File::GetCount()
{
	//Чтение количества из файла
	int count = 0;
	seekg(4, ios_base::beg);
	read((char*)&count, sizeof(int));
	return CountEndPoint(count);
}
//Записать в текстовый файл
void File::WriteTextFile(char* nameFile)
{
	//Создание текстового файла
	ofstream f(nameFile, ios::out);
	//Запись в файл количества конечных вершин
	f << pointHead << endl;
	f << count;
	if (count != 0)
		f << endl;
	int cursor = 8;
	//Поэлементная запись в файл
	for (int i = 0; i < count; ++i) {
		Node* node = new Node;
		readNode(cursor, node);
		f << node;
		cursor += sizeof(int) * 4 + strlen(node->str);
	}
	//Закрытие текстового файла
	f.close();
}
//Считать из текстового файла
void File::ReadTextFile(char* nameText)
{
	//Очистка двоичного файла
	Clear();
	//Открытие текстового файла
	ifstream f(nameText, ios::in);
	if (f.is_open())
	{
		f >> pointHead;
		f >> count;
		seekg(0, ios::beg);
		//Запись корневой вершины
		write((char*)&pointHead, sizeof(int));
		//Запись количества элементов
		write((char*)&count, sizeof(int));
		Node* node = new Node;
		int cursor = 8;
		//Запись элементов из текстового файла
		for (int i = 0; i < count; i++) {
			int len = 0;
			f >> node->countNodes >> node->left >> node->right >> len;
			seekp(cursor, ios_base::beg);
			write((char*)&node->countNodes, sizeof(int));
			write((char*)&node->left, sizeof(int));
			write((char*)&node->right, sizeof(int));
			write((char*)&len, sizeof(int));
			if (len == 0) {
				write("", len);
			}
			else {
				node->str = new char[len + 1];
				f >> node->str;
				node->str[len] = '\n';
				write(node->str, len);
			}
			close();
			open(nameFile, fstream::binary | fstream::in | fstream::out);
			cursor += sizeof(int) * 4 + len;
		}
	}
	else
	{
		cout << "Файл не удалось открыть";
	}
	//Закрытие текстового файла
	f.close();
}
//Сортировка строк конечных вершин
void File::SortStr()
{
	int cursori = 8;
	//Перебор элементов
	for (int i = 0; i < count; i++)
	{
		Node* iNode = new Node;
		readNode(cursori, iNode);
		int leni = strlen(iNode->str);
		//Проверка на наличие строки
		if (leni != 0)
		{
			int cursorj = cursori + sizeof(int) * 4 + leni;
			//Перебор следующих элементов
			for (int j = i + 1; j < count; j++)
			{
				Node* jNode = new Node;
				readNode(cursorj, jNode);
				int lenj = strlen(jNode->str);
				//Сравнение строк
				if (lenj != 0 && strcmp(iNode->str, jNode->str) < 0)
				{
					seekg(0, ios::end);
					streampos fileSize = tellg();
					seekg(0, std::ios::beg);
					char* buffer = new char[fileSize];
					read(buffer, fileSize);
					//Создание нового файла
					char tempName[6] = "1.bin";
					ofstream tempFile(tempName, fstream::binary | fstream::out);
					tempFile.write(buffer, cursori);
					//Меняем элементы местами и записывем их в новый файл
					tempFile.write((char*)&jNode->countNodes, sizeof(int));
					tempFile.write((char*)&jNode->left, sizeof(int));
					tempFile.write((char*)&jNode->right, sizeof(int));
					tempFile.write((char*)&lenj, sizeof(int));
					tempFile.write(jNode->str, lenj);
					int sdvig = cursori + sizeof(int) * 4 + leni;
					tempFile.write(buffer + sdvig, cursorj - sdvig);
					tempFile.write((char*)&iNode->countNodes, sizeof(int));
					tempFile.write((char*)&iNode->left, sizeof(int));
					tempFile.write((char*)&iNode->right, sizeof(int));
					tempFile.write((char*)&leni, sizeof(int));
					tempFile.write(iNode->str, leni);
					sdvig = cursorj + sizeof(int) * 4 + lenj;
					tempFile.write(buffer + sdvig, fileSize - static_cast<fstream::pos_type>(sdvig));
					delete[] buffer;
					//Закрытие файлов
					tempFile.close();
					close();
					//Удаление старого бинарного файла
					remove(nameFile);
					//Изменение имени нового бинарного файла
					rename(tempName, nameFile);
					//Открытие нового бинарного файла
					open(nameFile, fstream::binary | fstream::in | fstream::out);
					//Поменять значения местами
					char* temps = iNode->str;
					iNode->str = jNode->str;
					jNode->str = temps;
					cursorj -= leni;
					cursorj += lenj;
					int templ = lenj;
					lenj = leni;
					leni = templ;
				}
				cursorj += sizeof(int) * 4 + lenj;
			}
		}
		cursori += sizeof(int) * 4 + leni;
	}
}
//Очистка файла
void File::Clear()
{
	//Закрытие файла
	close();
	//Удаление файла
	remove(nameFile);
	//Пересоздание файла
	ofstream f(nameFile, fstream::out);
	//Закрытие файла
	f.close();
	//Открытие файла с аттрибутами
	open(nameFile, fstream::in | fstream::out | fstream::binary);
	//Запись корневой вершины
	seekp(0, ios_base::beg);
	pointHead = 0;
	count = 0;
	//Запись корневой вершины
	write((char*)&pointHead, sizeof(int));
	//Запись количества элементов
	write((char*)&count, sizeof(int));
	close();
	open(nameFile, fstream::in | fstream::out | fstream::binary);
}