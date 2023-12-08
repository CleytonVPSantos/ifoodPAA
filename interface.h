#ifndef INTERFACE_H
#define INTERFACE_H
#include <iostream>

using namespace std;

void tela();

void transicao();

bool escolha(struct Graph **ptrGraph, vector<tuple<int, int, double, int>> &address);

#endif