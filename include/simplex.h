#include "../lib/gmp/gmpxx.h"
#include <iostream>
#include <vector>

using namespace std;

//Return the basic variable with the lower ratio basic / l
int lowerRatio(vector<vector<mpq_class>>& tableu, int l, vector<int> basic){
    bool allLessThanZero = 1;
    for(auto b : basic){
        if(tableu[])
    }
}

//Pivotize the row l in collum c
void pivotize(vector<vector<mpq_class>>& tableu, int l, int c){
    int rows = tableu.size(), collums = tableu[0].size();
    if(tableu[l][c] != 1){
        mpq_class val = tableu[l][c];
        for(int i = 0; i < collums; i++){
            tableu[l][i] = tableu[l][i]/val;
        }
    }

    for(int i = 0; i<rows; i++){
        if(tableu[i][c] != 0 && i != l){
            vector<mpq_class> aux = tableu[l];
            mpq_class val = tableu[i][c];
            for(int j = 0; j < collums; j++){
                aux[j] *= val;
                tableu[i][j] += aux[j];
            }
        }
    }
}

//Solve a linear programming problem using simplex. Take as parameters a standard form tableu and a list of basic variables
void simplexSolver(vector<vector<mpq_class>>& tableu, vector<int> basic){
    bool end = 0;
    int rows = tableu.size(), collums = tableu[0].size();
    while(!end){
        bool allGreaterThanZero = 1;
        for(int i = 2; i < collums - 1; i ++){
            if(tableu[0][i] < 0){
                allGreaterThanZero = 0;
                
            }
        }
    }
}