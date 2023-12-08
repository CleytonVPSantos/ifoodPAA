#include <iostream>
#include <vector>
#include <tuple>
#include "interface.h"
#include "Graph.h"

using namespace std;

int main()
{
    bool bExit = false;
    struct Graph *ptrGraphCity = nullptr;
    vector<tuple<int, int, float, int>> address;
    while (!bExit)
    {
        tela();
        bExit = escolha(&ptrGraphCity, address);
    }
    return 0;
}