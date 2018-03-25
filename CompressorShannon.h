//
// Created by fox0117 on 16.03.18.
//

#ifndef KDZ_ZIPSHANON_H
#define KDZ_ZIPSHANON_H

#include "Compressor.h"
#include <vector>
#include <string>

class CompressorShannon : public Compressor
{
    struct Node {
        long long id;
        Node* zero;
        Node* one;
    };

    Node* root;
    std::vector<unsigned int> probabilities;
    std::vector<std::string> codes;
    std::vector<long long> sum;

    int getMid(int start, int end);
    void split(int start, int end, std::string* code);
    void build();
    void buildTree(Node* currentNode, int codeNum, int ind = 0);
    void clear(Node *curr);

    void addChance(unsigned int oneFreq) override
    {
        probabilities.push_back(oneFreq);
    }

public:
    unsigned long long compress(std::string filename) override;

    unsigned long long decompress(std::string filename) override;
};


#endif //KDZ_ZIPSHANON_H
