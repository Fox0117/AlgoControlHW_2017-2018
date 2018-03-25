// КДЗ по дисциплине Алгоритмы и структуры данных 2017-2018 уч.год
// Выполнил Кончагин Андрей Максимович, группа БПИ-161, дата (16.03.2018)
// Среда разработки CLion,
// состав проекта (main.cpp CompressorHuffman.h CompressorHuffman.cpp
// CompressorShannon.h CompressorShannon.cpp CompressorLZ77.h CompressorLZ77.cpp)
// Выполнены алгоритмы: Хаффман, Шеннон-Фано, Лемпель-Зив 77 года

#include <iostream>
#include <string>
#include "CompressorHuffman.h"
#include "CompressorShannon.h"
#include "CompressorLZ77.h"
#include "CHWController.h"

using namespace std;

int main() {
    CHWController* controller = new CHWController("../samples-for-students/");
    controller->estimateTestFiles();
    //controller->estimate(0, 1, 1, 2);
    controller->writeStatisticsToCSV("results.csv");
    return 0;
}