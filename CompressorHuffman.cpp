//
// Created by fox0117 on 16.03.18.
//

#include <iostream>
#include <map>
#include "CompressorHuffman.h"
#include <fstream>
#include <iostream>

int CompressorHuffman::binSearch(int key)
{
    int left = -1, right = nodes.size();
    while (right - left > 1)
    {
        int middle = (right + left) / 2;
        if (nodes[middle]->key >= key)
            left = middle;
        else
            right = middle;
    }
    return right;
}

void CompressorHuffman::build()
{
    codes.resize(nodes.size());

    while (nodes.size() > 1)
    {
        item* right = nodes[nodes.size()-1];
        nodes.pop_back();
        item* left = nodes[nodes.size()-1];
        nodes.pop_back();
        item* middle = new item(right->key + left->key, right ,left);
        int position = binSearch(middle->key);
        nodes.insert(nodes.begin() + position, middle);
    }
}

void CompressorHuffman::calcCodes(std::string *code, item *it)
{
    if (it->id != -1)
    {
        codes[it->id] = (*code);
        delete it;
        return;
    }

    *code += '0';
    calcCodes(code, it->zero);
    (*code)[code->size()-1]='1';
    calcCodes(code, it->one);
    code->pop_back();
    delete it;
}

void CompressorHuffman::clear(item *it)
{
    if (it->id != -1)
    {
        delete it;
        return;
    }
    clear(it->zero);
    clear(it->one);
    delete it;
}

unsigned long long CompressorHuffman::compress(std::string filename)
{
    input.open(filename, std::ios::in | std::ios::binary);
    output.open(filename + ".haff", std::ios::out | std::ios::binary);
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
    codes.clear();
    return 0;
}

unsigned long long CompressorHuffman::decompress(std::string filename)
{
    input.open(filename + ".haff", std::ios::in | std::ios::binary);
    output.open(filename + ".unhaff", std::ios::out | std::ios::binary);
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
    item* currentItem = nodes[0];
    unsigned char ch;
    input.read((char*) &ch, sizeof(unsigned char));
    while (input.tellg() != endFile && !input.eof())
    {
        for (char i = 7; i > -1; --i)
        {
            if (currentItem->id != -1)
            {
                output.write(&(chars[currentItem->id]), sizeof(char));
                currentItem = nodes[0];
            }

            if (ch & (1 << i))
                currentItem = currentItem->one;
            else
                currentItem = currentItem->zero;
        }

        if (currentItem->id != -1)
        {
            output.write(&(chars[currentItem->id]), sizeof(char));
            currentItem = nodes[0];
        }

        input.read((char*) &ch, sizeof(unsigned char));
    }

    for (char i = 7; i > lastByteLength; --i)
    {
        if (currentItem->id != -1)
        {
            output.write(&(chars[currentItem->id]), sizeof(char));
            currentItem = nodes[0];
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
    clear(nodes[0]);
    nodes.clear();
    codes.clear();
    return 0;
}