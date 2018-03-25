//
// Created by fox0117 on 16.03.18.
//

#ifndef KDZ_ZIPSHANON_H
#define KDZ_ZIPSHANON_H

#include "Compressor.h"

class CompressorShannon : Compressor
{

public:
    unsigned long long compress(std::string filename) override;

    unsigned long long decompress(std::string filename) override;
};


#endif //KDZ_ZIPSHANON_H
