#include <iostream>
#include <fstream>
#include "File.h"
#include <string>
#include <cstdio>

using namespace std;
//����������� ��� �������� �����
File::File(char* nameFile)
{
	//������ ����� �����
	this->nameFile = nameFile;
	//�������� ����� � �����������
	open(nameFile, fstream::binary | fstream::in | fstream::out);
	if (!is_open())
	{
		//���� ���� �� ��������, �� ���������� �������
		ofstream f(nameFile, fstream::binary);
		f.close();
		//��������� �������� ����� � �����������
		open(nameFile, fstream::binary | fstream::in | fstream::out);
		//������� ������ � ������ �����
		seekp(0, ios_base::beg);
		//������ �������� �������
		write((char*)&pointHead, sizeof(int));
		//������ ���������� ���������
		write((char*)&count, sizeof(int));
	}
	//������ �������� �������
	seekg(0, ios_base::beg);
	read((char*)&pointHead, sizeof(int));
	//������ ���������� �������� ������
	seekg(4, ios_base::beg);
	read((char*)&count, sizeof(int));
}
//���������� ��� �������� �����
File::~File()
{
	//�������� �����
	close();
}
//������ � ������ �������� � ����
void File::readNode(int cursor, Node* node)
{
	//�������� ��������� �� ���������
	seekg(cursor, ios_base::beg);
	//������ ������ � ������ � ����
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
//������ �������� �� ����
void File::writeNode(int cursor, Node* node)
{
	//�������� ��������� �� ���������
	seekp(cursor, ios_base::beg);
	//������ ������ �� ���� � ����
	write((char*)&node->countNodes, sizeof(int));
	write((char*)&node->left, sizeof(int));
	write((char*)&node->right, sizeof(int));
	int len = strlen(node->str);
	write((char*)&len, sizeof(int));
	write(node->str, len);
	close();
	open(nameFile, fstream::binary | fstream::in | fstream::out);
}
//������� ���� � ����
void File::insertFileNode(int cursor, Node* node) {
	//��������� ������� ����� � ������
	seekg(0, ios::end);
	streampos fileSize = tellg();
	seekg(0, ios::beg);
	char* buffer = new char[fileSize];
	//������ ������ � ������
	read(buffer, fileSize);
	//�������� ������ �����
	char tempName[6] = "1.bin";
	ofstream tempFile(tempName, fstream::binary | fstream::out);
	//������ ������ �� ������ ��������
	tempFile.write(buffer, cursor);
	//������ ������ ��������
	tempFile.write((char*)&node->countNodes, sizeof(int));
	tempFile.write((char*)&node->left, sizeof(int));
	tempFile.write((char*)&node->right, sizeof(int));
	int len = strlen(node->str);
	tempFile.write((char*)&len, sizeof(int));
	tempFile.write(node->str, len);
	//������ ������ ����� ������ ��������
	tempFile.write(buffer + cursor, fileSize - static_cast<fstream::pos_type>(cursor));
	//������� ���������� ������
	delete[] buffer;
	//�������� ������
	tempFile.close();
	close();
	//�������� ������� ��������� �����
	remove(nameFile);
	//��������� ����� ������ ��������� �����
	rename(tempName, nameFile);
	//�������� ������ ��������� �����
	open(nameFile, fstream::binary | fstream::in | fstream::out);
}
//�������� ������ �� ������������� �������
void File::DeleteNodeStr(int cursor, int strLenth) {
	//��������� ������� ����� � ������
	seekg(0, ios::end);
	streampos fileSize = tellg();
	seekg(0, std::ios::beg);
	char* buffer = new char[fileSize];
	//������ ������ � ������
	read(buffer, fileSize);
	//�������� ������ �����
	char tempName[6] = "1.bin";
	ofstream tempFile(tempName, fstream::binary | fstream::out);
	//������ ������ �� ��������� ������
	tempFile.write(buffer, cursor);
	//������ ������ ����� ��������� ������
	tempFile.write(buffer + cursor + strLenth, fileSize - static_cast<fstream::pos_type>(cursor + strLenth));
	//������� ���������� ������
	delete[] buffer;
	//�������� ������
	tempFile.close();
	close();
	//�������� ������� ��������� �����
	remove(nameFile);
	//��������� ����� ������ ��������� �����
	rename(tempName, nameFile);
	//�������� ������ ��������� �����
	open(nameFile, fstream::binary | fstream::in | fstream::out);
}
//��������� ��������� ���������������� �� ������ ����
int File::GetCursorNode(int beginCursor, int countNodes) {
	//������� ���������
	int countFileStep = beginCursor;
	//������� ��������� �� ���������
	for (int i = 0; i < countNodes; i++) {
		countFileStep += sizeof(int) * 3;
		seekg(countFileStep, ios_base::beg);
		int len = 0;
		read((char*)&len, sizeof(int));
		countFileStep += sizeof(int) + len;
	}
	return countFileStep;
}
//��������� ��������� �� ���� �� ������ ��������
int File::GetNode(int number) {
	int stepNodes = 8;
	string step = "";
	//������� ����� ��� �������� � ������� ��������
	while (number > 1)
	{
		if (number % 2 == 0)
			step += "1";
		else
			step += "2";
		number /= 2;
	}
	//������� � ������� ����
	for (int i = step.length() - 1; i >= 0; --i)
	{
		//���� ��� ����� 1, �� ������� ���� ���������� �����
		if (step[i] == '1')
		{
			stepNodes = GetCursorNode(stepNodes, 1);
		}
		else
		{
			//����� ������
			Node* left = new Node;
			int cursorLeft = GetCursorNode(stepNodes, 1);
			readNode(cursorLeft, left);
			stepNodes = GetCursorNode(stepNodes, left->countNodes + 2);
		}
	}
	return stepNodes;
}
//��������� ���������� ��������
void File::ChangeCountNodes(int number, int value)
{
	Node* node = new Node;
	readNode(8, node);
	int fileStep = 0;
	int countFileStep = 8;
	//���������� ��� ������ ������� ����
	string step = "";
	//���������� �����, ������� ���������� ��� �������� � ����
	while (number > 1)
	{
		if (number % 2 == 0)
			step += "1";
		else
			step += "2";

		number /= 2;
	}
	//������� � ������� ����
	for (int i = step.length() - 1; i >= 0; --i)
	{
		if (node == nullptr)
			break;
		//��������� ���������� �������� �� ��������� ��������
		node->countNodes += value;
		seekp(countFileStep, ios_base::beg);
		write((char*)&node->countNodes, sizeof(int));
		//���� ��� ����� 1, �� ������� ���� ���������� �����
		if (step[i] == '1')
		{
			countFileStep = GetCursorNode(countFileStep, 1);
		}
		else
		{
			//����� ������
			Node* left = new Node;
			int cursorLeft = GetCursorNode(countFileStep, 1);
			readNode(cursorLeft, left);
			countFileStep = GetCursorNode(countFileStep, left->countNodes + 2);
		}
		//������ ���������� ����
		readNode(countFileStep, node);
	}
}
//��������� ���������� ����
int File::GetFreeNode(Node* node, int cursor)
{
	//������� ����
	Node* current = node;
	int currentCursor = cursor;
	//�������� �� ������ ��������
	if (current->left == 0 || current->right == 0)
		return cursor;
	int stepLeft = GetCursorNode(cursor, 1);
	Node* currentLeft = new Node;
	readNode(stepLeft, currentLeft);
	Node* currentRight = new Node;
	int stepRight = GetCursorNode(cursor, currentLeft->countNodes + 2);
	readNode(stepRight, currentRight);
	//��������� ���������� �������� � ����� ����� ����� � ������ �����
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
//������ ���������� �������� ������
int File::CountEndPoint(int number) {
	//������ ���������� �������� ������
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
//������� ��������
void File::Insert(char* str)
{
	//������� � ������ ����
	if (pointHead == 0)
	{
		firstNode = new Node;
		firstNode->str = str;
		count++;
		//���������� �������� �������
		seekp(0, ios_base::beg);
		pointHead = 4;
		write((char*)&pointHead, sizeof(int));
		//������ ���������� �������� ������
		seekp(4, ios_base::beg);
		write((char*)&count, sizeof(int));
		//������ ������ � �� �����
		writeNode(8, firstNode);
	}
	else {
		firstNode = new Node;
		readNode(8, firstNode);
		//��������� ���������� ����
		int freeNodeCursor = GetFreeNode(firstNode, 8);
		Node* freeNode = new Node;
		readNode(freeNodeCursor, freeNode);
		//�������� ������ ���� � ������������ ���������
		Node* newNode = new Node;
		newNode->str = str;
		if (freeNode->left == 0)
		{
			//�������� ������ ����
			Node* leftNode = new Node;
			//���������� � ���� ��������
			leftNode->str = freeNode->str;
			//������� ������
			freeNode->str = nullptr;
			//�������� ������ ���� � ��������� ���� 
			freeNode->left = 1;
			seekp(freeNodeCursor + sizeof(int), ios_base::beg);
			write((char*)&freeNode->left, sizeof(int));
			int len = 0;
			seekp(freeNodeCursor + sizeof(int) * 3, ios_base::beg);
			write((char*)&len, sizeof(int));
			//�������� ������ �� ������������� �������
			DeleteNodeStr(freeNodeCursor + sizeof(int) * 4, strlen(leftNode->str));
			int insertCursor = freeNodeCursor + sizeof(int) * 4 + len;
			//������� �������� � ����
			insertFileNode(insertCursor, leftNode);
			//������ ������ ���� � ������ ���� ����������
			freeNode->right = 1;
			count += 2;
			seekp(freeNodeCursor + sizeof(int) * 2, ios_base::beg);
			write((char*)&freeNode->right, sizeof(int));
			insertCursor += sizeof(int) * 4 + strlen(leftNode->str);
			//������� �������� � ����
			insertFileNode(insertCursor, newNode);
			//��������� ���������� ��������
			ChangeCountNodes(count, 2);
		}
		else
		{
			//���������� ������ �������� � ������ ����
			freeNode->right = 1;
			count++;
			seekp(freeNodeCursor + sizeof(int) * 2, ios_base::beg);
			write((char*)&freeNode->right, sizeof(int));
			int insertCursor = GetCursorNode(freeNodeCursor, freeNode->countNodes + 1);
			//������� �������� � ����
			insertFileNode(insertCursor, newNode);
			//��������� ���������� ��������
			ChangeCountNodes(count, 1);
		}
		seekp(4, ios_base::beg);
		//������ ���������� �������� ������
		write((char*)&count, sizeof(int));
	}
}
//�������� ��������
void File::DeleteLast()
{
	if (count == 0) {
		cout << "���� �� �������� ���������" << endl;
		return;
	}
	//���� � ������ ������ 1 �������, �� �������� �������� �������
	if (count == 1)
	{
		//���������� �������� �������
		char tempName[6] = "1.bin";
		//�������� ������ ����� ��� ���������
		ofstream tempFile(tempName, fstream::binary | fstream::out);
		tempFile.seekp(0, ios_base::beg);
		pointHead = 0;
		tempFile.write((char*)&pointHead, sizeof(int));
		count = 0;
		tempFile.write((char*)&count, sizeof(int));
		//�������� ������
		tempFile.close();
		close();
		//�������� ������� ��������� �����
		remove(nameFile);
		//��������� ����� ������ ��������� �����
		rename(tempName, nameFile);
		//�������� ������ ��������� �����
		open(nameFile, fstream::binary | fstream::in | fstream::out);
	}
	else {
		//��������� ������������ ������� � ���������
		Node* parentLastNode = new Node;
		int parentLastNodeCursor = GetNode(count / 2);
		readNode(parentLastNodeCursor, parentLastNode);
		//��������� ���������� ��������
		ChangeCountNodes(count / 2, -2);
		//���� ����� ���������� ��������� ������, �� ��������� �����, ����� ������
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
		//�������� ������ ��������� �����
		seekg(0, ios::end);
		streampos fileSize = tellg();
		seekg(0, std::ios::beg);
		char* buffer = new char[fileSize];
		read(buffer, fileSize);
		char tempName[6] = "1.bin";
		ofstream tempFile(tempName, fstream::binary | fstream::out);
		//������ �� ���������� ��������
		tempFile.write(buffer, parentLastNodeCursor);
		tempFile.write((char*)&parentLastNode->countNodes, sizeof(int));
		tempFile.write((char*)&parentLastNode->left, sizeof(int));
		tempFile.write((char*)&parentLastNode->right, sizeof(int));
		int len = strlen(parentLastNode->str);
		tempFile.write((char*)&len, sizeof(int));
		tempFile.write(parentLastNode->str, len);
		//������ ����� ���������� ��������
		tempFile.write(buffer + deletedNode, fileSize - static_cast<fstream::pos_type>(deletedNode));
		//������� ������
		delete[] buffer;
		//�������� ������
		tempFile.close();
		close();
		//�������� ������� ��������� �����
		remove(nameFile);
		//��������� ����� ������ ��������� �����
		rename(tempName, nameFile);
		//�������� ������ ��������� �����
		open(nameFile, fstream::binary | fstream::in | fstream::out);
	}
}
//�������������� ��������
void File::Edit(int index, char* str)
{
	//�������� �� ������� ���������
	if (count == 0) {
		cout << "���� �� �������� ���������" << endl;
		return;
	}
	//�������� �� ������������� ��������
	if (index < CountEndPoint(count) && index >= 0) {

		Node* editNode = new Node;
		int editNodeCursor = 8;
		int countEndNodes = -1;
		//������� ��������� � ����� �������
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
		//��������� �������� ��������
		int oldStrLen = strlen(editNode->str);
		int nodeSize = sizeof(int) * 4 + oldStrLen;
		editNode->str = str;
		seekg(0, ios::end);
		streampos fileSize = tellg();
		seekg(0, std::ios::beg);
		char* buffer = new char[fileSize];
		read(buffer, fileSize);
		//�������� ������ ��������� �����
		char tempName[6] = "1.bin";
		ofstream tempFile(tempName, fstream::binary | fstream::out);
		tempFile.write(buffer, editNodeCursor);
		//������ ���������������� ��������
		tempFile.write((char*)&editNode->countNodes, sizeof(int));
		tempFile.write((char*)&editNode->left, sizeof(int));
		tempFile.write((char*)&editNode->right, sizeof(int));
		int len = strlen(editNode->str);
		tempFile.write((char*)&len, sizeof(int));
		tempFile.write(editNode->str, len);
		tempFile.write(buffer + editNodeCursor + nodeSize, fileSize - static_cast<fstream::pos_type>(editNodeCursor + nodeSize));
		//������� ������
		delete[] buffer;
		//�������� ������
		tempFile.close();
		close();
		//�������� ������� ��������� �����
		remove(nameFile);
		//��������� ����� ������ ��������� �����
		rename(tempName, nameFile);
		//�������� ������ ��������� �����
		open(nameFile, fstream::binary | fstream::in | fstream::out);
	}
	else {
		cout << "������: ������ ������� �� ����������" << endl;
	}
}
//����� ����� � �������
void File::Print(int cursor, int level)
{
	Node* node = new Node;
	readNode(cursor, node);
	if (pointHead != 0) {
		if (node->right != 0 || node->left != 0 || strlen(node->str) != 0) {
			//�������� �� ������� ������� �������
			if (node->right != 0) {
				Node* left = new Node;
				int cursorLeft = GetCursorNode(cursor, 1);
				readNode(cursorLeft, left);
				int cursorRight = GetCursorNode(cursor, left->countNodes + 2);
				Print(cursorRight, level + 1);
			}
			//������� ��� ������
			for (int i = 0; i < level; i++)
			{
				printf("%*c", 5, ' ');
			}
			//����� ������ ��� ������������� �������
			if (strlen(node->str) != 0)
				cout << node->str << endl;
			else
				cout << "�������" << endl;
			//�������� �� ������� ������ �������
			if (node->left != 0) {
				int cursorLeft = GetCursorNode(cursor, 1);
				Print(cursorLeft, level + 1);
			}
		}
	}
}
//����� �������� �� ������
bool File::Search(char* str)
{
	int cursor = 8;
	//������� ��������� � ��������� �� �������
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
//��������� ���������� ��������� � ������
int File::GetCount()
{
	//������ ���������� �� �����
	int count = 0;
	seekg(4, ios_base::beg);
	read((char*)&count, sizeof(int));
	return CountEndPoint(count);
}
//�������� � ��������� ����
void File::WriteTextFile(char* nameFile)
{
	//�������� ���������� �����
	ofstream f(nameFile, ios::out);
	//������ � ���� ���������� �������� ������
	f << pointHead << endl;
	f << count;
	if (count != 0)
		f << endl;
	int cursor = 8;
	//������������ ������ � ����
	for (int i = 0; i < count; ++i) {
		Node* node = new Node;
		readNode(cursor, node);
		f << node;
		cursor += sizeof(int) * 4 + strlen(node->str);
	}
	//�������� ���������� �����
	f.close();
}
//������� �� ���������� �����
void File::ReadTextFile(char* nameText)
{
	//������� ��������� �����
	Clear();
	//�������� ���������� �����
	ifstream f(nameText, ios::in);
	if (f.is_open())
	{
		f >> pointHead;
		f >> count;
		seekg(0, ios::beg);
		//������ �������� �������
		write((char*)&pointHead, sizeof(int));
		//������ ���������� ���������
		write((char*)&count, sizeof(int));
		Node* node = new Node;
		int cursor = 8;
		//������ ��������� �� ���������� �����
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
		cout << "���� �� ������� �������";
	}
	//�������� ���������� �����
	f.close();
}
//���������� ����� �������� ������
void File::SortStr()
{
	int cursori = 8;
	//������� ���������
	for (int i = 0; i < count; i++)
	{
		Node* iNode = new Node;
		readNode(cursori, iNode);
		int leni = strlen(iNode->str);
		//�������� �� ������� ������
		if (leni != 0)
		{
			int cursorj = cursori + sizeof(int) * 4 + leni;
			//������� ��������� ���������
			for (int j = i + 1; j < count; j++)
			{
				Node* jNode = new Node;
				readNode(cursorj, jNode);
				int lenj = strlen(jNode->str);
				//��������� �����
				if (lenj != 0 && strcmp(iNode->str, jNode->str) < 0)
				{
					seekg(0, ios::end);
					streampos fileSize = tellg();
					seekg(0, std::ios::beg);
					char* buffer = new char[fileSize];
					read(buffer, fileSize);
					//�������� ������ �����
					char tempName[6] = "1.bin";
					ofstream tempFile(tempName, fstream::binary | fstream::out);
					tempFile.write(buffer, cursori);
					//������ �������� ������� � ��������� �� � ����� ����
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
					//�������� ������
					tempFile.close();
					close();
					//�������� ������� ��������� �����
					remove(nameFile);
					//��������� ����� ������ ��������� �����
					rename(tempName, nameFile);
					//�������� ������ ��������� �����
					open(nameFile, fstream::binary | fstream::in | fstream::out);
					//�������� �������� �������
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
//������� �����
void File::Clear()
{
	//�������� �����
	close();
	//�������� �����
	remove(nameFile);
	//������������ �����
	ofstream f(nameFile, fstream::out);
	//�������� �����
	f.close();
	//�������� ����� � �����������
	open(nameFile, fstream::in | fstream::out | fstream::binary);
	//������ �������� �������
	seekp(0, ios_base::beg);
	pointHead = 0;
	count = 0;
	//������ �������� �������
	write((char*)&pointHead, sizeof(int));
	//������ ���������� ���������
	write((char*)&count, sizeof(int));
	close();
	open(nameFile, fstream::in | fstream::out | fstream::binary);
}