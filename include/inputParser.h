#include "../lib/gmp/gmpxx.h"
#include <iostream>
#include <tuple>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <set>

using namespace std;

struct mtxData{
    vector<mpq_class> objective;
    vector<vector<mpq_class>> constraints;
    unordered_map<int, int> variablesMap;
};

void parseObjectiveFunc(string line, vector<pair<int, mpq_class>>& objectiveFunction, set<int>& variables){
    string word;
    stringstream s(line);
    s >> word;
    bool max = 1;
    if(word != "MAX"){
        max = 0;
    }
    bool minusFlag = 0;
    //Parsing the objective function
    while(s){
        s >> word;
        char b = word.back();
        if(isdigit(b)){
            mpq_class coef = 1;
            int delimiter = 1;
            char f = word.front();
            if(isdigit(f)){ 
                for(int i=0; i<word.size(); i++){
                    if(!isdigit(word[i]) && word[i] !=  '/') {break;}
                    delimiter = i+1;
                }
                coef = mpq_class(word.substr(0, delimiter));
            }
            if(minusFlag){ coef *= -1; }
            int figures = 0;
                for(int i=word.size() - 1; i>=0; i--){
                        if(!isdigit(word[i])) {break;}
                        delimiter = i;
                        figures++;
                }
            string sub = word.substr(delimiter, figures);
            int idx = stoi(sub);
            pair<int, mpq_class> p = make_pair(idx, coef);
            objectiveFunction.push_back(p);
            variables.insert(idx);
            //cout << "Word = " << word << "Has coefficient = " << coef.get_d() << " and idx = " << idx << endl;
        }
        else if(b == '+'){minusFlag = 0;}
        else if(b == '-'){ minusFlag = 1;}
    }
    if(!max){
        for(int i =0; i<objectiveFunction.size(); i++){
            objectiveFunction[i].second *= -1;
        }
    }
}

void parseConstraints(fstream& f, vector<vector<pair<int, mpq_class>>>& constraintsMtx, set<int> variables, int& auxVariables){
    string line, word;
    auxVariables = 0;
    while(getline(f, line)){
        stringstream s(line);
        bool minusFlag = 0, leqFlag = 0;
        vector<pair<int, mpq_class>> constraint;
        while(s){
            s >> word;
            char b = word.back();
            if(isdigit(b)){
                mpq_class coef = 1;
                int delimiter = 1;
                char f = word.front();
                if(isdigit(f)){ 
                    for(int i=0; i<word.size(); i++){
                        if(!isdigit(word[i]) && word[i] !=  '/') {break;}
                        delimiter = i+1;
                    }
                    coef = mpq_class(word.substr(0, delimiter));
                }
                if(minusFlag){ coef *= -1; }
                int figures = 0;
                for(int i=word.size() - 1; i>=0; i--){
                        if(!isdigit(word[i])) {break;}
                        delimiter = i;
                        figures++;
                }
                string sub = word.substr(delimiter, figures);
                int idx = stoi(sub);
                pair<int, mpq_class> p = make_pair(idx, coef);
                constraint.push_back(p);
                variables.insert(idx);
            }
            else if(b == '+'){minusFlag = 0;}
            else if(b == '-'){ minusFlag = 1;}
            else if(b == '='){
                char f = word.front();
                if(f == '>'){
                    mpq_class coef = -1;
                    constraint.push_back(make_pair(-1, coef));
                    auxVariables++;
                    s >> word;
                    coef = mpq_class(word);
                    constraint.push_back(make_pair(0, coef));
                }
                else if(f == '<'){
                    mpq_class coef = 1;
                    constraint.push_back(make_pair(-1, coef));
                    auxVariables++;
                    s >> word;
                    leqFlag = 1;
                    coef = mpq_class(word);
                    constraint.push_back(make_pair(0, coef));
                }
                else if(f == '='){
                    s >> word;
                    mpq_class coef(word);
                    constraint.push_back(make_pair(0, coef));
                }else{
                    cout << "Bad input\n";
                }
            }
        }
        
        constraintsMtx.push_back(constraint);
        if(leqFlag){
            vector<pair<int, mpq_class>> newConstraint;
            mpq_class coef = 1;
            newConstraint.push_back(make_pair(-2, coef));
            newConstraint.push_back(make_pair(-1, coef));
            auxVariables++;
            coef = 0;
            newConstraint.push_back(make_pair(0, coef));
            constraintsMtx.push_back(newConstraint);
        }
        
    }
}

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

mtxData* inputParser(fstream& f){
    //A vector with pairs (index, value) of variables != 0 in the objective function
    vector<pair<int, mpq_class>> objectiveFunction;
    vector<vector<pair<int, mpq_class>>> constraintsMtx;
    mtxData* data = new mtxData;
    string line;
    set<int> variables;
    int auxVariables;
    unordered_map<int , int> idxMap;
    getline(f, line);
    parseObjectiveFunc(line, objectiveFunction, variables);
    parseConstraints(f, constraintsMtx, variables, auxVariables);
    int mapIdx = 0;
    //cout << "variables.size() = " << variables.size() << endl;
    for(auto it : variables){
        idxMap.insert(make_pair(it, mapIdx));
        data->variablesMap.insert(make_pair(mapIdx, it));
        mapIdx++;
    }
    int numVariables = variables.size() + auxVariables;
    //cout << "constraintsMtx.size() = " << constraintsMtx.size() << endl;
    for(int i = 0; i<constraintsMtx.size(); i++){
        vector<mpq_class> vec;
        for(int j = 0; j<numVariables+1; j++){
            vec.push_back(mpq_class(0));
        }
        data->constraints.push_back(vec);
    }

    int thisAux = variables.size() - 1;

    //Assigning values to objective function
    for(int j = 0; j<numVariables+1; j++){
            data->objective.push_back(mpq_class(0));
    }
    for(int i = 0; i<objectiveFunction.size(); i++){
        int k= objectiveFunction[i].first;
        int idx = idxMap[k];
        data->objective[idx] = objectiveFunction[i].second;
    }

    //Assingning values to constraints
    for(int i =0; i<constraintsMtx.size(); i++){
        for(int j = 0; j<constraintsMtx[i].size(); j++){
            auto var = constraintsMtx[i][j];
            int k = var.first, idx;
            //If k is a variable from the original problem
            if(k > 0){
                idx = idxMap[k];
                data->constraints[i][idx] = var.second;
            }
            else if(k == -1){
                thisAux++;
                idx = thisAux;
                data->constraints[i][idx] = var.second;
            }
            else if(k == -2){
                idx = thisAux;
                data->constraints[i][idx] = var.second;
            }
            else if(k == 0){
                idx = data->constraints[i].size() - 1;
                data->constraints[i][idx] = var.second;
            }
        }
    }
    printObjectiveFunct(data);
    printConstraints(data);

    return data;
}