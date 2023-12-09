#include <iostream>
#include <vector>
#include <chrono>
#include <tuple>
#include "graph.h"
#include "create_graph.h"
#include "dijkstra.h"
using namespace std;
using namespace std::chrono;

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

void execTime(chrono::duration<double, milli> timeDuration) {
    cout << "Tempo de execucao: " << timeDuration.count() << " milissegundos." << endl;
}

bool escolha(struct Graph **ptrGraph, vector<tuple<int, int, double, int>> &address)
{
    int iOpcao, iOpcao2, k, numEsquinas, numDCs;
    vector<tuple<int, int, double, int>> newAddress;
    tuple<int, int, double, int> entregador, endereco;
    vector<int> resposta;
    vector< tuple< int, int, vector<int> > > respostaTupla;
    Vertex iniVertex, vtxEntregador, vtxColeta, vtxCliente;
    
    string strFileName, strConfirma;
    
    auto timeStart = high_resolution_clock::now();
    auto timeStop = high_resolution_clock::now();
    auto timeDuration = duration<double, milli>(timeStop - timeStart);
    
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
        if ((**ptrGraph).numVertices != 0)
        {
            cout << "Ja ha uma ruas cadastradas. Deseja sobrescrever? (S/N)" << endl;
            cin >> strConfirma;
            if (strConfirma == "N")
            {
                cout << "Operacao cancelada." << endl;
                transicao();
                return false;
            }
        }
        cout << "Voce gostaria de fazer o input via arquivo (0) ou via terminal (1)?" << endl;
        cin >> iOpcao2;
        switch (iOpcao2)
        {
            case 0:
                cout << "Insira o nome do arquivo:" << endl;
                cin >> strFileName;
                timeStart = high_resolution_clock::now();
                *ptrGraph = (Graph*)constructGraphFromFile(strFileName);
                timeStop = high_resolution_clock::now();
                timeDuration = duration<double, milli>(timeStop - timeStart);
                if (*ptrGraph != nullptr)
                {
                    cout << "Grafo criado com sucesso!" << endl;
                    execTime(timeDuration);
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
        if (address.size() != 0)
        {
            cout << "Ja ha entregadores cadastrados. Deseja sobrescrever? (S/N)" << endl;
            cin >> strConfirma;
            if (strConfirma == "N")
            {
                cout << "Operacao cancelada." << endl;
                transicao();
                return false;
            }
        }
        address = vector<tuple<int, int, double, int>>();
        cout << "Voce gostaria de fazer o input via arquivo (0) ou via terminal (1)?" << endl;
        cin >> iOpcao2;
        switch (iOpcao2)
        {
            case 0:
                cout << "Insira o nome do arquivo:" << endl;
                cin >> strFileName;
                timeStart = high_resolution_clock::now();
                address = constructAddressFromFile(strFileName);
                timeStop = high_resolution_clock::now();
                timeDuration = duration<double, milli>(timeStop - timeStart);
                cout << "Entregadores adicionados!" << endl;
                execTime(timeDuration);
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
        (*ptrGraph)->addTemporalVertices(&address);
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
        timeStart = high_resolution_clock::now();
        resposta = findNClosest(iniVertex, **ptrGraph, k);
        timeStop = high_resolution_clock::now();
        timeDuration = duration<double, milli>(timeStop - timeStart);
        (**ptrGraph).deleteTemporalVertices();
        numEsquinas = (**ptrGraph).numVertices;
        cout << "Os " << k << " entregadores mais proximos estao nos enderecos:" << endl;
        for (int i = 0; i < k; i++)
        {
            entregador = address[resposta[i] - numEsquinas];
            cout << "(" << get<0>(entregador) << ", " << get<1>(entregador) << ", "
                 << get<2>(entregador) << ")" << endl;
        }
        execTime(timeDuration);
        transicao();
        return false;
    case 4:
        cout << "Opcao 4 - Definir rota dado pedido e entregador (operacao 2)" << endl;
        if (address.size() != 0) {
            cout << "AVISO: Essa operacao sobrescreve a lista de entregadores." << endl;
            cout << "Deseja continuar? (S/N)" << endl;
            cin >> strConfirma;
            if (strConfirma == "N")
            {
                cout << "Operacao cancelada." << endl;
                transicao();
                return false;
            }
            address = vector<tuple<int, int, double, int>>();
        }
        if (*ptrGraph == nullptr)
        {
            cout << "Nao ha rua cadastrada." << endl;
            transicao();
            return false;
        }
        cout << "Insira o endereco em que se encontra o entregador:" << endl;
        cin >> iVertice1 >> iVertice2 >> fWeight;
        address.push_back(make_tuple(iVertice1, iVertice2, fWeight, 1));
        cout << "Insira o endereco de coleta do pedido:" << endl;
        cin >> iVertice1 >> iVertice2 >> fWeight;
        address.push_back(make_tuple(iVertice1, iVertice2, fWeight, 3));
        cout << "Insira o endereco de entrega do pedido:" << endl;
        cin >> iVertice1 >> iVertice2 >> fWeight;
        address.push_back(make_tuple(iVertice1, iVertice2, fWeight, 2));
        timeStart = high_resolution_clock::now();
        (*ptrGraph)->addTemporalVertices(&address);
        for (auto x: (**ptrGraph).vertices) {
            if (x.type == 1)
                vtxEntregador = x;
            else if (x.type == 3)
                vtxColeta = x;
            else if (x.type == 2)
                vtxCliente = x;
        }
        resposta = simpleDelivery(vtxEntregador, vtxColeta, vtxCliente, **ptrGraph);
        timeStop = high_resolution_clock::now();
        timeDuration = duration<double, milli>(timeStop - timeStart);
        (**ptrGraph).deleteTemporalVertices();
        numEsquinas = (**ptrGraph).numVertices;
        cout << "A rota do entregador e:" << endl;
        for (int i = 0; i < resposta.size() - 1; i++)
        {
            if (resposta[i] >= numEsquinas) {
                endereco = address[resposta[i] - numEsquinas];
                cout << "(" << get<0>(endereco) << ", " << get<1>(endereco) << ", "
                     << get<2>(endereco) << ") --> ";
            }
            else {
                cout << resposta[i] << " --> ";
            }
        }
        if (resposta[resposta.size() - 1] > numEsquinas) {
            endereco = address[resposta[resposta.size() - 1] - numEsquinas];
            cout << "(" << get<0>(endereco) << ", " << get<1>(endereco) << ", "
                 << get<2>(endereco) << ")" << endl;
        }
        else {
            cout << resposta[resposta.size() - 1] << endl;
        }
        execTime(timeDuration);
        transicao();
        return false;
    case 5:
        cout << "Opcao 5 - Buscar rota via centro de distribuicao (operacao 3)" << endl;
        if (*ptrGraph == nullptr)
        {
            cout << "Nao ha rua cadastrada." << endl;
            transicao();
            return false;
        }
        if (address.size() == 0) {
            cout << "Desculpe, nenhum entregador cadastrado." << endl;
            transicao();
            return false;
        }
        cout << "E necessario adicionar centros de distribuicao para essa operacao." << endl;
        cout << "Voce gostaria de fazer o input via arquivo (0) ou via terminal (1)?" << endl;
        cin >> iOpcao2;
        if (iOpcao2 == 0) {
            cout << "Insira o nome do arquivo:" << endl;
            cin >> strFileName;
            newAddress = addDCsFromFile(strFileName);
            for (auto x: newAddress) {
                address.push_back(x);
            }
        }
        else if (iOpcao2 == 1) {
            cout << "Insira a quantidade de centros de distribuicao para esse pedido:" << endl;
            cin >> numDCs;
            cout << "Nas proximas " << numDCs << " linhas, insira o endereco de cada centro de distribuicao:" << endl;
            for (int i = 0; i < numDCs; i++) {
                cin >> iVertice1 >> iVertice2 >> fWeight;
                address.push_back(make_tuple(iVertice1, iVertice2, fWeight, 4));
            }
        }
        else {
            cout << "Opcao invalida." << endl;
            transicao();
            return false;
        }
        cout << "Centros de distribuicao adicionados!" << endl;
        cout << "Insira o endereco de entrega do pedido (cliente):" << endl;
        cin >> iVertice1 >> iVertice2 >> fWeight;
        address.push_back(make_tuple(iVertice1, iVertice2, fWeight, 2));
        cout << "Insira o numero de entregadores proximos a serem encontrados:" << endl;
        cin >> k;
        timeStart = high_resolution_clock::now();
        (*ptrGraph)->addTemporalVertices(&address);
        for (auto x: (**ptrGraph).vertices) {
            if (x.type == 2)
                vtxCliente = x;
        }
        respostaTupla = optmizedDelivery(vtxCliente, **ptrGraph, k);
        timeStop = high_resolution_clock::now();
        timeDuration = duration<double, milli>(timeStop - timeStart);
        (**ptrGraph).deleteTemporalVertices();
        numEsquinas = (**ptrGraph).numVertices;
        k = respostaTupla.size();
        cout << "===============" << endl;
        for (int i = 0; i < k; i++) {
            cout << "Caminho encontrado!\n";
            endereco = address[get<0>(respostaTupla[i]) - numEsquinas];
            cout << "Endereco do entregador: (" << get<0>(endereco) << ", " << get<1>(endereco) << ", " << get<2>(endereco) << ")" << endl;
            endereco = address[get<1>(respostaTupla[i]) - numEsquinas];
            cout << "Endereco do centro de distribuicao: (" << get<0>(endereco) << ", " << get<1>(endereco) << ", " << get<2>(endereco) << ")" << endl;
            cout << "Rota do entregador: ";
            for (int j = 0; j < get<2>(respostaTupla[i]).size() - 1; j++)
            {
                if (get<2>(respostaTupla[i])[j] >= numEsquinas) {
                    endereco = address[get<2>(respostaTupla[i])[j] - numEsquinas];
                    cout << "(" << get<0>(endereco) << ", " << get<1>(endereco) << ", "
                         << get<2>(endereco) << ") --> ";
                }
                else {
                    cout << get<2>(respostaTupla[i])[j] << " --> ";
                }
            }
            if (get<2>(respostaTupla[i])[get<2>(respostaTupla[i]).size() - 1] >= numEsquinas) {
                endereco = address[get<2>(respostaTupla[i])[get<2>(respostaTupla[i]).size() - 1] - numEsquinas];
                cout << "(" << get<0>(endereco) << ", " << get<1>(endereco) << ", "
                     << get<2>(endereco) << ")" << endl;
            }
            else {
                cout << get<2>(respostaTupla[i])[get<2>(respostaTupla[i]).size() - 1] << endl;
            }
            cout << "===============" << endl;
        }
        execTime(timeDuration);
        transicao();
        return false;
    default:
        cout << "Opcao invalida." << endl;
        transicao();
        return false;
    }
}