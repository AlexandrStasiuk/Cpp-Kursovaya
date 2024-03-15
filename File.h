#pragma once
#include <iostream>
#include <fstream>

using namespace std;

class File : public fstream
{
private:
	//�������� �������
	int pointHead = 0;
	struct Node
	{
		//�������� ������
		char* str;
		//��������� �� ����� � ������ ��������
		int left = 0;
		int right = 0;
		//���������� ��������
		int countNodes = 0;
		//���������� ��� ������� ������
		~Node() {
			delete[] str;
		}
	};
	//��� �����
	char* nameFile = nullptr;
	//������ ����
	Node* firstNode = nullptr;
	//���������� �����
	int count = 0;
	//������ � ������ �������� � ����
	void readNode(int cursor, Node* node);
	//������ �������� � ���� �� ����
	void writeNode(int cursor, Node* node);
	//������� ���� � ����
	void insertFileNode(int cursor, Node* node);
	//�������� ������ �� ������������� �������
	void DeleteNodeStr(int cursor, int strLenth);
	//��������� ��������� ���������������� �� ������ ����
	int GetCursorNode(int beginCursor, int countNodes);
	//��������� ��������� �� ���� �� ������ ��������
	int GetNode(int);
	//��������� ���������� ��������
	void ChangeCountNodes(int number, int value);
	//��������� ���������� ����
	int GetFreeNode(Node*, int);
	//������ ���������� �������� ������
	int CountEndPoint(int);

public:
	//����������� ��� �������� �����
	File(char*);
	//���������� ��� �������� �����
	~File();
	//������� ��������
	void Insert(char*);
	//�������� ��������
	void DeleteLast();
	//�������������� ��������
	void Edit(int, char*);
	//����� ����� � �������
	void Print(int cursor, int level);
	//����� �������� �� ������
	bool Search(char*);
	//��������� ���������� �������� ������
	int GetCount();
	//�������� � ��������� ����
	void WriteTextFile(char*);
	//������� �� ���������� �����
	void ReadTextFile(char*);
	//���������� ����� �������� ������
	void SortStr();
	//������� �����
	void Clear();
	//�������� �������� �����
	friend ofstream& operator<< (ofstream& os, Node* node) {
		os << node->countNodes << " " << node->left << " " << node->right << " " << strlen(node->str) << " " << node->str << endl;
		return os;
	}
};