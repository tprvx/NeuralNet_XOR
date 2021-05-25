#ifndef NETH
#define NETH

#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>

using namespace std;

class net
{

private:

    vector<double> x;                   //Входной слой
    double out;                         //Выходной слой
    double error = 0;                   //Ошибка сети
    double averageError;                //Средняя ошибка за эпоху

    vector<vector<unsigned>> inputs = {{0,0},{0,1},{1,0},{1,1}}; //Коллекция входных данных
    double desiredResult;                                        //Ожидаемое значение на выходе сети

    vector<vector<double>> a;           //Состояния нейронов слой:номер
    vector<vector<vector<double>>> w;   //Матрица весов слой:нейрон:номер
    vector<vector<vector<double>>> dw;   //Матрица поправок весов слой:нейрон:номер
    vector<vector<double>> delta;        //Ошибка нейрона

    vector<double> bias;                //Состояние нейронов смещения
    vector<vector<double>> bw;          //Веса нейронов смещения
    vector<vector<double>> dbw;          //Проправки весов нейронов смещения


    double learningRate;        //Коэффициент обучения       Меньше 0,3 ставить
    double limit;              //Точность обучения          0,001
    unsigned epoch = 0;                //Кол-во эпох

public:
    net(double _limit = 0.01, double _learningRate = 0.01);

    void initSet(unsigned variant);      //смена входных и выходных данных
    double feedForward();                //функция прямого распространения сигнала, решение задачи
    double actFunc(double value);        //активационная функция
    double backPropagation();            //функция вычисления попровок весов
    void trainNetwork();                 //функция тренировки сети
    double actFuncDiv(double value);     //производная функции активации
    void printInfo();                                       //Вывод результата
    double getDesireResult() { return desiredResult; }
    double getOutput() { return out;}
};


#endif // NETH
