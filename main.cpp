// КДЗ по дисциплине Алгоритмы и структуры данных 2017-2018 уч.год
// Выполнил Кончагин Андрей Максимович, группа БПИ-161, дата (16.03.2018)
// Среда разработки CLion, состав проекта (main.cpp CompressorHuffman.h CompressorHuffman.cpp CompressorShannon.h CompressorShannon.cpp CompressorLZ77.h CompressorLZ77.cpp)
// Выполнены алгоритмы: Хаффман, Шеннон-Фано, Лемпель-Зив 77 года

#include <iostream>
#include <string>
#include "CompressorHuffman.h"
#include "CompressorShannon.h"
#include "CompressorLZ77.h"

using namespace std;

int main() {
    std::string filename = "../samples-for-students/";
    CompressorHuffman comp;
    for(int i = 11; i < 37; ++i)
    {
        comp.compress(filename + to_string(i));
        comp.decompress(filename + to_string(i));
        cout << (comp.validate(filename + to_string(i), filename + to_string(i) + ".unhaff") ? "OK" : "ERROR") << std::endl;
    }

    /*comp.compress(filename + "13");
    comp.decompress(filename + "13");
    cout << (comp.validate(filename + "13", filename + "13.unhaff") ? "OK" : "ERROR") << std::endl;
    comp.compress("inputTest");
    comp.decompress("inputTest");
    cout << (comp.validate("inputTest", "inputTest.unhaff") ? "OK" : "ERROR") << std::endl;*/

    /*CompressorLZ77 compressorLZ77(20*1024, 16*1024);
    compressorLZ77.compress(filename+"05");
    compressorLZ77.decompress(filename+"05");
    cout << compressorLZ77.validate(filename+"05",filename+"05.unlz7720");*/
    return 0;
}