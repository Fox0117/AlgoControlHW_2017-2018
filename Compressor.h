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
