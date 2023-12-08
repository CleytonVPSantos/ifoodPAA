#include <iostream>
#include "interface.h"
#include "Graph.h"

using namespace std;

int main()
{
    bool bExit = false;
    struct Graph *ptrGraphCity = nullptr;
    while (!bExit)
    {
        tela();
        bExit = escolha(&ptrGraphCity);
    }
    return 0;
}