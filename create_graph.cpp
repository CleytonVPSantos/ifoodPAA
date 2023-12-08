#include <iostream>
#include <fstream>
#include "Graph.h"

using namespace std;

struct Graph* newGraph(int numVertices)
{
    struct Graph* newGraph = new Graph();
    newGraph->numVertices = numVertices;
    
    for (int i = 0; i < numVertices; i++) {
        newGraph->vertices.push_back(Vertex(i, 0));
        newGraph->edges.push_back(NULL);
    }

    return newGraph;
}

struct Graph* constructGraphFromFile(const string& strFileName)
{
    // Abre o arquivo
    ifstream inputFile(strFileName);
    // Se o arquivo não puder ser aberto, uma mensagem de erro será exibida
    if (!inputFile.is_open())
    {
        cout << "Erro ao abrir o arquivo " << strFileName << endl;
        return nullptr;
    }
    int numVertices;
    inputFile >> numVertices;
    struct Graph* ptrGraph = newGraph(numVertices);
    int iVertex1, iVertex2;
    float fWeight;
    // Lê os dados do arquivo e insere-os na árvore
    while (inputFile >> iVertex1 >> iVertex2 >> fWeight)
    {
        ptrGraph->addEdge(Vertex(iVertex1, 0), Vertex(iVertex2, 0), fWeight);
    }
    // Fecha o arquivo
    inputFile.close();
    return ptrGraph;
}

struct Graph* constructGraphFromUserInput()
{
    int numVertices, numArestas;
    cout << "Digite o numero de vertices: ";
    cin >> numVertices;
    cout << "Digite o numero de arestas: ";
    cin >> numArestas;
    struct Graph* ptrGraph = newGraph(numVertices);
    int iVertex1, iVertex2;
    float fWeight;
    cout << "Nas proximas " << numArestas << " linhas, digite o vertice 1, o vertice 2 e o peso da aresta, separados por espaco:" << endl;
    for (int i = 0; i < numArestas; i++)
    {
        cin >> iVertex1 >> iVertex2 >> fWeight;
        ptrGraph->addEdge(Vertex(iVertex1, 0), Vertex(iVertex2, 0), fWeight);
    }
    return ptrGraph;
}

vector<tuple<int, int, float, int>> constructAddressFromFile(const string& strFileName)
{
    // Abre o arquivo
    ifstream inputFile(strFileName);
    vector<tuple<int, int, float, int>> ret;
    // Se o arquivo não puder ser aberto, uma mensagem de erro será exibida
    if (!inputFile.is_open())
    {
        cout << "Erro ao abrir o arquivo " << strFileName << endl;
        return ret;
    }
    int iVertex1, iVertex2, iType;
    float fWeight;
    while (inputFile >> iVertex1 >> iVertex2 >> fWeight)
    {
        ret.push_back(make_tuple(iVertex1, iVertex2, fWeight, 1));
    }
    // Fecha o arquivo
    inputFile.close();
    return ret;
}

vector<tuple<int, int, float, int>> constructAddressFromUserInput()
{
    int numAdress;
    cout << "Digite o numero de enderecos: ";
    cin >> numAdress;
    vector<tuple<int, int, float, int>> ret;
    int iVertex1, iVertex2;
    float fWeight;
    cout << "Nas proximas " << numAdress << " linhas, digite o vertice 1, o vertice 2 e a fracao em que o entregador se encontra do comeco da rua, separados por espaco:" << endl;
    for (int i = 0; i < numAdress; i++)
    {
        cin >> iVertex1 >> iVertex2 >> fWeight;
        ret.push_back(make_tuple(iVertex1, iVertex2, fWeight, 1));
    }
    return ret;
}