#include <iostream>
#include <vector>
#include <tuple>
#include "Graph.h"
#include "create_graph.h"
#include "dijkstra.h"
using namespace std;

void tela()
{
    string strTelaInicio = "\tdP     dP          dP                          88888888b                     dP   \n\t88     88          88                          88                            88   \n\t88     88 88d888b. 88d888b. .d8888b. 88d888b. a88aaaa    .d8888b. .d8888b. d8888P \n\t88     88 88'  `88 88'  `88 88'  `88 88'  `88  88        88'  `88 Y8ooooo.   88   \n\tY8.   .8P 88       88.  .88 88.  .88 88    88  88        88.  .88       88   88   \n\t`Y88888P' dP       88Y8888' `88888P8 dP    dP  dP        `88888P8 `88888P'   dP";
    string strOpcoes = "\t*---------------------------------------*----------------------------------------*\n\t|                              0 - Sair do programa                              |\n\t*---------------------------------------*----------------------------------------*\n\t| 1 - Inserir informacoes sobre a cidade (input de ruas)                         |\n\t| 2 - Inserir informacoes sobre entregadores                                     |\n\t| 3 - Inserir pedido e retornar entregadores proximos (operacao 1)               |\n\t| 4 - Definir rota dado pedido e entregador (operacao 2)                         |\n\t| 5 - Buscar rota via centro de distribuicao (operacao 3)                        |\n\t*--------------------------------------------------------------------------------*";
    cout << strTelaInicio << endl;
    cout << "\t   /------------------------------------------------------------------\\\n           |                      INSIRA A OPERACAO DESEJADA                  |" << endl;
    cout << "           \\__________________________________________________________________/" << endl;
    cout << strOpcoes << endl;
}

void transicao()
{
    cout << "Pressione Enter para continuar..." << endl;
    cin.ignore();
    cin.get();
}

bool escolha(struct Graph **ptrGraph, vector<tuple<int, int, double, int>> &address)
{
    int iOpcao, iOpcao2, k;
    vector<int> resposta;
    Vertex iniVertex;
    
    string strFileName, strConfirma;
    
    cin >> iOpcao;
    switch (iOpcao)
    {
    case 0:
        cout << "Opcao 0 - Sair" << endl;
        cout << "Digite SAIR para confirmar" << endl;
        cin >> strConfirma;
        if (strConfirma == "SAIR")
        {
            cout << "Saindo..." << endl;
            transicao();
            return true;
        }
        else
        {
            cout << "Operacao cancelada." << endl;
            transicao();
            return false;
        }
    case 1:
        cout << "Opcao 1 - Inserir informacoes sobre a cidade (input de ruas)" << endl;
        cout << "Voce gostaria de fazer o input via arquivo (0) ou via terminal (1)?" << endl;
        cin >> iOpcao2;
        switch (iOpcao2)
        {
            case 0:
                cout << "Insira o nome do arquivo:" << endl;
                cin >> strFileName;
                *ptrGraph = (Graph*)constructGraphFromFile(strFileName);
                if (*ptrGraph != nullptr)
                {
                    cout << "Grafo criado com sucesso!" << endl;
                    transicao();
                }
                else
                {
                    cout << "Erro ao criar grafo." << endl;
                    transicao();
                }
                return false;
            case 1:
                *ptrGraph = (Graph*)constructGraphFromUserInput();
                if (ptrGraph != nullptr)
                {
                    cout << "Grafo criado com sucesso!" << endl;
                    transicao();
                }
                else
                {
                    cout << "Erro ao criar grafo." << endl;
                    transicao();
                }
                return false;
            default:
                cout << "Opcao invalida." << endl;
                transicao();
                return false;
        }
    case 2:
        cout << "Opcao 2 - Inserir informacoes sobre entregadores" << endl;
        cout << "Voce gostaria de fazer o input via arquivo (0) ou via terminal (1)?" << endl;
        cin >> iOpcao2;
        switch (iOpcao2)
        {
            case 0:
                cout << "Insira o nome do arquivo:" << endl;
                cin >> strFileName;
                address = constructAddressFromFile(strFileName);
                cout << "Entregadores adicionados!" << endl;
                transicao();
                return false;
            case 1:
                address = constructAddressFromUserInput();
                cout << "Entregadores adicionados!" << endl;
                transicao();
                return false;
            default:
                cout << "Opcao invalida." << endl;
                transicao();
                return false;
        }
    case 3:
        cout << "Opcao 3 - Inserir pedido e retornar entregadores proximos (operacao 1)" << endl;
        if (address.size() == 0) {
            cout << "Desculpe, nenhum entregador cadastrado." << endl;
            transicao();
            return false;
        }
        if (*ptrGraph == nullptr)
        {
            cout << "Nao ha rua cadastrada." << endl;
            transicao();
            return false;
        }
        int iVertice1, iVertice2;
        double fWeight;
        cout << "Indique o endereco de coleta do pedido:" << endl;
        cin >> iVertice1 >> iVertice2 >> fWeight;
        address.push_back(make_tuple(iVertice1, iVertice2, fWeight, 3));
        //Ordenar adress
        (*ptrGraph)->addTemporalVertices(address);
        //iterar para achar o vertice de coleta tipo
        for (int i = 0; i < (*ptrGraph)->numVertices; i++)
        {
            if ((*ptrGraph)->vertices[i].type == 3)
            {
                iniVertex = (*ptrGraph)->vertices[i];
                break;
            }
        }
        cout << "Insira o numero de entregadores proximos a serem encontrados:" << endl;
        cin >> k;/*
        cout << "Infos do grafo:\n";
        cout << "numVertices: " << (*ptrGraph)->numVertices << endl;
        for (auto x: (**ptrGraph).vertices)
        {
            cout << "id: " << x.id << " type: " << x.type << endl;
        }
        iOpcao2 = 0;
        for (auto y: (**ptrGraph).edges)
        {
            cout << "vizinhos de " << iOpcao2++ << ": ";
            while (y != nullptr) {
                cout << y->vertexId << " ";
                y = y->next;
            }
            cout << endl;
        }*/
        resposta = findNClosest(iniVertex, **ptrGraph, k);
        cout << "Os " << k << " entregadores mais proximos sao:" << endl;
        for (int i = 0; i < k; i++)
        {
            cout << resposta[i] << endl;
        }
        transicao();
        return false;
    default:
        cout << "Opcao invalida." << endl;
        transicao();
        return false;
    }
}