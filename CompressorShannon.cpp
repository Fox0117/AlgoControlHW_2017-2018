//
// Created by fox0117 on 16.03.18.
//

#include <climits>
#include <cmath>
#include "CompressorShannon.h"

int CompressorShannon::getMid(int start, int end)
{
    long long prevFreq = 0, fullProb = 0;
    int found = start;
    for(int i = start; i < end; fullProb += probabilities[i], ++i);

    double midPosition = (double)fullProb / 2, difference = LONG_MAX;
    for(int i = start; i < end; ++i){
        prevFreq += probabilities[i];
        double nextDiff = std::fabs(prevFreq - midPosition);
        if(difference < nextDiff)
            return found;

        if(difference >= nextDiff){
            difference = nextDiff;
            found = i;
        }
    }

    return found;
}

void CompressorShannon::split(int start, int end, std::string *code)
{
    if (end - start > 1)
    {
        int mid = getMid(start, end);
        *code += '0';
        if (mid + 1 < end)
            split(start, mid+1, code);
        else
            split(start, mid, code);

        (*code)[code->size()-1]='1';

        if (mid + 1 > start)
            split(mid+1, end, code);
        else
            split(mid, end, code);

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

void CompressorShannon::clear(Node *curr)
{
    if (curr->zero != nullptr)
        clear(curr->zero);

    if (curr->one != nullptr)
        clear(curr->one);

    delete curr;
}

void CompressorShannon::buildTree(Node *currentNode, int codeNum, int ind)
{
    if (codeNum >= codes.size())
        return;

    if (ind >= codes[codeNum].length())
    {
        currentNode->id = codeNum;
        return;
    }

    if (codes[codeNum][ind] == '0')
    {
        if (currentNode->zero == nullptr)
            currentNode->zero = new Node {-1, nullptr, nullptr};

        buildTree(currentNode->zero, codeNum, ind+1);
    }
    else
    {
        if (currentNode->one == nullptr)
            currentNode->one = new Node {-1, nullptr, nullptr};

        buildTree(currentNode->one, codeNum, ind+1);
    }

    if (currentNode == root && codeNum < codes.size())
        buildTree(root, codeNum + 1);
}

unsigned long long CompressorShannon::compress(std::string filename)
{
    input.open(filename, std::ios::in | std::ios::binary);
    output.open(filename + ".shan", std::ios::out | std::ios::binary);
    input.clear();
    output.clear();

    unsigned int* freq;
    int chars[256];
    unsigned char ch;
    freq = getFrequences();

    for (int i = 0; i < 256; output.write((char *) &freq[i], sizeof(unsigned int)), ++i);

    addChances(chars, freq);

    build();
    std::string code = "";
    char pos = 7;
    unsigned char outChar = 0;
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
    probabilities.clear();
    sum.clear();
    codes.clear();
    return 0;
}

unsigned long long CompressorShannon::decompress(std::string filename)
{
    input.open(filename + ".shan", std::ios::in | std::ios::binary);
    output.open(filename + ".unshan", std::ios::out | std::ios::binary);
    input.clear();
    output.clear();

    char lastByteLength;
    input.seekg(-sizeof(char), std::ios::end);
    std::streampos endFile = input.tellg();
    input.read(&lastByteLength, sizeof(char));
    input.seekg(0);
    unsigned int freq[256] = {};
    char chars[256];
    for (int i = 0; i < 256; input.read((char*) &(freq[i++]), sizeof(unsigned int)));

    addChances(chars, freq);

    build();
    std::string code = "";

    root = new Node {-1, nullptr, nullptr};
    buildTree(root, 0);

    Node* currentItem = root;
    unsigned char ch;
    input.read((char*) &ch, sizeof(unsigned char));
    while (input.tellg() != endFile && !input.eof())
    {
        for (char i = 7; i > -1; --i)
        {
            if (currentItem->id != -1)
            {
                output.write(&(chars[currentItem->id]), sizeof(char));
                currentItem = root;
            }

            if (ch & (1 << i))
                currentItem = currentItem->one;
            else
                currentItem = currentItem->zero;
        }

        if (currentItem->id != -1)
        {
            output.write(&(chars[currentItem->id]), sizeof(char));
            currentItem = root;
        }

        input.read((char*) &ch, sizeof(unsigned char));
    }

    for (char i = 7; i > lastByteLength; --i)
    {
        if (currentItem->id != -1)
        {
            output.write(&(chars[currentItem->id]), sizeof(char));
            currentItem = root;
        }

        if (ch & (1 << i))
            currentItem = currentItem->one;
        else
            currentItem = currentItem->zero;
    }

    if (currentItem->id != -1)
        output.write(&(chars[currentItem->id]), sizeof(char));

    input.close();
    output.close();
    clear(root);
    sum.clear();
    probabilities.clear();
    codes.clear();
    return 0;
}