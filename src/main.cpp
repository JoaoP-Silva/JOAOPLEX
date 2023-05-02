#include <fstream>
#include <iostream>

#include "../include/inputParser.h"
#include "../lib/gmp/gmpxx.h"

int main(int argc, char* argv[]){
    //fstream f("input.txt", std::fstream::in);
    fstream f(argv[1], std::fstream::in);

    //Parse the input, generating a matrix close to canonical form.
    mtxData* data = inputParser(f);


    return 0;
}