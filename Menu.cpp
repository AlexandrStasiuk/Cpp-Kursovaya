#include "Menu.h"
#include <iostream>
#include <string>
#include <ctime>
#include "File.h"

using namespace std;
//Список опций
void Menu::Header()
{
    cout << "1 Добавить элемент" << endl;
    cout << "2 Удалить элемент" << endl;
    cout << "3 Вывод файла в консоль" << endl;
    cout << "4 Редактировать элемент" << endl;
    cout << "5 Поиск значения по строке" << endl;
    cout << "6 Количество конечных вершин" << endl;
    cout << "7 Сохранить в текстовый файл" << endl;
    cout << "8 Считать данные из текстового файла" << endl;
    cout << "9 Генерация файла" << endl;
    cout << "10 Сортировка строк в дереве" << endl;
    cout << "11 Очистка файл" << endl;
    cout << "12 Закончить" << endl;
}
//Создание строки для добавления
char* Menu::CreateString()
{
    //Ввод строки
    cout << "Введите значение\n";
    char* str = new char[25];
    cin >> str;
    return str;
}
//Создание числа для функций
int Menu::CreateNumber()
{
    //Ввод числа
    cout << "Введите номер конечной вершины\n";
    int number;
    cin >> number;
    return number;
}
//Генерация случайной строки
char* Menu::GenerateString(int len)
{
    //Символы для генерации
    static const char alphanum[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    int size = sizeof(alphanum) - 1;
    //Выделение памяти
    char* str = new char[len + 1];
    //Посимвольная генерация строки
    for (int i = 0; i < len; ++i) {
        str[i] = alphanum[rand() % size];
    }
    str[len] = '\0';
    return str;
}
//Генерация файла из случайных строк
void Menu::GenerateFile(int count, File* f)
{
    for (int i = 0; i < count; i++)
    {
        //Создание сгенерированной строки
        char* str = GenerateString(rand() % 15 + 1);
        //Вставка строки в файл
        f->Insert(str);
    }
}

//Вызов меню
void Menu::Call()
{
    //Номер опции
    int choice;
    //Создание двоичного файла
    File* f = new File(binaryFile);
    while (true)
    {
        //Очистка терминала
        system("cls");
        //Вызов опций
        Header();
        cin >> choice;
        cout << endl;

        if (choice == 12)
            break;
        //Кейсы для каждой опции
        switch (choice)
        {
        case 1:
        {
            //Создание строки для добавления
            char* str = CreateString();
            //Начальное время
            clock_t start_time = clock();
            //Вставка элемента
            f->Insert(str);
            //Конечное время
            clock_t end_time = clock();
            //Время работы
            clock_t lastTime = (double)(end_time - start_time);
            cout << "Время работы - " << lastTime << "мс\n";
            break;
        }
        case 2:
        {
            //Начальное время
            clock_t start_time = clock();
            //Удаление элемента
            f->DeleteLast();
            //Конечное время
            clock_t end_time = clock();
            //Время работы
            clock_t lastTime = (double)(end_time - start_time);
            cout << "Время работы - " << lastTime << "мс\n";
            break;
        }
        case 3:
        {
            //Начальное время
            clock_t start_time = clock();
            //Вывод файла в консоль
            f->Print(8, 0);
            //Конечное время
            clock_t end_time = clock();
            //Время работы
            clock_t lastTime = (double)(end_time - start_time);
            cout << "Время работы - " << lastTime << "мс\n";
            break;
        }
        case 4:
        {
            //Создание числа для поиска
            int index = CreateNumber();
            //Создание строки для поиска
            char* str = CreateString();
            //Начальное время
            clock_t start_time = clock();
            //Редактирование элемента
            f->Edit(index, str);
            //Конечное время
            clock_t end_time = clock();
            //Время работы
            clock_t lastTime = (double)(end_time - start_time);
            cout << "Время работы - " << lastTime << "мс\n";
            break;
        }
        case 5:
        {
            //Создание строки для поиска
            char* str = CreateString();
            //Начальное время
            clock_t start_time = clock();
            //Поиск элемента по строке
            bool isHave = f->Search(str);
            if (isHave)
            {
                cout << "Такое значение есть\n";
            }
            else
            {
                cout << "Такого значения нет\n";
            }
            //Конечное время
            clock_t end_time = clock();
            //Время работы
            clock_t lastTime = (double)(end_time - start_time);
            cout << "Время работы - " << lastTime << "мс\n";
            break;
        }
        case 6:
        {
            //Начальное время
            clock_t start_time = clock();
            //Количество элементов
            cout << "Количество - " << f->GetCount() << endl;
            //Конечное время
            clock_t end_time = clock();
            //Время работы
            clock_t lastTime = (double)(end_time - start_time);
            cout << "Время работы - " << lastTime << "мс\n";
            break;
        }
        case 7:
        {
            //Начальное время
            clock_t start_time = clock();
            //Запись в текстовый файл
            f->WriteTextFile(txtFile);
            //Конечное время
            clock_t end_time = clock();
            //Время работы
            clock_t lastTime = (double)(end_time - start_time);
            cout << "Время работы - " << lastTime << "мс\n";
            break;
        }
        case 8:
        {
            //Начальное время
            clock_t start_time = clock();
            //Чтение из текстового файла
            f->ReadTextFile(txtFile);
            //Конечное время
            clock_t end_time = clock();
            //Время работы
            clock_t lastTime = (double)(end_time - start_time);
            cout << "Время работы - " << lastTime << "мс\n";
            break;
        }
        case 9:
        {
            int count = 0;
            cout << "Введите количество элементов ";
            cin >> count;
            //Начальное время
            clock_t start_time = clock();
            //Очистка файла
            f->Clear();
            //Генерация файла
            GenerateFile(count, f);
            //Конечное время
            clock_t end_time = clock();
            //Время работы
            clock_t lastTime = (double)(end_time - start_time);
            cout << "Время работы - " << lastTime << "мс\n";
            break;
        }
        case 10:
        {
            //Начальное время
            clock_t start_time = clock();
            //Сортировка строк
            f->SortStr();
            //Конечное время
            clock_t end_time = clock();
            //Время работы
            clock_t lastTime = (double)(end_time - start_time);

            cout << "Время работы - " << lastTime << "мс\n";
            break;
        }
        case 11:
        {
            //Начальное время
            clock_t start_time = clock();
            //Очистка файла
            f->Clear();
            //Конечное время
            clock_t end_time = clock();
            //Время работы
            clock_t lastTime = (double)(end_time - start_time);

            cout << "Время работы - " << lastTime << "мс\n";
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