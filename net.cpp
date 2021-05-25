#include "net.h"

void net::trainNetwork()
{
    averageError = 1;
    while( averageError > limit)
    {
        error = 0;
        averageError = 0;
        for(unsigned i = 0; (i < 4); i++)
        {
            initSet(i); //смена обучающей выборки
            feedForward();
            backPropagation();
            averageError += abs(error);

        }
        averageError /= 4;
        epoch++;
        if((epoch % 1000) == 0)
        {
            cout << "----------------------- Эпоха " << epoch << " ---------------------------" << endl;
            printInfo();
        }
    }


    cout << endl << "---------------------Обучение завершено!-----------------------" << endl << endl;
    cout << " Коэффициент скорости обучения " << learningRate << endl;
    cout << " Потребовалось: "  << epoch << " эпох" << endl;

}

//Смена обуцающих данных
net::net(double _limit, double _learningRate)
{
    limit = _limit;
    learningRate = _learningRate;
    //Задаем входы
    for(unsigned i = 0; i < 2; i++)
    {
        x.push_back(inputs[1][i]);
        //cout << x.at(i) << " " << endl;
    }

    //Задаем желаемое значение(операция исключающее .или.)
    this->desiredResult = static_cast<double>(static_cast<bool>(x[0]) ^ static_cast<bool>(x[1]));

    //Нейроны смещения всегда равны 1
    for(unsigned i = 0; i < 3; i++)
    {
        bias.push_back(1);
        //cout << bias.at(i) << " " << endl;
    }

    //Придаем случайные значения весам
    w.resize(3);
    dw.resize(3);
    for(unsigned i = 0; i < 3; i++)
    {
        w[i].resize(3);
        dw[i].resize(3);
        for(unsigned j = 0; j < 3; j++)
        {
            dw[i][j].resize(3);
            for(unsigned k = 0; k < 3; k++)
            {
                w[i][j].push_back((rand() % 1000)/1000.0);
                //cout << w[i][j][k] << " ";
            }
            //cout << endl;
        }
        //cout << endl;
    }

    //Веса нейронов смещения
    bw.resize(3);
    dbw.resize(3);
    a.resize(3);
    delta.resize(3);
    for(unsigned i = 0; i < 3; i++)
    {
        dbw[i].resize(3);
        a[i].resize(3);
        delta[i].resize(3);
        for(unsigned j = 0; j < 3; j++)
        {
            bw[i].push_back((rand() % 1000)/1000.0);
            //cout << bw[i][j] << " ";
        }
        //cout << endl;
    }
}

void net::initSet(unsigned variant)
{
    //Задаем входы
    for(unsigned i = 0; i < 2; i++)
    {
        x[i] = inputs[variant][i];
    }

    //Задаем выход(операция исключающее .или.)
    desiredResult = static_cast<double>(static_cast<bool>(x[0]) ^ static_cast<bool>(x[1]));
}

double net::feedForward()
{

    for(unsigned i = 0; i < 3; i++)
        a[0][i] = actFunc(x[0]*w[0][0][i] + x[1]*w[0][1][i] + bias[0]*bw[0][i]);

    for(unsigned i = 0; i < 3; i++)
        a[1][i] = actFunc(a[0][0]*w[1][0][i] + a[0][1]*w[1][1][i] + a[0][2]*w[1][2][i] + bias[1]*bw[1][i]);

    out = abs(actFunc(a[1][0]*w[2][0][0] + a[1][1]*w[2][1][0] + a[1][2]*w[2][2][0] + bias[2]*bw[2][0]));

    return  out;
}


//Расчет ошибки
double net::backPropagation()
{
    //Вычисление ошибки
    error = desiredResult - out;
    for(unsigned j = 0; j < 3; j++)
    {
            delta[1][j] = error*w[2][j][0];
            delta[0][j] = delta[1][j]*w[1][j][0] +delta[1][j]*w[1][j][1] + delta[1][j]*w[1][j][2];
    }


    //Первый слой
    for(unsigned i = 0; i < 2; i++) //проход по нейронам
    {
        for(unsigned j = 0; j < 3; j++)     //Проход по номеру веса
        {
             dw[0][i][j] = learningRate*delta[0][j]*actFuncDiv(a[0][j])*x[i];
             w[0][i][j] += dw[0][i][j];
        }
    }


    //Второй слой
    for(unsigned i = 0; i < 3; i++) //проход по нейронам
    {
        for(unsigned j = 0; j < 3; j++)     //Проход по номеру веса
        {
             dw[1][i][j] = learningRate*delta[1][j]*actFuncDiv(a[1][j])*a[0][i];
             w[1][i][j] += dw[1][i][j];
        }
    }

    //Выходной слой
    for(unsigned i = 0; i < 3; i++)
    {
        dw[2][i][0] = learningRate*error*actFuncDiv(out)*a[1][i];
        w[2][i][0] += dw[2][i][0];
    }

    //Нейроны смещения
    for(unsigned i = 0; i < 2; i++) //проход по нейронам
    {
        for(unsigned j = 0; j < 3; j++)     //Проход по номеру веса
        {
             dbw[i][j] = learningRate*delta[i][j]*actFuncDiv(a[i][j])*bias[i];
             bw[i][j] += dbw[i][j];
        }
    }
    dbw[2][1] = learningRate*error*actFuncDiv(out)*bias[2];
    bw[2][1] += dbw[2][1];

    return error;
}

double net::actFunc(double value) { return tanh(value); }               //Гиперболический тангенс

double net::actFuncDiv(double value) { return 1/pow(cosh(value),2); }

void net::printInfo()
{
    cout << endl;
    cout << "x1 = " << x[0] << endl;
    cout << "x2 = " << x[1] << endl;
    /*cout << "a00 = " << a[0][0] << endl;
    cout << "a01 = " << a[0][1] << endl;
    cout << "a02 = " << a[0][2] << endl;
    cout << "a10 = " << a[1][0] << endl;
    cout << "a11 = " << a[1][1] << endl;
    cout << "a12 = " << a[1][2] << endl;*/
    cout << "output = " << out << endl;
    cout << "error = " << error << endl;
    cout << "averageError = " << averageError << endl;
}
