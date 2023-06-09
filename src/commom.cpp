#include "../include/commom.h"

using namespace std;

//Commom functions
void printVector(vector<mpq_class>& v){
    for(int i =0; i<v.size(); i++){
        cout<< v[i].get_d() << " ";
    }
    cout << endl;
}

void printObjectiveFunct(mtxData* d){
    cout<< "Objective function :" << endl;
    vector<mpq_class> v = d->objective;
    printVector(v);
}

void printConstraints(mtxData* d){
    vector<vector<mpq_class>> m = d->constraints;
    cout << "Constraints :" << endl;
    for(int i =0; i<m.size(); i++){
        vector<mpq_class> v = m[i];
        printVector(v);
        cout << endl;
    }
}

void printTableu(vector<vector<mpq_class>> m){
    cout << "Tableu:" << endl;
    for(int i =0; i<m.size(); i++){
        vector<mpq_class> v = m[i];
        printVector(v);
        cout << endl;
    }
}

void printBase(vector<int> v){
    cout << "Base:\n";
    for(int i = 0; i < v.size(); i++){
        cout << v[i] << " ";
    }
    cout << endl;
}

void printResult(results* r, fstream& outFile){
    outFile << "Status: " << r->status << endl;
    if(r->status == "otimo"){
        outFile << "Objetivo: " << r->z.get_d() << endl;
        outFile << "Solucao:\n";
        for(int i = 0; i < r->variables.size(); i++){
            outFile << r->variables[i] << " = " << r->solution[i].get_d() << " ";
        }
        outFile << endl;
    }
    outFile << "Certificado:\n";
    for(auto i : r->certificate){
        outFile << i.get_d() << " ";
    }
    outFile << endl;
}


void setSolution(results* r, vector<vector<mpq_class>> tableau, 
                vector<int> base, mtxData* d){

    vector<mpq_class> coef(d->numObjectiveVar, 0);
    vector<string> var(d->numObjectiveVar, "0");
    for(int i = 0; i <var.size(); i++){ var[i] = i; }

    int rows = tableau.size(), collums = tableau[0].size();
    
    for(auto b : base){
        for(int i = 1; i < rows; i++){
            if(tableau[i][b] == 1 && ((b - rows + 1) < d->numObjectiveVar)){
                mpq_class val = tableau[i][collums - 1];
                coef[b - rows + 1] = val;
                break;
            }
        }
    }
    r->solution = coef;
    for(int i = 0; i < var.size(); i++){
        var[i] = d->variablesMap[i];
    }
    r->variables = var;
}
