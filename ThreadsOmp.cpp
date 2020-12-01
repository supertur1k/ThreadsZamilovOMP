#include <iostream>
#include <list>
#include <algorithm>
#include "omp.h"

using namespace std;

int setLength1, setLength2, setLength3, setLength4;
int* A1, * A2, * A3, * A4;


// Основной метод вычисления декартового произведения
void DoWork(int begin, int end, list<int*>& multiply)
{
    for (int i = begin; i < end; i++)
    {
        for (int j = 0; j < setLength2; j++)
        {
            for (int k = 0; k < setLength3; k++)
            {
                for (int m = 0; m < setLength4; m++)
                {
                    int* multiplyFoo = new int[4];
                    multiplyFoo[0] = A1[i];
                    multiplyFoo[1] = A2[j];
                    multiplyFoo[2] = A3[k];
                    multiplyFoo[3] = A4[m];
                    multiply.push_back(multiplyFoo);
                }
            }
        }
    }
}

int main()
{
    // Для русского языка
    setlocale(LC_ALL, "ru");
    // Кол-во потоков
    int threadsCounter;
    cout << "Введите кол-во потоков: " << endl;
    cin >> threadsCounter;
    if (threadsCounter < 1)
    {
        cout << "Некорректный ввод." << endl;
        return 0;
    }

    // Ввод мощностей 4 множеств
    cout << "Введите количество элементов первого множества: " << endl;
    cin >> setLength1;
    if (setLength1 < 1)
    {
        cout << "Некорректный ввод." << endl;
        return 0;
    }
    cout << "Введите количество элементов второго множества: " << endl;
    cin >> setLength2;
    if (setLength2 < 1)
    {
        cout << "Некорректный ввод." << endl;
        return 0;
    }
    cout << "Введите количество элементов третьего множества: " << endl;
    cin >> setLength3;
    if (setLength3 < 1)
    {
        cout << "Некорректный ввод." << endl;
        return 0;
    }
    cout << "Введите количество элементов четвертого множества: " << endl;
    cin >> setLength4;
    if (setLength4 < 1)
    {
        cout << "Некорректный ввод." << endl;
        return 0;
    }

    omp_set_num_threads(threadsCounter);

    A1 = new int[setLength1]; 
    for (int i = 0; i < setLength1; i++) {
        A1[i] = i;
    }
    A2 = new int[setLength2];
    for (int i = 0; i < setLength2; i++) {
        A2[i] = i;
    }
    A3 = new int[setLength3];
    for (int i = 0; i < setLength3; i++) {
        A3[i] = i;
    }
    A4 = new int[setLength4];
    for (int i = 0; i < setLength4; i++) {
        A4[i] = i;
    }

    A1[0] = 0;

    if (setLength1 < threadsCounter) threadsCounter = setLength1;
    int period = setLength1 / threadsCounter;

    list<int*>* multiply = new list<int*>[threadsCounter];

    int begin, end;
    begin = 0;

#pragma omp parallel for
    for (int i = 0; i < threadsCounter; i++) {
        if (i != threadsCounter - 1) {
            end = begin + period;
        }
        else {
            end = setLength1;
        }
        multiply[i] = list<int*>();
        DoWork(begin, end, multiply[i]);
        begin = end;
#pragma omp critical 
        {
            cout << "thread " << omp_get_thread_num() << endl;
        }
    }

    // Собираем перемножения распределенные среди первого множества по потокам во едино.
    list<int*> multiplyFinal;
    for (int i = 0; i < threadsCounter; i++)
    {
        multiplyFinal.insert(multiplyFinal.end(), multiply[i].begin(), multiply[i].end());
    }

    for (auto i = multiplyFinal.begin(); i != multiplyFinal.end(); i++)
    {
        cout << (*i)[0] << "\t" << (*i)[1] << "\t" << (*i)[2] << "\t" << (*i)[3] << endl;
    }

    delete[] A1;
    delete[] A2;
    delete[] A3;
    delete[] A4;
    delete[] multiply;
}