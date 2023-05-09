#include <fstream>
#include <iostream>
#include <unordered_set>

#include "../include/simplex.h"
#include "../include/inputParser.h"

int main(int argc, char* argv[]){
    fstream fin(argv[1], std::fstream::in);
    fstream fout(argv[2], std::fstream::out);

    //Parse the input, generating a matrix close to canonical form.
    if(!fin.is_open()){ cout << "File error\n"; return 0;}
    mtxData* data = inputParser(fin);
    //Struct to save results
    results* out = new results;


    //Whether objective function is empty
    if(data->objective.size() == 1){
        out->status = "otimo";
        out->certificate = vector<mpq_class>(1, 0);
        out->z = 0;
        printResult(out, fout);
        return 0;
    }
    //Whether have a objective function but no constraints
    else if(data->constraints.empty()){
        out->status = "ilimitado";
        out->certificate = vector<mpq_class>(1, -1);
        printResult(out, fout);
        return 0;
    }


    int rows = data->constraints.size() + 1, collumns = data->constraints[0].size() + rows - 1;

    //Setting the main tableau
    vector<vector<mpq_class>> mainTableu(rows, vector<mpq_class>(collumns, 0));
    for(int j = 0; j < rows - 1; j++){
        mainTableu[j + 1][j] = 1;
    }
    for(int j = rows - 1; j < collumns; j++){
        mainTableu[0][j] = -1 * data->objective[j - rows + 1];
    }
    for(int i = 1; i < rows; i++){
        for(int j = rows - 1; j < collumns; j++){
            mainTableu[i][j] = data->constraints[i - 1][j - rows + 1];
        }
    }

    //Checking if there are negatives values in b
    for(int i = 1; i < rows; i++){
        if(mainTableu[i][collumns - 1] < 0){
            for(int k = 0; k < collumns; k ++){
                mainTableu[i][k] *= -1;
            }
        }
    }
    //Setting the auxiliar tableau
    int auxRows = rows, auxCollumns = collumns + rows - 1;
    vector<vector<mpq_class>> auxTableu(auxRows, vector<mpq_class>(auxCollumns, 0));
    for(int i = 1; i < auxRows; i++){
        for(int j = 0; j < collumns - 1; j++){
            auxTableu[i][j] = mainTableu[i][j];
        }
    }

    {
        int j = collumns - 1;
        for(int i = 1; i < auxRows; i++){
            auxTableu[i][j] = 1;
            auxTableu[0][j] = 1;
            j++;
        }
    }
    for(int i = 0; i < auxRows; i++){
        auxTableu[i][auxCollumns - 1] = mainTableu[i][collumns - 1];
    }

    {
        int j = collumns - 1;
        for(int i = 1; i < auxRows; i++){
            pivotize(auxTableu, i, j);
            j++;
        }
    }
    vector<int> base;
    for(int i = collumns - 1; i < auxCollumns -1; i ++){
        base.push_back(i);
    }
    //Phase 1 simplex
    simplexSolver(auxTableu, base);

    //Checking feasibility
    if(auxTableu[0][auxCollumns -1] < 0){
        //Original tableau is infeasible
        out->status = "inviavel";
        for(int j = 0; j < data->numObjectiveVar; j++){
            out->certificate.push_back(mainTableu[0][j]);
        }
    }else{
        //Returning to the original tableau
        for(int j = 0; j < rows - 1; j ++){
            mainTableu[0][j] = auxTableu[0][j];
        }
        for(int i = 1; i < rows; i++){
            for(int j = 0; j < collumns - 1; j++){
                mainTableu[i][j] = auxTableu[i][j];
            }
        }
        for(int i = 0; i < rows; i++){
            mainTableu[i][collumns -1] = auxTableu[i][auxCollumns - 1];
        }
        for(auto b : base){
            for(int i = 1; i < rows; i++){
                if(mainTableu[i][b] == 1){
                    pivotize(mainTableu, i, b);
                    break;
                }
            }
        }

        //Phase two: solve it!
        int r = simplexSolver(mainTableu, base);

        //Whether its a minimization problem, multiply by -1 Z and the certificate.
        if(data->min){
            for(int j = 0; j < rows -1; j++){
                mainTableu[0][j] *= -1;
            }
            mainTableu[0][collumns - 1] *= -1;
        }

        if(r){
            out->status = "ilimitado";
            for(int i = 1; i < rows; i++){
                out->certificate.push_back(mainTableu[i][r]);
            }
        }else{
            out->status = "otimo";
            out->z = mainTableu[0][collumns - 1];
            for(int j = 0; j < data->numObjectiveVar; j++){
                out->certificate.push_back(mainTableu[0][j]);
            }
            setSolution(out, mainTableu, base, data);
        }
    }
    
    printResult(out, fout);
    return 0;
}