# Soluções para a UrbanFast 🚚📦

Este projeto é o trabalho final do curso de Projeto e Análise de Algoritmos, do curso de graduação em Ciência de Dados da EMAp. O curso foi ministrado por Thiago Pinheiro de Araújo, e este projeto foi elaborado por Luís Felipe Marques, Cleyton Santos, Otávio Matos, Jean Horn e Fabrício Venturim.

Este repositório contém soluções desenvolvidas para otimizar a operação da UrbanFast, um novo serviço de entregas fictício. O objetivo é criar algoritmos que resolvam quatro operações-chave.

## Operações Principais 🎯

### Operação 1: Encontrar Entregadores Próximos 📍
Esta funcionalidade permite localizar entregadores disponíveis nas proximidades para realizar um dado pedido, reduzindo o tempo de espera para os clientes.

### Operação 2: Definir a Rota de uma Entrega Simples 🗺️
A operação visa calcular a rota mais rápida para uma entrega simples, dado um entregador e o pedido a ser realizado.

### Operação 3: Definir a Rota de uma Entrega Considerando Centros de Distribuição 🏭
Para entregas que envolvem centros de distribuição, esta funcionalidade determina entregadores próximos para coletar o pedido e levá-lo ao cliente.

### Operação 4: Sugerir Entregas Adicionais com Base em uma Rota ➕
Essa operação oferece sugestões para entregas adicionais que podem ser incluídas de forma eficiente com base na rota atual.

## Pré-requisitos 🛠️

- C++17

## Instalação ⚙️

1. Clone o repositório para sua máquina local.
   ```bash
   git clone https://github.com/CleytonVPSantos/ifoodPAA.git
   cd urbanfast-delivery
   ```

2. Execute a aplicação.
   ```bash
   make main
   ```

3. Siga as instruções da aplicação.

## Guia de Estilo para Inputs 📝
Muitas vezes, a aplicação oferecerá a opção de carregar dados através de arquivos externos. Esses arquivos, muitas vezes de extensão `.txt`, devem seguir o seguinte guia de estilo para serem corretamente carregados.

### Input de grafo da cidade 🏙 (opção 1)
A primeira linha do arquivo deve ter um número `N`, a quantidade de esquinas a comporem ao grafo. As linhas seguintes descreverão as arestas do grafo. Cada linha dever da forma `X Y W`, composta de dois inteiros, `X` e `Y`, entre `0` e `N-1`, e um número decimal `W` representando o peso da aresta, proporcional ao tempo para percorrer a via entre as esquinas `X` e `Y`.

### Input de entregadores 🛵 (opção 2)
O arquivo deve ser composto de várias linhas, cada uma representando o endereço da localização de um entregador. Cada endereço deve ser da forma `X Y f`, sendo `X` e `Y` inteiros entre `0` e `N-1`, sendo `N` a quantidade de esquinas na cidade, e `f` um número decimal entre `0` e `1`, representando a fração do caminho entre `X` e `Y` em que se encontra o entregador.
