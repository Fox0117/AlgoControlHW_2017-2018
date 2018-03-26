// КДЗ по дисциплине Алгоритмы и структуры данных 2017-2018 уч.год
// Выполнил Кончагин Андрей Максимович, группа БПИ-161, дата (16.03.2018)
// Среда разработки CLion,
// состав проекта (main.cpp CompressorHuffman.h CompressorHuffman.cpp
// CompressorShannon.h CompressorShannon.cpp CompressorLZ77.h CompressorLZ77.cpp)
// Выполнены алгоритмы: Хаффман, Шеннон-Фано, Лемпель-Зив 77 года

#include <iostream>
#include <string>
#include "CHWController.h"

using namespace std;

int main() {
    std::string dir = "../samples-for-students/";
    std::cout << "Enter test directory: ";
    std::cin >> dir;
    CHWController* controller = new CHWController(dir);
    controller->estimateTestFiles();
    controller->writeStatisticsToCSV("results.csv");
    return 0;
}