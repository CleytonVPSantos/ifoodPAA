#include "sort.h"

bool operator>(const std::tuple<int, int, double, int>& a, const std::tuple<int, int, double, int>& b) {
    // Comparação com base nas três primeiras entradas
    if (std::get<0>(a) != std::get<0>(b)) {
        return std::get<0>(a) > std::get<0>(b);
    }

    if (std::get<1>(a) != std::get<1>(b)) {
        return std::get<1>(a) > std::get<1>(b);
    }

    return std::get<2>(a) > std::get<2>(b);
}

void swap(std::vector<std::tuple<int, int, double, int>>* address, int i, int j) {
    std::tuple<int, int, double, int> aux = (*address)[i];
    (*address)[i] = (*address)[j];
    (*address)[j] = aux;
}

void heapify(std::vector<std::tuple<int, int, double, int>>* address, int n, int root) {
    int inx = root;
    int leftInx = 2 * root + 1;
    int rightInx = 2 * root + 2;

    if ((leftInx < n) && operator>((*address)[leftInx], (*address)[inx])) {
        inx = leftInx;
    }

    if ((rightInx < n) && operator>((*address)[rightInx], (*address)[inx])) {
        inx = rightInx;
    }

    if (inx != root) {
        swap(address, root, inx);
        heapify(address, n, inx);
    }
}

void buildHeap(std::vector<std::tuple<int, int, double, int>>* address, int n) {
    for (int i = (n / 2 - 1); i >= 0; i--) {
        heapify(address, n, i);
    }
}

void heapSort(std::vector<std::tuple<int, int, double, int>>* address) {
    int n = (*address).size();

    // Construir o heap inicial
    buildHeap(address, n);

    // Trocar o maior elemento (raiz) com o último elemento e reconstruir o heap
    for (int i = n - 1; i > 0; i--) {
        swap(address, 0, i);
        heapify(address, i, 0);
    }
}

void sortAddressList(std::vector<std::tuple<int, int, double, int>>* address) {
    // Percorre a lista, troca os elementos se o primeiro for maior que o segundo,
    // e ajusta o terceiro elemento para ser 1 a menos do seu valor original.
    for (auto& tuple : *address) {
        if (std::get<0>(tuple) > std::get<1>(tuple)) {
            // Trocar a primeira entrada com a segunda
            int aux = std::get<0>(tuple);
            std::get<0>(tuple) = std::get<1>(tuple);
            std::get<1>(tuple) = aux;
            std::get<2>(tuple) = 1 - std::get<2>(tuple);
        }
    }
    heapSort(address);
}