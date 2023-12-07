#include <iostream>
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

bool escolha()
{
    int iOpcao, iData;
    
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
    default:
        cout << "Opcao invalida." << endl;
        transicao();
        return false;
    }
}