//
// Created by fox0117 on 23.03.18.
//

#ifndef KDZ_COMPRESSOR_H
#define KDZ_COMPRESSOR_H

#include <string>
#include <fstream>

class Compressor {

protected:
    std::ifstream input;
    std::ofstream output;

    unsigned int* getFrequences()
    {
        unsigned int* freq = new unsigned int[256];
        unsigned char ch;
        input.read((char*) &ch, sizeof(unsigned char));
        while (!input.eof())
        {
            freq[(int)ch]++;
            input.read((char*) &ch, sizeof(unsigned char));
        }

        return freq;
    }

    void addChances(int* chars, unsigned int* freq)
    {
        int maxInd = 0;
        for (int j = 0; j < 256; ++j)
        {
            for (int i = 0; i < 256; ++i)
                if (freq[maxInd] < freq[i])
                    maxInd = i;

            if (freq[maxInd] > 0)
            {
                addChance(freq[maxInd]);
                chars[maxInd] = j;
                freq[maxInd] = 0;
            }
            else
                break;
        }
    }

    void addChances(char* chars, unsigned int* freq)
    {
        int maxInd = 0;
        for (int j = 0; j < 256; ++j)
        {
            for (int i = 0; i < 256; ++i)
                if (freq[maxInd] < freq[i])
                    maxInd = i;

            if (freq[maxInd] > 0)
            {
                addChance(freq[maxInd]);
                chars[j] = (char)maxInd;
                freq[maxInd] = 0;
            }
            else
                break;
        }
    }

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

    bool validate(std::string inFilename, std::string outFilename)
    {
        std::ifstream validator(inFilename, std::ios::in | std::ios::binary);
        input.open(outFilename, std::ios::in | std::ios::binary);
        validator.clear();
        input.clear();
        input.seekg(0, std::ios::end);
        validator.seekg(0, std::ios::end);
        if (input.tellg() != validator.tellg())
            return false;

        input.seekg(0, std::ios::beg);
        validator.seekg(0, std::ios::beg);
        char unhaff, val;
        input.read(&unhaff, sizeof(char));
        validator.read(&val, sizeof(char));
        while (!validator.eof() && !input.eof())
        {
            if (val != unhaff)
            {
                validator.close();
                input.close();
                return false;
            }

            input.read(&unhaff, sizeof(char));
            validator.read(&val, sizeof(char));
        }

        validator.close();
        input.close();
        return true;
    }
};


#endif //KDZ_COMPRESSOR_H
