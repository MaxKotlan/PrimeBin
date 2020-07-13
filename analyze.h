#pragma once
#include <vector>
#include "digit.h"

class Analyzer{
    public:
        Analyzer(std::vector<uint8_t>* buffer);
        void search();

    private:
        std::vector<uint8_t>* inputdata;
        uint32_t _maxconsecutivedigits;
        uint32_t _maxbase;
};

Analyzer::Analyzer(std::vector<uint8_t>* buffer) : inputdata(buffer) {
    search();
}

void Analyzer::search(){
    uint32_t consecutivedigit = 0;
    for (uint8_t byte : *inputdata){
        if (AlphaNumeric::isAlphaNumeric(byte)) consecutivedigit++; else consecutivedigit = 0;
        if (consecutivedigit > _maxconsecutivedigits) _maxconsecutivedigits = consecutivedigit;
    }
    //std::cout << "The maximum number of consecutive digits is " << _maxconsecutivedigits << std::endl;
}