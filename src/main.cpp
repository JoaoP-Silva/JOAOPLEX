#include <fstream>
#include <iostream>

#include "../include/simplex.h"

#include "../lib/gmp/gmpxx.h"

int main(int argc, char* argv[]){
    //fstream f("input.txt", std::fstream::in);
    fstream f(argv[1], std::fstream::in);

    //Parse the input, generating a matrix close to canonical form.
    mtxData* data = inputParser(f);
    int rows = data->constraints.size() + 1, collumns = data->constraints[0].size() + rows - 1;
    vector<vector<mpq_class>> mainTableu(rows, vector<mpq_class>(collumns, 0));
    for(int j = 0; j < rows - 1; j++){
        mainTableu[j + 1][j] = 1;
    }
    for(int j = rows - 1; j < collumns; j++){
        mainTableu[0][j] = data->objective[j - rows + 1];
    }
    for(int i = 1; i < rows; i++){
        for(int j = rows - 1; j < collumns; j++){
            mainTableu[i][j] = data->constraints[i - 1][j - rows + 1];
        }
    }
    printTableu(mainTableu);
    return 0;
}