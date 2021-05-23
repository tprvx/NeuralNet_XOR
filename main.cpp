#include <iostream>
#include <math.h>
#include <fstream>

using namespace std;

void startInitSet();                //начальная конфигурация, случайные значения весов
void initSet(unsigned variant);     //смена входных и выходных данных
void evalateNeuralNetwork();        //функция прямого распространения сигнала, решение задачи
double actFunc(double value);       //активационная функция
double getNeuralNetError();         //функция вычисления попровок весов
void trainNetwork();                //функция тренировки
double actFuncDiv(double value);    //производная функции активации
void printInfo();

double inputs[4][2] = {{0,0},{0,1},{1,0},{1,1}}; //Коллекция входных данных
double desiredResult = 1;       //Идеальное значение на выходе

double x[2];    //Входные воздействия
double a[3];    //состояние нейронов a
double b[3];    //состояние нейронов b
double bias[3]; //Нейроны смещения
double out = 0; //Выход

double xw[3][3];  //веса скрытого слоя a
double aw[3][3];  //веса скрытого слоя b
double bw[3];    //веса выходного слоя
double biw[3][3];   //веса нейронов смещения

double delta = 0;   //ошибка на выходе
double del_a[3];    //ошибка нейрона а
double del_b[3];    //ошибка нейрона b

double dxw[3][3];    //Дельты весов слоя a
double daw[3][3];    //Дельты весов слоя b
double dbw[3];       //Дельты весов выходного слоя
double dbiw[3][3];       //Дельты весов слоя смещения

double learningRate = 0.1;      //Коэффициент обучения  --0.825--
double limit = 0.0000001;    //Точность обучения
unsigned epoch = 0;     //кол-во эпох


int main()
{
    char isTrain;
    cout << "Тренировать нейросеть? ('y' or 'n')";
    cin >> isTrain;
    if(isTrain == 'y')
    {
        //тренировка
        trainNetwork();
        ofstream out;          // поток для записи
        out.open("coefficents.txt"); // окрываем файл для записи
        if (out.is_open())
        {

        }
        out.close();
    }
/*
    string line;
    ifstream in("coefficents.txt"); // окрываем файл для чтения
    if (in.is_open())
        {
            while (getline(in, line))
            {
                std::cout << line << std::endl;
            }
        }
    in.close();     // закрываем файл
*/
    cout << endl << "Проверка: " << endl;

    auto p = 0.0;


    for (int i = 0; i < 4; i++)
    {
        initSet(i);
        evalateNeuralNetwork();
        printInfo();
        p += abs(desiredResult - out);;
    }
    cout << endl << "Точность: " << 100 - p/4*100 << "%" << endl;

    return 0;
}

void trainNetwork()
{
    startInitSet();
    double err = 1;
    while( abs(err) > limit)
    {
        for(unsigned i = 0; (i < 4); i++)
        {
            initSet(i); //смена обучающей выборки
            evalateNeuralNetwork();
            err = getNeuralNetError();
         }
        epoch++;
        if((epoch % 100) == 0)
        {
            cout << "----------------------- Эпоха " << epoch << " ---------------------------" << endl;
            printInfo();
        }
    }


    cout << endl << "---------------------Обучение завершено!-----------------------" << endl << endl;
    cout << " Точность более " << (1-limit)*100 << "%" << endl;
    cout << " Коэффициент скорости обучения " << learningRate << endl;
    cout << " Потребовалось: "  << epoch << " эпох" << endl;

}

//Смена обуцающих данных
void initSet(unsigned variant)
{
    //Задаем входы
    for(int i = 0; i < 2; i++)
    {
        x[i] = inputs[variant][i];
    }

    //Задаем выход(операция исключающее .или.)
    desiredResult = (double)((bool)x[0] ^ (bool)x[1]);
}

//Начальлная инициализация необученной сети случаными значениями
void startInitSet()
{
    //Задаем входы
    for(int i = 0; i < 2; i++)
    {
        x[i] = inputs[1][i];
    }

    //Задаем выход(операция исключающее .или.)
    desiredResult = (double)((bool)x[0] ^ (bool)x[1]);

    //Нейроны смещения всегда равны 1
    bias[0] = 1;
    bias[1] = 1;
    bias[2] = 1;

    //Придаем случайные значения весам
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            xw[i][j] = (rand() % 1000)/1000.0;
           // cout << "xw" << i << j << " = " << xw[i][j] << endl;
        }
       // cout << endl;
    }
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            aw[i][j] = (rand() % 1000)/1000.0;
            //cout << "aw" << i << j << " = " << aw[i][j] << endl;
        }
        //cout << endl;
    }
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            biw[i][j] = (rand() % 1000)/1000.0;
            //cout << "biw" << i << j << " = " << biw[i][j] << endl;
        }
        //cout << endl;
    }

    for(int j = 0; j < 3; j++)
    {
        bw[j] = (rand() % 1000)/1000.0;
        //cout << "bw" << j << " = " << bw[j] << endl;
    }
}


//Расчет выходного значения
void evalateNeuralNetwork()
{

    a[0] = actFunc(x[0]*xw[0][0] + x[1]*xw[1][0] + bias[0]*biw[0][0]);
    a[1] = actFunc(x[0]*xw[0][1] + x[1]*xw[1][1] + bias[0]*biw[0][1]);
    a[2] = actFunc(x[0]*xw[0][2] + x[1]*xw[1][2] + bias[0]*biw[0][2]);

    b[0] = actFunc(a[0]*aw[0][0] + a[1]*aw[1][0] + bias[1]*biw[1][0]);
    b[1] = actFunc(a[0]*aw[0][1] + a[1]*aw[1][1] + bias[1]*biw[1][1]);
    b[2] = actFunc(a[0]*aw[0][2] + a[1]*aw[1][2] + bias[1]*biw[1][2]);
/*
    cout << endl;
    cout << "x1 = " << x[0] << endl;
    cout << "x2 = " << x[1] << endl;
    cout << "a1 = " << a[0] << endl;
    cout << "a2 = " << a[1] << endl;
    cout << "a3 = " << a[2] << endl;
    cout << "b1 = " << b[0] << endl;
    cout << "b2 = " << b[1] << endl;
    cout << "b3 = " << b[2] << endl;
    cout << "output = " << out << endl;
*/
    //Выход
    out = actFunc(b[0]*bw[0] + b[1]*bw[1] + b[2]*bw[2] + bias[2]*biw[2][1]);

}


//Расчет ошибки
double getNeuralNetError()
{
    //Вычисление ошибки
    delta = desiredResult - out;

    del_b[0] = delta*bw[0];
    del_b[1] = delta*bw[1];
    del_b[2] = delta*bw[2];

    del_a[0] = del_b[0]*aw[0][0] + del_b[1]*aw[0][1] + del_b[2]*aw[0][2];
    del_a[1] = del_b[0]*aw[1][0] + del_b[1]*aw[1][1] + del_b[2]*aw[1][2];
    del_a[1] = del_b[0]*aw[2][0] + del_b[2]*aw[2][1] + del_b[2]*aw[2][2];

/*
    cout << endl << "delta = " << delta << endl;
    cout << "del[0] = " << del[0] << endl;
    cout << "del[1] = " << del[1] << endl << endl;
*/
    //Вычисление корректировок xw
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            dxw[i][j] = learningRate*del_a[j]*actFuncDiv(a[j])*x[i];
            //cout << "dw[i][j]" << " = " << del[j] << "*" << w[i][j] << "*" << actFuncDiv(x[i]) << "*" << learningRate << endl;
            // cout << "dw" << i << j << " = " << dw[i][j] << endl;
        }

    }
    //cout << endl;
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            xw[i][j] += dxw[i][j];
           // cout << "w" << i << j << " = " << w[i][j] << endl;
        }

    }


    //Вычисление корректировок aw
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            daw[i][j] = learningRate*del_b[j]*actFuncDiv(b[j])*a[i];
            //cout << "dw[i][j]" << " = " << del[j] << "*" << w[i][j] << "*" << actFuncDiv(x[i]) << "*" << learningRate << endl;
            // cout << "dw" << i << j << " = " << dw[i][j] << endl;
        }

    }
    //cout << endl;
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            aw[i][j] += daw[i][j];
           // cout << "w" << i << j << " = " << w[i][j] << endl;
        }

    }

    //Вычисление корректировок bw
    for(int i = 0; i < 3; i++)
    {
        dbw[i] = learningRate*delta*actFuncDiv(out)*b[i];
       // cout <<  "dv" << i << " = " << dv[i] << endl;
    }
    //cout << endl;
    for(int i = 0; i < 3; i++)
    {
        bw[i] += dbw[i];
        //cout <<  "v[i]" << i << " = " << v[i] << endl;
    }

    //нейроны смещения


    dbiw[0][0] = learningRate*del_a[0]*actFuncDiv(a[0])*bias[0];
    dbiw[0][1] = learningRate*del_a[1]*actFuncDiv(a[1])*bias[0];
    dbiw[0][2] = learningRate*del_a[2]*actFuncDiv(a[2])*bias[0];

    dbiw[1][0] = learningRate*del_b[0]*actFuncDiv(b[0])*bias[1];
    dbiw[1][1] = learningRate*del_b[1]*actFuncDiv(b[1])*bias[1];
    dbiw[1][2] = learningRate*del_b[2]*actFuncDiv(b[2])*bias[1];

    dbiw[2][0] = learningRate*delta*actFuncDiv(out)*bias[2];


    biw[0][0] += dbiw[0][0];
    biw[0][1] += dbiw[0][1];
    biw[0][2] += dbiw[0][2];

    biw[1][0] += dbiw[1][0];
    biw[1][1] += dbiw[1][1];
    biw[1][2] += dbiw[1][2];

    biw[2][0] += dbiw[2][0];


/*
    cout << "dbw[0] = " << dbw[0] << endl;
    cout << "dbw[1] = " << dbw[1] << endl;
    cout << "dbw[2] = " << dbw[2] << endl << endl;
*/

/*
    cout << "bw[0] = " << bw[0] << endl;
    cout << "bw[1] = " << bw[1] << endl;
    cout << "bw[2] = " << bw[2] << endl;
*/
    return delta;
}

double actFunc(double value)
{
    auto sigmoid = 1/(1 + exp(-value));

    return tanh(value);
}


double actFuncDiv(double value)
{
    return 1/pow(cosh(value),2);
    //return actFunc(value)*(1 - actFunc(value));
}

void printInfo()
{
    cout << endl;
    cout << "x1 = " << x[0] << endl;
    cout << "x2 = " << x[1] << endl;
   /* cout << "a1 = " << a[0] << endl;
    cout << "a2 = " << a[1] << endl;
    cout << "a3 = " << a[2] << endl;
    cout << "b1 = " << b[0] << endl;
    cout << "b2 = " << b[1] << endl;
    cout << "b3 = " << b[2] << endl;*/
    cout << "output = " << out << endl;
}
