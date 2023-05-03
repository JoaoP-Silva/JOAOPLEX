#ifndef COMMOM_H
#define COMMOM_H

#include <vector>
#include <unordered_map>
#include <iostream>
#include <string>

#include "../lib/gmp/gmpxx.h"

using namespace std;

// ----Commom data structures-----

//Struct to save all data after parsing the input
struct mtxData{
    vector<mpq_class> objective;
    vector<vector<mpq_class>> constraints;
    unordered_map<int, int> variablesMap;
    int numObjectiveVar;
};


//Struct to save all data after simplex
struct results{
    vector<mpq_class> certificate;
    vector<mpq_class> solution;
    vector<int> variables;
    mpq_class z;
    string status;
};

//Commom functions
void printVector(vector<mpq_class>& v);

void printObjectiveFunct(mtxData* d);

void printConstraints(mtxData* d);

void printTableu(vector<vector<mpq_class>> m);

void printBase(vector<int> v);

void printResult(results* r);

void setSolution(results* r, vector<vector<mpq_class>> tableau,
                vector<int> base, mtxData* d);

#endif