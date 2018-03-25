//
// Created by fox0117 on 23.03.18.
//

#ifndef KDZ_COMPRESSOR_H
#define KDZ_COMPRESSOR_H

#include <string>
#include <fstream>
#include <iostream>

class Compressor {

protected:
    std::ifstream input;
    std::ofstream output;

    unsigned int* getFrequences();

    void addChances(int* chars, unsigned int* freq);

    void addChances(char* chars, unsigned int* freq);

    virtual void addChance(unsigned int oneFreq)
    {
        return;
    }

public:
    virtual unsigned long long compress(std::string filename)
    {
        return 0;
    }

    virtual unsigned long long decompress(std::string filename)
    {
        return 0;
    }

    bool validate(std::string inFilename, std::string outFilename);
};


#endif //KDZ_COMPRESSOR_H
