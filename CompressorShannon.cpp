//
// Created by fox0117 on 16.03.18.
//

#include "CompressorShannon.h"

int CompressorShannon::getMid(int start, int end)
{
    double median = (sum[end] - sum[start]) / 2.0;
    double prevDiff = sum[end], nextDiff;
    int midPosition = start;
    while (midPosition < end &&
           (nextDiff = abs(sum[midPosition+1] - sum[start] - median)) < prevDiff)
    {
        prevDiff = nextDiff;
        ++midPosition;
    }

    return midPosition-1;
}

void CompressorShannon::split(int start, int end, std::string *code)
{
    if (end - start > 1)
    {
        int mid = getMid(start, end);
        *code += '0';
        split(start, mid+1, code);
        (*code)[code->size()-1]='1';
        split(mid+1, end, code);
        code->pop_back();
    }
    else
        codes[start] = *code;
}

void CompressorShannon::build()
{
    codes.resize(probabilities.size());
    sum.resize(probabilities.size()+1);
    sum[0]=0;
    for (int i = 1; i < sum.size(); sum[i] = sum[i-1]+probabilities[i-1], ++i);
    std::string code = "";
    split(0, probabilities.size(), &code);
}

unsigned long long CompressorShannon::compress(std::string filename)
{
    /*input.open(filename, std::ios::in | std::ios::binary);
    output.open(filename + ".shan", std::ios::out | std::ios::binary);
    input.clear();
    output.clear();

    unsigned int freq[256] = {};
    int chars[256];
    unsigned char ch;
    input.read((char*) &ch, sizeof(unsigned char));
    while (!input.eof())
    {
        freq[(int)ch]++;
        input.read((char*) &ch, sizeof(unsigned char));
    }

    for (int i = 0; i < 256; output.write((char *) &freq[i], sizeof(unsigned int)), ++i);

    int maxInd = 0;
    for (int j = 0; j < 256; ++j)
    {
        for (int i = 0; i < 256; ++i)
            if (freq[maxInd] < freq[i])
                maxInd = i;

        if (freq[maxInd] > 0)
        {
            nodes.push_back(new item(freq[maxInd], nullptr, nullptr, nodes.size()));
            chars[maxInd] = j;
            freq[maxInd] = 0;
        }
        else
            break;
    }

    build();
    std::string code = "";
    calcCodes(&code, nodes[0]);

    char pos = 7;
    unsigned char outChar = 0;
    code = "";
    ch = 0;
    input.clear();
    input.seekg(0, std::ios::beg);
    input.read((char*) &ch, sizeof(unsigned char));
    while (!input.eof())
    {
        code = codes[chars[(int) ch]];
        for (int i = 0; i < code.length(); ++i)
        {
            if (code[i] == '1')
                outChar = outChar | (1 << pos);

            if (pos == 0)
            {
                output.write((char*) &outChar, sizeof(unsigned char));
                pos = 8;
                outChar = 0;
            }

            --pos;
        }

        input.read((char*) &ch, sizeof(unsigned char));
    }

    output.write((char*) &outChar, sizeof(unsigned char));
    output.write(&pos, sizeof(char));
    input.close();
    output.close();
    nodes.clear();
    codes.clear();*/
    return 0;
}

unsigned long long CompressorShannon::decompress(std::string filename)
{

}