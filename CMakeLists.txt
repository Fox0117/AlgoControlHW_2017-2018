cmake_minimum_required(VERSION 3.9)
project(KDZ)

set(CMAKE_CXX_STANDARD 11)

add_executable(KDZ main.cpp CompressorHuffman.h CompressorHuffman.cpp
        CompressorShannon.h CompressorShannon.cpp CompressorLZ77.h CompressorLZ77.cpp Compressor.cpp Compressor.h)