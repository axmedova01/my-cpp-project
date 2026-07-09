#include <iostream>
#include <ctime>
#include <vector>
#include <fstream>
using namespace std;

const int countCircle = 3; //кол-во эл-ов в массиве geometry класс круг
const int countRectangle = 4; //кол-во эл-ов в массиве geometry класс прямоугольник
const int R = 500; //размеры рамки
const int N = 100;

class figure {
public:
    double* geometry;
    double* speed;
    virtual double move() = 0; //виртуальная ф-ия
    virtual double rotate() = 0;
};

class circle: figure {
public:
    circle() {
        geometry = new double[countCircle];
        geometry[0] = 0;
        geometry[1] = 0;
        geometry[2] = 0;
    }
    circle(double x, double y, double r) {
        geometry = new double[countCircle];
        geometry[0] = x;
        geometry[1] = y;
        geometry[2] = r;
        speed =  new double[countCircle];
    }

    double move() override { return 0; }; //реализация виртуальной ф-ии, чтобы класс наследника не был тоже абстрактным как родительский
    double rotate() override { return 0; };

    bool test()
    {
        if ((geometry[0] + geometry[2] < N*R) && (geometry[0] - geometry[2] > 0) && (geometry[1] + geometry[2] < N*R) && (geometry[1] - geometry[2] > 0))
        {
            return true;
        }
        else return false;
    }

    friend ostream& operator << (ostream& ost, circle& crc); //перегруженная ф-ия оператора << объявляется как друг класса circle, чтобы получить доступ к личным данным в пределах circle объекта

};

class rectangle : figure {
public:
    rectangle() {
        geometry = new double[countRectangle];
        geometry[0] = 0;
        geometry[1] = 0;
        geometry[2] = 0;
        geometry[3] = 0;
    }
    rectangle(double x, double y, double length, double width) {
        geometry = new double[countRectangle];
        geometry[0] = x;
        geometry[1] = y;
        geometry[2] = length; //длина прямоугольника
        geometry[3] = width; //ширина прямоугольника
        speed = new double[countRectangle];
    }

    double move() override { return 0; }; //реализация виртуальных ф-ий родительского класса
    double rotate() override { return 0; };

    bool test()
    {
        if (((geometry[0] - geometry[2] / 2) > 0) && ((geometry[0] + geometry[2] / 2) < N*R)  && ((geometry[0] + geometry[3] / 2) < N*R) && ((geometry[0] - geometry[3] / 2) > 0))
        {
            return true;
        }
        else return false;
    }
    friend ostream& operator << (ostream& ost, rectangle& rct);
};

ostream& operator << (ostream& ost, circle& crc) //перегрузка оператора << для собственных классов
{
    return ost << "x = " << crc.geometry[0] << "; y = " << crc.geometry[1] << "; r = " << crc.geometry[2] << "\n";
}

ostream& operator << (ostream& ost, rectangle& rct)
{
    return ost << "x = " << rct.geometry[0] << "; y = " << rct.geometry[1] << "; length = " << rct.geometry[2] << "; width = " << rct.geometry[3] << "\n";
}

int main() {

    srand(time(NULL));
    vector <circle> circleArr; //вектов типа класс circle
    //нет длины вектора, обычно указываем: circleArr(5) - в этом случае массив автоматически заполняется нулями
    //или circleArr.reserve(5)  - мы выделили память, в которой уже хранится ненужный для ПК мусор ввиде рандомных чисел; + по времени
    vector <rectangle> rectangleArr;

    ofstream out; //ofstream - выходной файловый поток; ifstream - входной; fstream - для ввода и вывода
    out.open("D:\\PROBLEM\\C++\\laba1\\output.txt");

    int K = 20; //кол-во проходов по циклу на проверку

    for (int i = 0; i < K; i++)
    {
        if (rand() % 2 == 1)
        {
            circle circleExample(rand(), rand(), rand()); //тоже самое что обЪявить массив int arr[][][]
            if (circleExample.test())
            {
                circleArr.push_back(circleExample); //push_back - добавление эл-та в вектор
                cout << circleExample; //вывод в консоли
                out << circleExample; //вывод в файл output.txt
            }
        }
        else
        {
            rectangle rectangleExample(rand(), rand(), rand(), rand());
            if (rectangleExample.test())
            {
                rectangleArr.push_back(rectangleExample);
                cout << rectangleExample;
                out << rectangleExample;
            }
        }
        //.size() - кол-во эл-ов; .clear() - удалить все эл-ты вектора; push_back() - добавить ячейку в конец вектора; pop_back() - удалить последний эл-т; .empty() - проверить вектор на пустоту
        //обращение к ячейке: 1)индексы с []; 2).at()
    }
    out.close();
    return 0;
}
