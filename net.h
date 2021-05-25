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

    vector<double> x;                   //�室��� ᫮�
    double out;                         //��室��� ᫮�
    double error = 0;                   //�訡�� ��
    double averageError;                //�।��� �訡�� �� ���

    vector<vector<unsigned>> inputs = {{0,0},{0,1},{1,0},{1,1}}; //�������� �室��� ������
    double desiredResult;                                        //��������� ���祭�� �� ��室� ��

    vector<vector<double>> a;           //����ﭨ� ���஭�� ᫮�:�����
    vector<vector<vector<double>>> w;   //����� ��ᮢ ᫮�:���஭:�����
    vector<vector<vector<double>>> dw;   //����� ���ࠢ�� ��ᮢ ᫮�:���஭:�����
    vector<vector<double>> delta;        //�訡�� ���஭�

    vector<double> bias;                //����ﭨ� ���஭�� ᬥ饭��
    vector<vector<double>> bw;          //��� ���஭�� ᬥ饭��
    vector<vector<double>> dbw;          //�யࠢ�� ��ᮢ ���஭�� ᬥ饭��


    double learningRate;        //�����樥�� ���祭��       ����� 0,3 �⠢���
    double limit;              //��筮��� ���祭��          0,001
    unsigned epoch = 0;                //���-�� ��

public:
    net(double _limit = 0.01, double _learningRate = 0.01);

    void initSet(unsigned variant);      //ᬥ�� �室��� � ��室��� ������
    double feedForward();                //�㭪�� ��אַ�� �����࠭���� ᨣ����, �襭�� �����
    double actFunc(double value);        //��⨢�樮���� �㭪��
    double backPropagation();            //�㭪�� ���᫥��� ���஢�� ��ᮢ
    void trainNetwork();                 //�㭪�� �७�஢�� ��
    double actFuncDiv(double value);     //�ந������� �㭪樨 ��⨢�樨
    void printInfo();                                       //�뢮� १����
    double getDesireResult() { return desiredResult; }
    double getOutput() { return out;}
};


#endif // NETH
