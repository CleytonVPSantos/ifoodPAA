#include <iostream>
#include "interface.h"

using namespace std;

int main()
{
    bool bExit = false;
    while (!bExit)
    {
        tela();
        bExit = escolha();
    }
    return 0;
}