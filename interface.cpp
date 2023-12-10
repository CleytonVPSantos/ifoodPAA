#include <iostream>
#include <vector>
#include <chrono>            // For time measurement
#include <tuple>
#include "graph.h"           // Auxiliary functions for graph manipulation
#include "createGraph.h"     // Auxiliary functions for graph creation
#include "dijkstra.h"        // Auxiliary funcitons for operations
using namespace std;
using namespace std::chrono;

void screen() // Welcoming menu
{
    string strLogo = "\tdP     dP          dP                          88888888b                     dP   \n\t88     88          88                          88                            88   \n\t88     88 88d888b. 88d888b. .d8888b. 88d888b. a88aaaa    .d8888b. .d8888b. d8888P \n\t88     88 88'  `88 88'  `88 88'  `88 88'  `88  88        88'  `88 Y8ooooo.   88   \n\tY8.   .8P 88       88.  .88 88.  .88 88    88  88        88.  .88       88   88   \n\t`Y88888P' dP       88Y8888' `88888P8 dP    dP  dP        `88888P8 `88888P'   dP";
    string strOptionMenu = "\t*---------------------------------------*----------------------------------------*\n\t|                              0 - Sair do programa                              |\n\t*---------------------------------------*----------------------------------------*\n\t| 1 - Inserir informacoes sobre a cidade (input de ruas)                         |\n\t| 2 - Inserir informacoes sobre entregadores                                     |\n\t| 3 - Inserir pedido e retornar entregadores proximos (operacao 1)               |\n\t| 4 - Definir rota dado pedido e entregador (operacao 2)                         |\n\t| 5 - Buscar rota via centro de distribuicao (operacao 3)                        |\n\t*--------------------------------------------------------------------------------*";
    cout << strLogo << endl;
    cout << "\t   /------------------------------------------------------------------\\\n           |                      INSIRA A OPERACAO DESEJADA                  |" << endl;
    cout << "           \\__________________________________________________________________/" << endl;
    cout << strOptionMenu << endl;
}

void transition() // Transition between screens
{
    cout << "Pressione Enter para continuar..." << endl;
    cin.ignore();
    cin.get();
}

void execTime(chrono::duration<double, milli> timeDuration) // Print execution time
{
    cout << "Tempo de execucao: " << timeDuration.count() << " milissegundos." << endl;
}

bool choose(struct Graph **ptrGraph, vector<tuple<int, int, double, int>> &address) // Choose operation
{
    // Auxiliary variables
    int iOption, iSecondaryOption, numRequest, numCorners, numDCs;
    vector<tuple<int, int, double, int>> newAddress;
    tuple<int, int, double, int> deliveryAddress, genericAddress;
    vector<int> answer;
    vector< tuple< int, int, vector<int> > > tupleAnswer;
    Vertex iniVertex, vtxDelivery, vtxShop, vtxCustomer;
    
    string strFileName, strConfirm;
    
    // Time measurement variables
    auto timeStart = high_resolution_clock::now();
    auto timeStop = high_resolution_clock::now();
    auto timeDuration = duration<double, milli>(timeStop - timeStart);
    
    // Get operation from user
    cin >> iOption;
    switch (iOption)
    {
    case 0:
        cout << "Opcao 0 - Sair" << endl;
        cout << "Digite SAIR para confirmar" << endl;
        cin >> strConfirm;
        if (strConfirm == "SAIR")
        {
            cout << "Saindo..." << endl;
            transition();
            return true;
        }
        else
        {
            cout << "Operacao cancelada." << endl;
            transition();
            return false;
        }
    case 1:
        cout << "Opcao 1 - Inserir informacoes sobre a cidade (input de ruas)" << endl;
        if ((**ptrGraph).numVertices != 0) // Confirm possible loss of information
        {
            cout << "Ja ha uma ruas cadastradas. Deseja sobrescrever? (S/N)" << endl;
            cin >> strConfirm;
            if (strConfirm != "S")
            {
                cout << "Operacao cancelada." << endl;
                transition();
                return false;
            }
        }
        // Ask whether user wants to input from file or terminal
        cout << "Voce gostaria de fazer o input via arquivo (0) ou via terminal (1)?" << endl;
        cin >> iSecondaryOption;
        switch (iSecondaryOption)
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
                    transition();
                }
                else
                {
                    cout << "Erro ao criar grafo." << endl;
                    transition();
                }
                return false;
            case 1:
                *ptrGraph = (Graph*)constructGraphFromUserInput();
                if (ptrGraph != nullptr)
                {
                    cout << "Grafo criado com sucesso!" << endl;
                    transition();
                }
                else
                {
                    cout << "Erro ao criar grafo." << endl;
                    transition();
                }
                return false;
            default:
                cout << "Opcao invalida." << endl;
                transition();
                return false;
        }
    case 2:
        cout << "Opcao 2 - Inserir informacoes sobre entregadores" << endl;
        if (address.size() != 0) // Confirm possible loss of information
        {
            cout << "Ja ha entregadores cadastrados. Deseja sobrescrever? (S/N)" << endl;
            cin >> strConfirm;
            if (strConfirm != "S")
            {
                cout << "Operacao cancelada." << endl;
                transition();
                return false;
            }
        }
        // Erase past delivery addresses
        address = vector<tuple<int, int, double, int>>();
        // Ask whether user wants to input from file or terminal
        cout << "Voce gostaria de fazer o input via arquivo (0) ou via terminal (1)?" << endl;
        cin >> iSecondaryOption;
        switch (iSecondaryOption)
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
                transition();
                return false;
            case 1:
                address = constructAddressFromUserInput();
                cout << "Entregadores adicionados!" << endl;
                transition();
                return false;
            default:
                cout << "Opcao invalida." << endl;
                transition();
                return false;
        }
    case 3:
        cout << "Opcao 3 - Inserir pedido e retornar entregadores proximos (operacao 1)" << endl;
        if (address.size() == 0) // Confirm possible loss of information
        {
            cout << "Desculpe, nenhum entregador cadastrado." << endl;
            transition();
            return false;
        }
        if (*ptrGraph == nullptr) // Confirm possible loss of information
        {
            cout << "Nao ha rua cadastrada." << endl;
            transition();
            return false;
        }
        int iVertex1, iVertex2;
        double fWeight;
        cout << "Indique o endereco de coleta do pedido:" << endl;
        cin >> iVertex1 >> iVertex2 >> fWeight;
        // Add to the address vector
        address.push_back(make_tuple(iVertex1, iVertex2, fWeight, 3));
        // Add temporal vertices to the graph
        (*ptrGraph)->addTemporalVertices(&address);
        // Find the shop vertex
        for (int i = 0; i < (*ptrGraph)->numVertices; i++)
        {
            if ((*ptrGraph)->vertices[i].type == 3)
            {
                iniVertex = (*ptrGraph)->vertices[i];
                break;
            }
        }
        cout << "Insira o numero de entregadores proximos a serem encontrados:" << endl;
        cin >> numRequest;
        timeStart = high_resolution_clock::now();
        answer = findNClosest(iniVertex, **ptrGraph, numRequest);
        timeStop = high_resolution_clock::now();
        timeDuration = duration<double, milli>(timeStop - timeStart);
        // Erase temporal vertices from the graph
        (**ptrGraph).deleteTemporalVertices();
        numCorners = (**ptrGraph).numVertices;
        cout << "Os " << numRequest << " entregadores mais proximos estao nos enderecos:" << endl;
        for (int i = 0; i < numRequest; i++)
        {
            deliveryAddress = address[answer[i] - numCorners];
            cout << "(" << get<0>(deliveryAddress) << ", " << get<1>(deliveryAddress) << ", "
                 << get<2>(deliveryAddress) << ")" << endl;
        }
        for (int i = 0; i < address.size(); i++){
            if(get<3>(address[i]) != 1){
                address.erase(address.begin() + i);
                i--;
            }
        }
        execTime(timeDuration);
        transition();
        return false;
    case 4:
        cout << "Opcao 4 - Definir rota dado pedido e entregador (operacao 2)" << endl;
        if (*ptrGraph == nullptr) // Confirm possible loss of information
        {
            cout << "Nao ha rua cadastrada." << endl;
            transition();
            return false;
        }
        newAddress = vector<tuple<int, int, double, int>>();
        cout << "Insira o endereco em que se encontra o entregador:" << endl;
        cin >> iVertex1 >> iVertex2 >> fWeight;
        newAddress.push_back(make_tuple(iVertex1, iVertex2, fWeight, 1));
        cout << "Insira o endereco de coleta do pedido:" << endl;
        cin >> iVertex1 >> iVertex2 >> fWeight;
        newAddress.push_back(make_tuple(iVertex1, iVertex2, fWeight, 3));
        cout << "Insira o endereco de entrega do pedido:" << endl;
        cin >> iVertex1 >> iVertex2 >> fWeight;
        newAddress.push_back(make_tuple(iVertex1, iVertex2, fWeight, 2));
        timeStart = high_resolution_clock::now();
        (*ptrGraph)->addTemporalVertices(&newAddress);
        // Find the delivery, shop and customer vertices
        for (auto x: (**ptrGraph).vertices) {
            if (x.type == 1)
                vtxDelivery = x;
            else if (x.type == 3)
                vtxShop = x;
            else if (x.type == 2)
                vtxCustomer = x;
        }
        answer = simpleDelivery(vtxDelivery, vtxShop, vtxCustomer, **ptrGraph);
        timeStop = high_resolution_clock::now();
        timeDuration = duration<double, milli>(timeStop - timeStart);
        // Erase temporal vertices from the graph  
        (**ptrGraph).deleteTemporalVertices();
        numCorners = (**ptrGraph).numVertices;
        cout << "A rota do entregador e:" << endl;
        for (int i = 0; i < answer.size() - 1; i++)
        {
            if (answer[i] >= numCorners) {
                genericAddress = newAddress[answer[i] - numCorners];
                cout << "(" << get<0>(genericAddress) << ", " << get<1>(genericAddress) << ", "
                     << get<2>(genericAddress) << ") --> ";
            }
            else {
                cout << answer[i] << " --> ";
            }
        }
        if (answer[answer.size() - 1] > numCorners) {
            genericAddress = newAddress[answer[answer.size() - 1] - numCorners];
            cout << "(" << get<0>(genericAddress) << ", " << get<1>(genericAddress) << ", "
                 << get<2>(genericAddress) << ")" << endl;
        }
        else {
            cout << answer[answer.size() - 1] << endl;
        }
        newAddress = vector<tuple<int, int, double, int>>();
        execTime(timeDuration);
        transition();
        return false;
    case 5:
        cout << "Opcao 5 - Buscar rota via centro de distribuicao (operacao 3)" << endl;
        if (*ptrGraph == nullptr) // Corfirm possible loss of information
        {
            cout << "Nao ha rua cadastrada." << endl;
            transition();
            return false;
        }
        if (address.size() == 0) // Confirm possible loss of information
        {
            cout << "Desculpe, nenhum entregador cadastrado." << endl;
            transition();
            return false;
        }
        cout << "E necessario adicionar centros de distribuicao para essa operacao." << endl;
        cout << "Voce gostaria de fazer o input via arquivo (0) ou via terminal (1)?" << endl;
        cin >> iSecondaryOption;
        if (iSecondaryOption == 0) {
            cout << "Insira o nome do arquivo:" << endl;
            cin >> strFileName;
            newAddress = addDCsFromFile(strFileName);
            for (auto x: newAddress) {
                address.push_back(x);
            }
        }
        else if (iSecondaryOption == 1) {
            cout << "Insira a quantidade de centros de distribuicao para esse pedido:" << endl;
            cin >> numDCs;
            cout << "Nas proximas " << numDCs << " linhas, insira o endereco de cada centro de distribuicao:" << endl;
            for (int i = 0; i < numDCs; i++) {
                cin >> iVertex1 >> iVertex2 >> fWeight;
                address.push_back(make_tuple(iVertex1, iVertex2, fWeight, 4));
            }
        }
        else {
            cout << "Opcao invalida." << endl;
            transition();
            return false;
        }
        cout << "Centros de distribuicao adicionados!" << endl;
        cout << "Insira o endereco de entrega do pedido (cliente):" << endl;
        cin >> iVertex1 >> iVertex2 >> fWeight;
        address.push_back(make_tuple(iVertex1, iVertex2, fWeight, 2));
        cout << "Insira o numero de entregadores proximos a serem encontrados:" << endl;
        cin >> numRequest;
        timeStart = high_resolution_clock::now();
        // Add temporal vertices to the graph
        (*ptrGraph)->addTemporalVertices(&address);
        // Find the customer vertex
        for (auto x: (**ptrGraph).vertices) {
            if (x.type == 2)
                vtxCustomer = x;
        }
        tupleAnswer = optmizedDelivery(vtxCustomer, **ptrGraph, numRequest);
        timeStop = high_resolution_clock::now();
        timeDuration = duration<double, milli>(timeStop - timeStart);
        // Erase temporal vertices from the graph
        (**ptrGraph).deleteTemporalVertices();
        numCorners = (**ptrGraph).numVertices;
        numRequest = tupleAnswer.size();
        cout << "===============" << endl;
        for (int i = 0; i < numRequest; i++) {
            cout << "Caminho encontrado!\n";
            genericAddress = address[get<0>(tupleAnswer[i]) - numCorners];
            cout << "Endereco do entregador: (" << get<0>(genericAddress) << ", " << get<1>(genericAddress) << ", " << get<2>(genericAddress) << ")" << endl;
            genericAddress = address[get<1>(tupleAnswer[i]) - numCorners];
            cout << "Endereco do centro de distribuicao: (" << get<0>(genericAddress) << ", " << get<1>(genericAddress) << ", " << get<2>(genericAddress) << ")" << endl;
            cout << "Rota do entregador: ";
            for (int j = 0; j < get<2>(tupleAnswer[i]).size() - 1; j++)
            {
                if (get<2>(tupleAnswer[i])[j] >= numCorners) {
                    genericAddress = address[get<2>(tupleAnswer[i])[j] - numCorners];
                    cout << "(" << get<0>(genericAddress) << ", " << get<1>(genericAddress) << ", "
                         << get<2>(genericAddress) << ") --> ";
                }
                else {
                    cout << get<2>(tupleAnswer[i])[j] << " --> ";
                }
            }
            if (get<2>(tupleAnswer[i])[get<2>(tupleAnswer[i]).size() - 1] >= numCorners) {
                genericAddress = address[get<2>(tupleAnswer[i])[get<2>(tupleAnswer[i]).size() - 1] - numCorners];
                cout << "(" << get<0>(genericAddress) << ", " << get<1>(genericAddress) << ", "
                     << get<2>(genericAddress) << ")" << endl;
            }
            else {
                cout << get<2>(tupleAnswer[i])[get<2>(tupleAnswer[i]).size() - 1] << endl;
            }
            cout << "===============" << endl;
        }
        for (int i = 0; i < address.size(); i++){
            if(get<3>(address[i]) != 1){
                address.erase(address.begin() + i);
                i--;
            }
        }
        execTime(timeDuration);
        transition();
        return false;
    default:
        cout << "Opcao invalida." << endl;
        transition();
        return false;
    }
}