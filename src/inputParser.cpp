#include "../include/inputParser.h"

using namespace std;

void parseObjectiveFunc(string line, vector<pair<string, mpq_class>>& objectiveFunction, 
    unordered_map<string, mpq_class>& variables, bool& min){
    string word;
    stringstream s(line);
    s >> word;
    min = 0;
    if(word != "MAX"){
        min = 1;
    }
    bool minusFlag = 0;
    mpq_class coef = 1;
    //Parsing the objective function
    while(s){
        s >> word;
        char f = word.front();
        if(isdigit(f)){
            coef = f;
        }
        else if(f == '*' || f == '+'){ continue; }
        else if(f == '('){
            string frac;
             s >> word;
            while(f != ')'){
                frac += word;
                s >> word;f = word.front();
            }
            coef = frac;
        }
        else if (f == '-'){
            minusFlag = 1;
        }else{
            if(minusFlag){ coef *= -1; }
            variables.insert(make_pair(word, coef));
            objectiveFunction.push_back(make_pair(word, coef));
        }
    }
    if(min){
        for(int i =0; i<objectiveFunction.size(); i++){
            objectiveFunction[i].second *= -1;
        }
    }
}

void parseConstraints(fstream& f, vector<vector<pair<string, mpq_class>>>& constraintsMtx, 
    unordered_map<string, mpq_class>& variables, int& auxVariables){
    string line, word;
    auxVariables = 0;
    while(getline(f, line)){
        vector<pair<string, mpq_class>> constraint;
        set<string> varInThisConstraint;
        mpq_class coef = 1;
        bool minusFlag = 0, geqZeroFlag = 0;
        string word;
        stringstream s(line);
        while(s){
            s >> word;
            char f = word.front();
            if(isdigit(f)){
                coef = word;
            }
            else if(f == '*' || f == '+'){ continue; }
            else if(f == '('){
                string frac;
                while(f != ')'){
                    s >> word;f = word.front();
                    frac += word;
                }
                coef = frac;
            }
            else if (f == '-'){
                minusFlag = 1;
            }
            else if(f == '>'){
                s >> word;
                if(stoi(word) == 0 && varInThisConstraint.size() == 1){
                    geqZeroFlag = 1;
                    break;
                }
                mpq_class coef = -1;
                constraint.push_back(make_pair("aux", coef));
                auxVariables++;
                coef = mpq_class(word);
                constraint.push_back(make_pair("res", coef));
                s >> word;
            }
            else if(f == '<'){
                s >> word; 
                mpq_class coef = 1;
                constraint.push_back(make_pair("aux", coef));
                auxVariables++;
                coef = mpq_class(word);
                constraint.push_back(make_pair("res", coef));
                s >> word;
            }
            else if(f == '='){
                s >> word;
                mpq_class coef(word);
                constraint.push_back(make_pair("res", coef));
                s >> word;
            }else{
                if(minusFlag){ coef *= -1; }
                variables.insert(make_pair(word, coef));
                varInThisConstraint.insert(word);
                constraint.push_back(make_pair(word, coef));
            }
        }
        if(!geqZeroFlag){ constraintsMtx.push_back(constraint); }
    }
}

mtxData* inputParser(fstream& f){
    //A vector with pairs (index, value) of variables != 0 in the objective function
    vector<pair<string, mpq_class>> objectiveFunction;
    vector<vector<pair<string, mpq_class>>> constraintsMtx;
    mtxData* data = new mtxData;
    string line;
    bool min;
    unordered_map<string, mpq_class> variables;
    int auxVariables;
    unordered_map<string , int> idxMap;
    getline(f, line);
    parseObjectiveFunc(line, objectiveFunction, variables, min);
    int _numObjectiveVar = variables.size();
    data->numObjectiveVar = _numObjectiveVar;

    parseConstraints(f, constraintsMtx, variables, auxVariables);
    
    //cout << "variables.size() = " << variables.size() << endl;
    for(int mapIdx = 0; mapIdx < variables.size(); mapIdx++){
        string thisVar = objectiveFunction[mapIdx].first;
        idxMap.insert(make_pair(thisVar, mapIdx));
        data->variablesMap.insert(make_pair(mapIdx, thisVar));
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
        string k= objectiveFunction[i].first;
        int idx = idxMap[k];
        data->objective[idx] = objectiveFunction[i].second;
    }

    //Assingning values to constraints
    for(int i =0; i<constraintsMtx.size(); i++){
        for(int j = 0; j<constraintsMtx[i].size(); j++){
            auto var = constraintsMtx[i][j];
            int idx;
            string k = var.first;

            if(k == "aux"){
                thisAux++;
                idx = thisAux;
                data->constraints[i][idx] = var.second;
            }
            else if(k == "res"){
                idx = data->constraints[i].size() - 1;
                data->constraints[i][idx] = var.second;
            }else{
                idx = idxMap[k];
                data->constraints[i][idx] = var.second;
            }
        }
    }
    //printObjectiveFunct(data);
    //printConstraints(data);
    data->min = min;
    return data;
}