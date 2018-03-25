//
// Created by fox0117 on 16.03.18.
//

#ifndef KDZ_HAFFMAN_H
#define KDZ_HAFFMAN_H

#include <fstream>
#include <string>
#include <vector>
#include "Compressor.h"

class CompressorHuffman : public Compressor
{

private:
    struct item
    {
    public:
        item* zero;
        item* one;
        int key;
        int id;

        item(int key, item* zero, item* one, int id = -1)
        {
            this->one=one;
            this->zero=zero;
            this->key=key;
            this->id=id;
        }

        const bool operator<(const item& other) const
        {
            return (key<other.key);
        }

        const bool operator<=(const item& other) const
        {
            return (key<=other.key);
        }
    };

    std::vector<std::string> codes;
    std::vector<item*> nodes;

    int binSearch(int key);

    void calcCodes(std::string* code, item* it);

    void build();

    void clear(item *it);

    void addChance(unsigned int oneFreq) override
    {
        nodes.push_back(new item(oneFreq, nullptr, nullptr, nodes.size()));
    }

public:
    unsigned long long compress(std::string filename) override;
    unsigned long long decompress(std::string filename) override;
};


#endif //KDZ_HAFFMAN_H
