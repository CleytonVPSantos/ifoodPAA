#ifndef INTERFACE_H
#define INTERFACE_H
#include <iostream>
#include <chrono>

using namespace std;

void tela();

void transicao();

void tempoExec(chrono::duration<double, milli> timeDuration);

bool escolha(struct Graph **ptrGraph, vector<tuple<int, int, double, int>> &address);

#endif