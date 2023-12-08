#ifndef CREATE_TREE_H
#define CREATE_TREE_H
#include <iostream>
#include <fstream>
using namespace std;

struct Graph* newGraph(int numVertices);

struct Graph* constructGraphFromFile(const string& strFileName);

// Constrói uma árvore binária de busca a partir de dados inseridos pelo usuário
struct Graph* constructGraphFromUserInput();

#endif