#pragma once
#include <vector>

class Converter{
    public:
        Converter(std::string filename);
        void swapEndianess();
        void setBase(uint32_t nbase) { base = nbase; };
        void read();
        void write();
        void convertToBinary();

    private:
        std::string inputfilename;
        FILE *input;
        FILE *output;
        std::vector<uint8_t> buffer;
        std::vector<uint32_t> outputdata;
        uint32_t base;
};