#ifndef INTERFACE_H
#define INTERFACE_H
#include <iostream>
#include <chrono>

using namespace std;

void screen();

void transition();

void execTime(chrono::duration<double, milli> timeDuration);

bool choose(struct Graph **ptrGraph, vector<tuple<int, int, double, int>> &address);

#endif