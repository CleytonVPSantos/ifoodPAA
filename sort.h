#ifndef SORT_H
#define SORT_H
#include <iostream>
#include <vector>
#include <tuple>

bool operator>(const std::tuple<int, int, float, int>& a, const std::tuple<int, int, double, int>& b);

void swap(std::vector<std::tuple<int, int, double, int>>* address, int i, int j);

void heapify(std::vector<std::tuple<int, int, double, int>>* address, int n, int root);

void buildHeap(std::vector<std::tuple<int, int, double, int>>* address, int n);

void heapSort(std::vector<std::tuple<int, int, double, int>>* address);


#endif