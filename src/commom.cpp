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

void printResult(results* r){
    cout << "Status: " << r->status << endl;
    if(r->status == "otimo"){
        cout << "Objetivo: " << r->z.get_d() << endl;
        cout << "Solucao:\n";
        for(int i = 0; i < r->variables.size(); i++){
            cout << r->solution[i].get_d() << "x" << r->variables[i] << " ";
        }
        cout<< endl;
    }
    cout << "Certificado:\n";
    for(auto i : r->certificate){
         cout << i.get_d() << " ";
    }
    cout << endl;
}


void setSolution(results* r, vector<vector<mpq_class>> tableau, 
                vector<int> base, unordered_map<int, int> varMap){
    vector<int> var;
    vector<mpq_class> coef;
    int rows = tableau.size(), collums = tableau[0].size();
    for(auto b : base){
        for(int i = 1; i < rows; i++){
            if(tableau[i][b] == 1){
                mpq_class val = tableau[i][collums - 1];
                var.push_back(i);
                coef.push_back(val);
                break;
            }
        }
    }
    r->solution = coef;
    for(int i = 0; i < var.size(); i++){
        var[i] = varMap[var[i]];
    }
    r->variables = var;
}
