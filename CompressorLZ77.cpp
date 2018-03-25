//
// Created by fox0117 on 16.03.18.
//

#include "CompressorLZ77.h"

CompressorLZ77::Node CompressorLZ77::getMaxPrefixNode(int histIndStart, int prevIndStart, int prevIndEnd, int fileSize)
{
    int size = 2*prevIndEnd - histIndStart - prevIndStart + 1;
    int prevLen = prevIndEnd - prevIndStart;
    int* prefix = new int[size];
    prefix[0] = 0;
    prefix[prevLen] = 0;
    for (int i = 1; i < prevLen; ++i)
    {
        int j = prefix[i-1];
        for(; j > 0 && buffer[j + prevIndStart] != buffer[i + prevIndStart]; j = prefix[j-1]);

        if (buffer[j + prevIndStart] == buffer[i + prevIndStart])
            ++j;

        prefix[i] = j;
    }

    for (int i = prevLen + 1; i < size; ++i)
    {
        int j = prefix[i-1];
        for(; j > 0 && (buffer[j + prevIndStart] != buffer[i - (prevLen + 1) + histIndStart] || j+prevIndStart == prevIndEnd);
              j = prefix[j-1]);

        if (buffer[j + prevIndStart] == buffer[i - (prevLen + 1) + histIndStart])
            ++j;

        prefix[i] = j;
    }

    Node result(0, 0, buffer[prevIndStart]);
    int maxPrefInd = 0;
    for (int i = prevLen + 1; i < size; ++i)
        if (i - prefix[i] + 1 > prevLen && i - prefix[i] < prevIndEnd - histIndStart
            && prefix[maxPrefInd] < prefix[i] && prefix[i] + prevIndStart < fileSize)
            maxPrefInd = i;

    if (maxPrefInd > 0)
    {
        result.len = prefix[maxPrefInd];
        result.offs = prevIndStart - (maxPrefInd - prefix[maxPrefInd] -prevLen) - histIndStart;
        result.ch = buffer[prefix[maxPrefInd] + prevIndStart];
    }
    delete[] prefix;
    return result;
}

unsigned long long CompressorLZ77::compress(std::string filename)
{
    input.open(filename, std::ios::in | std::ios::binary);
    output.open(filename + ".lz77" + std::to_string(buffSize/1024), std::ios::out | std::ios::binary);

    input.seekg(0, std::ios::end);
    int fileSize = input.tellg();
    input.seekg(0, std::ios::beg);
    char* tmpBuffer = new char[buffSize*2];
    input.read(tmpBuffer, buffSize*2);
    buffer = std::string(tmpBuffer, buffSize*2);

    int prevInd = 0;
    do
    {
        if (prevInd - histBuff >= buffSize)
        {
            buffer = buffer.substr(buffSize);
            input.read(tmpBuffer, buffSize);
            buffer.append(tmpBuffer, buffSize);
            fileSize -= buffSize;
            prevInd -= buffSize;
        }

        Node result = getMaxPrefixNode(std::max(0, (prevInd - histBuff)),
                                       prevInd, std::min(prevInd + prevBuff, fileSize), fileSize);

        prevInd += result.len + 1;
        result.print(output);
    }
    while (prevInd < fileSize);

    delete[] tmpBuffer;
    input.close();
    output.close();

    return 0;
}

unsigned long long CompressorLZ77::decompress(std::string filename)
{
    input.open(filename + ".lz77" + std::to_string(buffSize / 1024), std::ios::in | std::ios::binary);
    output.open(filename + ".unlz77" + std::to_string(buffSize / 1024), std::ios::out | std::ios::binary);

    readBytes = 0;
    int prevInd = 0;
    unsigned short offs, len;
    char ch;
    buffer.clear();
    buffer.resize(buffSize*2, ' ');
    input.read((char*) &offs, sizeof(unsigned short));
    input.read((char*) &len, sizeof(unsigned short));
    input.read(&ch, sizeof(char));
    while (!input.eof())
    {
        for (int j = 0; j < len; ++j)
        {
            if (readBytes >= buffSize*2)
            {
                output.write(buffer.c_str(), buffSize);
                buffer = buffer.substr(buffSize);
                buffer.resize(buffSize*2);
                prevInd -= buffSize;
                readBytes = buffSize;
            }
            buffer[readBytes++] = buffer[prevInd - offs + j];
        }

        if (readBytes >= buffSize*2)
        {
            output.write(buffer.c_str(), buffSize);
            buffer = buffer.substr(buffSize);
            buffer.resize(buffSize*2);
            prevInd -= buffSize;
            readBytes = buffSize;
        }

        buffer[readBytes++] = ch;
        prevInd +=len + 1;

        input.read((char*) &offs, sizeof(unsigned short));
        input.read((char*) &len, sizeof(unsigned short));
        input.read(&ch, sizeof(char));
    }

    output.write(buffer.c_str(), readBytes);
    input.close();
    output.close();

    return 0;
}