#pragma once
#include <vector>

class Converter{
    public:
        Converter(std::string filename);
        void swapEndianess();
        void write();
        void asciiToBinary();

    private:
        std::string inputfilename;
        FILE *input;
        FILE *output;
        std::vector<uint8_t> buffer;
        std::vector<uint32_t> outputdata;
};