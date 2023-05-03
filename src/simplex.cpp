#include "../include/simplex.h"

using namespace std;

//Return the element with the lower ratio b / l. If all elements ratio are leq than 0, return -1.
int lowerRatio(vector<vector<mpq_class>>& tableu, int c){
    int lastCollumn = tableu[0].size(), lines = tableu.size(), lowerLine = -1;
    int huge = numeric_limits<int>::max(); 
    mpq_class lower(huge);
    for(int i = 1; i < lines; i++){
        if(tableu[i][c] > 0 && tableu[i][lastCollumn]/tableu[i][c] < lower){
            lower = tableu[i][lastCollumn];
            lowerLine = i;
        }
    }
    return lowerLine;
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
                aux[j] *= -1 * val;
                tableu[i][j] += aux[j];
            }
        }
    }
}

//Solve a linear programming problem using simplex. Take as parameters a standard form tableu and a list of basic variables
int simplexSolver(vector<vector<mpq_class>>& tableu, vector<int>& base ){
    int rows = tableu.size(), collums = tableu[0].size();
    while(1){
        bool allGreaterThanZero = 1;
        for(int j = rows; j < collums - 1; j ++){
            if(tableu[0][j] < 0){
                allGreaterThanZero = 0;
                int r = lowerRatio(tableu, j);
                if(r != -1){
                    pivotize(tableu, r, j);
                    base[r - 1] = j;
                    break;
                }else{
                    return j;
                    break;
                }
            }
        }
        if(allGreaterThanZero){ return 0; }
    }
}