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

