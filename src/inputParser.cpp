#include "../include/inputParser.h"

using namespace std;

void parseObjectiveFunc(string line, vector<pair<int, mpq_class>>& objectiveFunction, set<int>& variables, bool& min){
    string word;
    stringstream s(line);
    s >> word;
    min = 0;
    if(word != "MAX"){
        min = 1;
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
            else if(f == '-'){ minusFlag = 1; }
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
    if(min){
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
        if(line.empty()){continue;}
        bool minusFlag = 0, geqZeroFlag = 0;
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
                }else if(f == '-'){ minusFlag = 1; }
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
                    s >> word;
                    if(word == "0"){
                        geqZeroFlag = 1;
                        break;
                    }
                    mpq_class coef = -1;
                    constraint.push_back(make_pair(-1, coef));
                    auxVariables++;
                    coef = mpq_class(word);
                    constraint.push_back(make_pair(0, coef));
                    s >> word;
                }
                else if(f == '<'){
                    mpq_class coef = 1;
                    constraint.push_back(make_pair(-1, coef));
                    auxVariables++;
                    s >> word;
                    coef = mpq_class(word);
                    constraint.push_back(make_pair(0, coef));
                    s >> word;
                }
                else if(f == '='){
                    s >> word;
                    mpq_class coef(word);
                    constraint.push_back(make_pair(0, coef));
                    s >> word;
                }else{
                    cout << "Bad input\n";
                }
            }
        }
        
        if(!geqZeroFlag){ constraintsMtx.push_back(constraint); }
    }
}

mtxData* inputParser(fstream& f){
    //A vector with pairs (index, value) of variables != 0 in the objective function
    vector<pair<int, mpq_class>> objectiveFunction;
    vector<vector<pair<int, mpq_class>>> constraintsMtx;
    mtxData* data = new mtxData;
    string line;
    bool min;
    set<int> variables;
    int auxVariables;
    unordered_map<int , int> idxMap;
    getline(f, line);
    parseObjectiveFunc(line, objectiveFunction, variables, min);
    int _numObjectiveVar = variables.size();
    data->numObjectiveVar = _numObjectiveVar;
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
    //printObjectiveFunct(data);
    //printConstraints(data);
    data->min = min;
    return data;
}