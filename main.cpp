#include <iostream>
#include <vector>
#include <tuple>
#include "interface.h"
#include "createGraph.h"
#include "graph.h"

using namespace std;

int main()
{
    bool bExit = false;
    struct Graph *ptrGraphCity = newGraph(0);
    vector<tuple<int, int, double, int>> address;
    while (!bExit)
    {
        tela();
        bExit = escolha(&ptrGraphCity, address);
    }
    return 0;
}