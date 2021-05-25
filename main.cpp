#include "net.h"

int main()
{
    net n(0.00002,0.0005);
    char isTrain = 'y';
    cout << "�७�஢��� �������? ('y' or 'n')";
    cin >> isTrain;
    if(isTrain == 'y')
    {
        n.trainNetwork();
    }

    cout << endl << "�஢�ઠ: " << endl;

    auto p = 0.0;
    for (unsigned i = 0; i < 4; i++)
    {
        n.initSet(i);
        n.feedForward();
        n.printInfo();
        p += abs(n.getDesireResult() - n.getOutput());
    }
    cout << endl << "��筮���: " << 100 - p/4*100 << "%" << endl;

    return 0;
}
