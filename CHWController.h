//
// Created by fox0117 on 25.03.18.
//

#ifndef KDZ_CHWCONTROLLER_H
#define KDZ_CHWCONTROLLER_H

#include "CompressorLZ77.h"
#include "CompressorHuffman.h"
#include "CompressorShannon.h"

class CHWController
{
    struct CompressorStatistic
    {
        double coefficient;
        unsigned long long compressingTime;
        unsigned long long decompressingTime;

        void put(double coefficient, unsigned long long compressingTime, unsigned long long decompressingTime)
        {
            this->coefficient=coefficient;
            this->compressingTime=compressingTime;
            this->decompressingTime=decompressingTime;
        }
    };

    std::string fileDir = "";
    unsigned long long frequency[36][256] = {{}, {}, {}, {}, {}};
    unsigned long long fileSizes[36] = {};
    CompressorStatistic compressorStatistic[36][5];
    Compressor* compressors[5];
    std::string comp_ext[5] = { ".haff", ".shan", ".lz775", ".lz7710", ".lz7720" };
    std::string decomp_ext[5] = { ".unhaff", ".unshan", ".unlz775", ".unlz7710", ".unlz7720" };
    std::string algo_name[5] = { "Huffman", "Shannon-Fano", "LZ775", "LZ7710", "LZ7720" };

    CHWController(const CHWController& other){} //Hiding copying constructor

    double estimateCoefficient(int algo, int file);

    bool validate(int fromAlgo, int toAlgo, int fromFile, int toFile, std::string exactFilename = "");
    void writeFrequenciesToCSV(int fileNum, std::string file);
    void estimateFrequency();
    double estimateEntropy(int fileNum);

public:
    void estimate(int fromAlgo, int toAlgo, int fromFile, int toFile, std::string exactFilename = "");

    CHWController(const std::string& dir)
    {
        fileDir = dir;
        compressors[0] = new CompressorHuffman();
        compressors[1] = new CompressorShannon();
        compressors[2] = new CompressorLZ77(5*1024, 4*1024);
        compressors[3] = new CompressorLZ77(10*1024, 8*1024);
        compressors[4] = new CompressorLZ77(20*1024, 16*1024);
    }

    ~CHWController() { for (int i = 0; i < 5; delete compressors[i++]); }

    void estimateTestFiles() { return estimate(0, 5, 1, 36); }

    void estimateExactFile(std::string file) { return estimate(0, 5, 1, 1, file); }

    bool validateTestFiles() { return validate(0, 5, 1, 36); }

    bool validateExactFile(std::string file) { return validate(0, 5, 1, 1, file); }

    bool estimateAndValidateTestFiles()
    {
        estimateTestFiles();
        return validateTestFiles();
    }

    bool estimateAndValidateExactFile(std::string file)
    {
        estimateExactFile(file);
        return validateExactFile(file);
    }

    void writeStatisticsToCSV(std::string file);
};


#endif //KDZ_CHWCONTROLLER_H
