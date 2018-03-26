//
// Created by fox0117 on 23.03.18.
//

#include "CHWController.h"
#include <fstream>
#include <cmath>

double CHWController::estimateCoefficient(int algo, int file)
{
    std::string app = "";
    if (file < 10)
        app += "0";

    app+= std::to_string(file);
    std::ifstream input(fileDir + app + comp_ext[algo], std::ios::in | std::ios::binary);
    input.seekg(0, std::ios::end);
    unsigned long size = input.tellg();
    input.close();

    return (double)size / fileSizes[file-1];
}

void CHWController::estimateFrequency()
{
    std::ifstream input;

    for(int i = 1; i <= 36; ++i)
    {
        std::string app = "";
        if (i < 10)
            app += "0";

        app += std::to_string(i);
        input.open(fileDir + app, std::ios::in | std::ios::binary);
        unsigned char ch;
        input.read((char*) &ch, 1);
        while (!input.eof())
        {
            frequency[i-1][ch]++;
            input.read((char*) &ch, 1);
        }
        input.clear();
        input.seekg(0, std::ios::end);
        fileSizes[i-1] = input.tellg();

        input.close();
    }
}

double CHWController::estimateEntropy(int fileNum)
{
    double sum = 0;

    for(int i = 0; i < 256; ++i)
    {
        double prob  = (double) frequency[fileNum-1][i] / fileSizes[fileNum-1];

        if(prob > 0)
            sum+= prob * std::log(prob);
    }

    return  -sum;
}

void CHWController::estimate(int fromAlgo, int toAlgo, int fromFile, int toFile, std::string exactFilename)
{
    estimateFrequency();

    for (int j = fromFile; j <= toFile; ++j)
    {
        for(int i = fromAlgo; i < toAlgo; ++i)
        {
            for (int c = 0; c < 20; ++c)
            {
                std::string app = "";
                if (j < 10)
                    app += "0";

                app += std::to_string(j);
                if (exactFilename != "")
                    app = exactFilename;

                compressorStatistic[j-1][i].compressingTime += compressors[i]->compress(fileDir + app);
                compressorStatistic[j-1][i].decompressingTime += compressors[i]->decompress(fileDir + app);
                if (exactFilename == "" && c == 0)
                    compressorStatistic[j-1][i].coefficient = estimateCoefficient(i, j);
            }
        }
    }

}

bool CHWController::validate(int fromAlgo, int toAlgo, int fromFile, int toFile, std::string exactFilename)
{
    bool flag = true;
    for (int i = fromAlgo; i < toAlgo; ++i)
    {
        for(int j = fromFile; j <= toFile; ++j)
        {
            std::string app = "";
            if (j < 10)
                app += "0";

            app += std::to_string(j);
            if (exactFilename != "")
                app = exactFilename;

            std::cout << app + ": " + ((flag = flag && compressors[i]->validate(fileDir + app, fileDir + app + decomp_ext[i])) ? "OK" : "ERROR") << std::endl;
        }
    }

    return flag;
}

void CHWController::writeFrequenciesToCSV(int fileNum, std::string file)
{
    std::ofstream output(file);
    for(int i = 0; i < 256; ++i){
        output << i << "\t;" << frequency[fileNum][i] << std::endl;
    }

    output.close();
}

void CHWController::writeStatisticsToCSV(std::string file)
{
    std::ofstream output("generated/" + file);
    output << "File;\t";
    output << "Entropy;\t";
    for (int i = 0; i < 5; ++i)
    {
        output << algo_name[i] << " K;\t";
        output << algo_name[i] << " tp;\t";
        output << algo_name[i] << " tu;\t";
    }
    output << std::endl;

    for (int i = 1; i <= 36; ++i)
    {
        output << i << ";\t";
        output << estimateEntropy(i) << ";\t";
        for (int j = 0; j < 5; ++j)
        {
            output << compressorStatistic[i-1][j].coefficient << ";\t";
            output << compressorStatistic[i-1][j].compressingTime / 20.0 << ";\t";
            output << compressorStatistic[i-1][j].decompressingTime / 20.0 << ";\t";
            compressorStatistic[i-1][j].compressingTime=0;
            compressorStatistic[i-1][j].decompressingTime=0;
        }
        output << std::endl;
        writeFrequenciesToCSV(i, "generated/" + std::to_string(i) + "_freq.csv");
    }

    output.close();
}