#pragma once
#include <vector>

class Converter{
    public:
        Converter(std::string filename);
        void asciiToBinary();

    private: 
        FILE *input;
        FILE *output;
        std::vector<uint8_t> buffer;
        std::vector<uint32_t> outputdata;
};