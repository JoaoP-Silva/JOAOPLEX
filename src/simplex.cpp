#include "../include/simplex.h"

using namespace std;

//Return the element with the lower ratio b / l. If all elements ratio are leq than 0, return -1.
int lowerRatio(vector<vector<mpq_class>>& tableau, int c){
    int lastCollumn = tableau[0].size() - 1, lines = tableau.size(), lowerLine = -1;
    int huge = numeric_limits<int>::max(); 
    mpq_class lower(huge);
    for(int i = 1; i < lines; i++){
        if(tableau[i][c] > 0 && tableau[i][lastCollumn]/tableau[i][c] < lower){
            lower = tableau[i][lastCollumn];
            lowerLine = i;
        }
    }
    return lowerLine;
}

//Pivotize the row l in collum c
void pivotize(vector<vector<mpq_class>>& tableau, int l, int c){
    int rows = tableau.size(), collums = tableau[0].size();
    if(tableau[l][c] != 1){
        mpq_class val = tableau[l][c];
        for(int i = 0; i < collums; i++){
            tableau[l][i] = tableau[l][i]/val;
        }
    }

    for(int i = 0; i<rows; i++){
        if(tableau[i][c] != 0 && i != l){
            vector<mpq_class> aux = tableau[l];
            mpq_class val = tableau[i][c];
            for(int j = 0; j < collums; j++){
                aux[j] *= -1 * val;
                tableau[i][j] += aux[j];
            }
        }
    }
}

//Solve a linear programming problem using simplex. Take as parameters a standard form tableau and a list of basic variables
int simplexSolver(vector<vector<mpq_class>>& tableau, vector<int>& base ){
    int rows = tableau.size(), collums = tableau[0].size();
    //Before start, check if all b >= 0
    for(int i = 1; i < rows; i++){
        if(tableau[i][collums - 1] < 0){
            for(int k = 0; k < collums; k++){
                tableau[i][k] = -1 * tableau[i][k];
            }
        }
    }
    while(1){
        bool allGreaterThanZero = 1;
        for(int j = rows - 1; j < collums - 1; j ++){
            if(tableau[0][j] < 0){
                allGreaterThanZero = 0;
                int r = lowerRatio(tableau, j);
                if(r != -1){
                    pivotize(tableau, r, j);
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