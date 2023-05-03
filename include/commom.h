#ifndef COMMOM_H
#define COMMOM_H

#include <vector>
#include <unordered_map>
#include <iostream>
#include "../lib/gmp/gmpxx.h"

using namespace std;

//Commom data structures
struct mtxData{
    vector<mpq_class> objective;
    vector<vector<mpq_class>> constraints;
    unordered_map<int, int> variablesMap;
};

//Commom functions
void printVector(vector<mpq_class>& v);

void printObjectiveFunct(mtxData* d);

void printConstraints(mtxData* d);

void printTableu(vector<vector<mpq_class>> m);

#endif