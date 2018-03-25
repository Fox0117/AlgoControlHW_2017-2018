//
// Created by fox0117 on 16.03.18.
//

#ifndef KDZ_COMPRESSORLZ77_H
#define KDZ_COMPRESSORLZ77_H

#include "Compressor.h"
#include <vector>
#include <iostream>

class CompressorLZ77 : public Compressor
{
    class Node
    {
    public:
        unsigned short offs;
        unsigned short len;
        char ch;

        Node(int o, int l, char c) : offs(o), len(l), ch(c) {};

        Node() {};

        void print(std::ofstream& output)
        {
            output.write((char*) &offs, sizeof(unsigned short));
            output.write((char*) &len, sizeof(unsigned short));
            output.write(&ch, sizeof(char));

            //std::cout << offs << ' ' << len << ' ' << ch << std::endl;
        }
    };

    Node getMaxPrefixNode(int histIndStart, int prevIndStart, int prevIndEnd, int fileSize);

    int prevBuff, histBuff, buffSize, readBytes;
    std::string buffer;

public:
    CompressorLZ77(int bufferSize, int histBufferSize)
    {
        buffSize = bufferSize;
        prevBuff = bufferSize - histBufferSize;
        histBuff = histBufferSize;
    }

    ~CompressorLZ77()
    {
    }

    unsigned long long compress(std::string filename) override;

    unsigned long long decompress(std::string filename) override;
};


#endif //KDZ_COMPRESSORLZ77_H
