#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

#include <iostream>
#include <tuple>
#include <fstream>
#include <sstream>
#include <set>

#include "commom.h"

using namespace std;

void parseObjectiveFunc(string line, vector<pair<int, mpq_class>>& objectiveFunction, set<int>& variables);

void parseConstraints(fstream& f, vector<vector<pair<int, mpq_class>>>& constraintsMtx, set<int> variables, int& auxVariables);

mtxData* inputParser(fstream& f);

#endif