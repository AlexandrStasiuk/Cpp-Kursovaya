#include "Menu.h"
#include <iostream>
#include <string>
#include <ctime>
#include "File.h"

using namespace std;
//������ �����
void Menu::Header()
{
    cout << "1 �������� �������" << endl;
    cout << "2 ������� �������" << endl;
    cout << "3 ����� ����� � �������" << endl;
    cout << "4 ������������� �������" << endl;
    cout << "5 ����� �������� �� ������" << endl;
    cout << "6 ���������� �������� ������" << endl;
    cout << "7 ��������� � ��������� ����" << endl;
    cout << "8 ������� ������ �� ���������� �����" << endl;
    cout << "9 ��������� �����" << endl;
    cout << "10 ���������� ����� � ������" << endl;
    cout << "11 ������� ����" << endl;
    cout << "12 ���������" << endl;
}
//�������� ������ ��� ����������
char* Menu::CreateString()
{
    //���� ������
    cout << "������� ��������\n";
    char* str = new char[25];
    cin >> str;
    return str;
}
//�������� ����� ��� �������
int Menu::CreateNumber()
{
    //���� �����
    cout << "������� ����� �������� �������\n";
    int number;
    cin >> number;
    return number;
}
//��������� ��������� ������
char* Menu::GenerateString(int len)
{
    //������� ��� ���������
    static const char alphanum[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    int size = sizeof(alphanum) - 1;
    //��������� ������
    char* str = new char[len + 1];
    //������������ ��������� ������
    for (int i = 0; i < len; ++i) {
        str[i] = alphanum[rand() % size];
    }
    str[len] = '\0';
    return str;
}
//��������� ����� �� ��������� �����
void Menu::GenerateFile(int count, File* f)
{
    for (int i = 0; i < count; i++)
    {
        //�������� ��������������� ������
        char* str = GenerateString(rand() % 15 + 1);
        //������� ������ � ����
        f->Insert(str);
    }
}

//����� ����
void Menu::Call()
{
    //����� �����
    int choice;
    //�������� ��������� �����
    File* f = new File(binaryFile);
    while (true)
    {
        //������� ���������
        system("cls");
        //����� �����
        Header();
        cin >> choice;
        cout << endl;

        if (choice == 12)
            break;
        //����� ��� ������ �����
        switch (choice)
        {
        case 1:
        {
            //�������� ������ ��� ����������
            char* str = CreateString();
            //��������� �����
            clock_t start_time = clock();
            //������� ��������
            f->Insert(str);
            //�������� �����
            clock_t end_time = clock();
            //����� ������
            clock_t lastTime = (double)(end_time - start_time);
            cout << "����� ������ - " << lastTime << "��\n";
            break;
        }
        case 2:
        {
            //��������� �����
            clock_t start_time = clock();
            //�������� ��������
            f->DeleteLast();
            //�������� �����
            clock_t end_time = clock();
            //����� ������
            clock_t lastTime = (double)(end_time - start_time);
            cout << "����� ������ - " << lastTime << "��\n";
            break;
        }
        case 3:
        {
            //��������� �����
            clock_t start_time = clock();
            //����� ����� � �������
            f->Print(8, 0);
            //�������� �����
            clock_t end_time = clock();
            //����� ������
            clock_t lastTime = (double)(end_time - start_time);
            cout << "����� ������ - " << lastTime << "��\n";
            break;
        }
        case 4:
        {
            //�������� ����� ��� ������
            int index = CreateNumber();
            //�������� ������ ��� ������
            char* str = CreateString();
            //��������� �����
            clock_t start_time = clock();
            //�������������� ��������
            f->Edit(index, str);
            //�������� �����
            clock_t end_time = clock();
            //����� ������
            clock_t lastTime = (double)(end_time - start_time);
            cout << "����� ������ - " << lastTime << "��\n";
            break;
        }
        case 5:
        {
            //�������� ������ ��� ������
            char* str = CreateString();
            //��������� �����
            clock_t start_time = clock();
            //����� �������� �� ������
            bool isHave = f->Search(str);
            if (isHave)
            {
                cout << "����� �������� ����\n";
            }
            else
            {
                cout << "������ �������� ���\n";
            }
            //�������� �����
            clock_t end_time = clock();
            //����� ������
            clock_t lastTime = (double)(end_time - start_time);
            cout << "����� ������ - " << lastTime << "��\n";
            break;
        }
        case 6:
        {
            //��������� �����
            clock_t start_time = clock();
            //���������� ���������
            cout << "���������� - " << f->GetCount() << endl;
            //�������� �����
            clock_t end_time = clock();
            //����� ������
            clock_t lastTime = (double)(end_time - start_time);
            cout << "����� ������ - " << lastTime << "��\n";
            break;
        }
        case 7:
        {
            //��������� �����
            clock_t start_time = clock();
            //������ � ��������� ����
            f->WriteTextFile(txtFile);
            //�������� �����
            clock_t end_time = clock();
            //����� ������
            clock_t lastTime = (double)(end_time - start_time);
            cout << "����� ������ - " << lastTime << "��\n";
            break;
        }
        case 8:
        {
            //��������� �����
            clock_t start_time = clock();
            //������ �� ���������� �����
            f->ReadTextFile(txtFile);
            //�������� �����
            clock_t end_time = clock();
            //����� ������
            clock_t lastTime = (double)(end_time - start_time);
            cout << "����� ������ - " << lastTime << "��\n";
            break;
        }
        case 9:
        {
            int count = 0;
            cout << "������� ���������� ��������� ";
            cin >> count;
            //��������� �����
            clock_t start_time = clock();
            //������� �����
            f->Clear();
            //��������� �����
            GenerateFile(count, f);
            //�������� �����
            clock_t end_time = clock();
            //����� ������
            clock_t lastTime = (double)(end_time - start_time);
            cout << "����� ������ - " << lastTime << "��\n";
            break;
        }
        case 10:
        {
            //��������� �����
            clock_t start_time = clock();
            //���������� �����
            f->SortStr();
            //�������� �����
            clock_t end_time = clock();
            //����� ������
            clock_t lastTime = (double)(end_time - start_time);

            cout << "����� ������ - " << lastTime << "��\n";
            break;
        }
        case 11:
        {
            //��������� �����
            clock_t start_time = clock();
            //������� �����
            f->Clear();
            //�������� �����
            clock_t end_time = clock();
            //����� ������
            clock_t lastTime = (double)(end_time - start_time);

            cout << "����� ������ - " << lastTime << "��\n";
            break;
        }
        default:
        {
            break;
        }
        }
        system("pause");
    }
}