#pragma once
#include <iostream>
#include <fstream>

using namespace std;

class File : public fstream
{
private:
	//Корневая вершина
	int pointHead = 0;
	struct Node
	{
		//Хранимые данные
		char* str;
		//Указатели на левый и правый потомков
		int left = 0;
		int right = 0;
		//Количество потомков
		int countNodes = 0;
		//Деструктор для очистки памяти
		~Node() {
			delete[] str;
		}
	};
	//Имя файла
	char* nameFile = nullptr;
	//Первый узел
	Node* firstNode = nullptr;
	//Количество узлов
	int count = 0;
	//Чтение и запись элемента в узел
	void readNode(int cursor, Node* node);
	//Запись элемента в файл из узла
	void writeNode(int cursor, Node* node);
	//Вставка узла в файл
	void insertFileNode(int cursor, Node* node);
	//Удаление строки из промежуточной вершины
	void DeleteNodeStr(int cursor, int strLenth);
	//Получение указателя позиционирования по номеру узла
	int GetCursorNode(int beginCursor, int countNodes);
	//Получение указателя на узел по номеру элемента
	int GetNode(int);
	//Изменение количества потомков
	void ChangeCountNodes(int number, int value);
	//Получение свободного узла
	int GetFreeNode(Node*, int);
	//Расчет количества конечных вершин
	int CountEndPoint(int);

public:
	//Конструктор для открытия файла
	File(char*);
	//Деструктор для закрытия файла
	~File();
	//Вставка элемента
	void Insert(char*);
	//Удаление элемента
	void DeleteLast();
	//Редактирование элемента
	void Edit(int, char*);
	//Вывод файла в консоль
	void Print(int cursor, int level);
	//Поиск элемента по строке
	bool Search(char*);
	//Получение количества конечных вершин
	int GetCount();
	//Записать в текстовый файл
	void WriteTextFile(char*);
	//Считать из текстового файла
	void ReadTextFile(char*);
	//Сортировка строк конечных вершин
	void SortStr();
	//Очистка файла
	void Clear();
	//Выходной файловый поток
	friend ofstream& operator<< (ofstream& os, Node* node) {
		os << node->countNodes << " " << node->left << " " << node->right << " " << strlen(node->str) << " " << node->str << endl;
		return os;
	}
};