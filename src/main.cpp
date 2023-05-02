#include <fstream>
#include "../include/inputParser.h"

int main(int argc, char* argv[]){
    //fstream f("input.txt", std::fstream::in);
    fstream f( argv[1], std::fstream::in);
    mtxData* data = inputParser(f);
    return 0;
}