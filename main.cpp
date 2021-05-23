#include <iostream>
#include <math.h>
#include <fstream>

using namespace std;

void startInitSet();                //��砫쭠� ���䨣����, ��砩�� ���祭�� ��ᮢ
void initSet(unsigned variant);     //ᬥ�� �室��� � ��室��� ������
void evalateNeuralNetwork();        //�㭪�� ��אַ�� �����࠭���� ᨣ����, �襭�� �����
double actFunc(double value);       //��⨢�樮���� �㭪��
double getNeuralNetError();         //�㭪�� ���᫥��� ���஢�� ��ᮢ
void trainNetwork();                //�㭪�� �७�஢��
double actFuncDiv(double value);    //�ந������� �㭪樨 ��⨢�樨
void printInfo();

double inputs[4][2] = {{0,0},{0,1},{1,0},{1,1}}; //�������� �室��� ������
double desiredResult = 1;       //�����쭮� ���祭�� �� ��室�

double x[2];    //�室�� �������⢨�
double a[3];    //���ﭨ� ���஭�� a
double b[3];    //���ﭨ� ���஭�� b
double bias[3]; //���஭� ᬥ饭��
double out = 0; //��室

double xw[3][3];  //��� ���⮣� ᫮� a
double aw[3][3];  //��� ���⮣� ᫮� b
double bw[3];    //��� ��室���� ᫮�
double biw[3][3];   //��� ���஭�� ᬥ饭��

double delta = 0;   //�訡�� �� ��室�
double del_a[3];    //�訡�� ���஭� �
double del_b[3];    //�訡�� ���஭� b

double dxw[3][3];    //������ ��ᮢ ᫮� a
double daw[3][3];    //������ ��ᮢ ᫮� b
double dbw[3];       //������ ��ᮢ ��室���� ᫮�
double dbiw[3][3];       //������ ��ᮢ ᫮� ᬥ饭��

double learningRate = 0.1;      //�����樥�� ���祭��  --0.825--
double limit = 0.0000001;    //��筮��� ���祭��
unsigned epoch = 0;     //���-�� ��


int main()
{
    char isTrain;
    cout << "�७�஢��� �������? ('y' or 'n')";
    cin >> isTrain;
    if(isTrain == 'y')
    {
        //�७�஢��
        trainNetwork();
        ofstream out;          // ��⮪ ��� �����
        out.open("coefficents.txt"); // ���뢠�� 䠩� ��� �����
        if (out.is_open())
        {

        }
        out.close();
    }
/*
    string line;
    ifstream in("coefficents.txt"); // ���뢠�� 䠩� ��� �⥭��
    if (in.is_open())
        {
            while (getline(in, line))
            {
                std::cout << line << std::endl;
            }
        }
    in.close();     // ����뢠�� 䠩�
*/
    cout << endl << "�஢�ઠ: " << endl;

    auto p = 0.0;


    for (int i = 0; i < 4; i++)
    {
        initSet(i);
        evalateNeuralNetwork();
        printInfo();
        p += abs(desiredResult - out);;
    }
    cout << endl << "��筮���: " << 100 - p/4*100 << "%" << endl;

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
            initSet(i); //ᬥ�� �����饩 �롮ન
            evalateNeuralNetwork();
            err = getNeuralNetError();
         }
        epoch++;
        if((epoch % 100) == 0)
        {
            cout << "----------------------- ���� " << epoch << " ---------------------------" << endl;
            printInfo();
        }
    }


    cout << endl << "---------------------���祭�� �����襭�!-----------------------" << endl << endl;
    cout << " ��筮��� ����� " << (1-limit)*100 << "%" << endl;
    cout << " �����樥�� ᪮��� ���祭�� " << learningRate << endl;
    cout << " ���ॡ�������: "  << epoch << " ��" << endl;

}

//����� ������� ������
void initSet(unsigned variant)
{
    //������ �室�
    for(int i = 0; i < 2; i++)
    {
        x[i] = inputs[variant][i];
    }

    //������ ��室(������ �᪫���饥 .���.)
    desiredResult = (double)((bool)x[0] ^ (bool)x[1]);
}

//��砫쫭�� ���樠������ �����祭��� �� ��砭묨 ���祭�ﬨ
void startInitSet()
{
    //������ �室�
    for(int i = 0; i < 2; i++)
    {
        x[i] = inputs[1][i];
    }

    //������ ��室(������ �᪫���饥 .���.)
    desiredResult = (double)((bool)x[0] ^ (bool)x[1]);

    //���஭� ᬥ饭�� �ᥣ�� ࠢ�� 1
    bias[0] = 1;
    bias[1] = 1;
    bias[2] = 1;

    //�ਤ��� ��砩�� ���祭�� ��ᠬ
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


//����� ��室���� ���祭��
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
    //��室
    out = actFunc(b[0]*bw[0] + b[1]*bw[1] + b[2]*bw[2] + bias[2]*biw[2][1]);

}


//����� �訡��
double getNeuralNetError()
{
    //���᫥��� �訡��
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
    //���᫥��� ���४�஢�� xw
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


    //���᫥��� ���४�஢�� aw
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

    //���᫥��� ���४�஢�� bw
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

    //���஭� ᬥ饭��


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
