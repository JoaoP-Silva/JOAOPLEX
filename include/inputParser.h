#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

#include <iostream>
#include <tuple>
#include <fstream>
#include <sstream>
#include <set>
#include <unordered_map>

#include "commom.h"

using namespace std;

void parseObjectiveFunc(string line, vector<pair<string, mpq_class>>& objectiveFunction, unordered_map<string, mpq_class>& variables, bool& min);

void parseConstraints(fstream& f, vector<vector<pair<string, mpq_class>>>& constraintsMtx, unordered_map<string, mpq_class>& variables, int& auxVariables);

mtxData* inputParser(fstream& f);

#endif