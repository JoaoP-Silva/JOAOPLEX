#ifndef SIMPLEX_H
#define SIMPLEX_H

#include <limits>

#include "commom.h"

using namespace std;

//Return the element with the lower ratio b / l. If all elements ratio are leq than 0, return -1.
int lowerRatio(vector<vector<mpq_class>>& tableu, int c);

//Pivotize the row l in collum c
void pivotize(vector<vector<mpq_class>>& tableu, int l, int c);

//Solve a linear programming problem using simplex. Take as parameters a standard form tableu and a list of basic variables
int simplexSolver(vector<vector<mpq_class>>& tableu, vector<int>& base);

#endif