#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif
#include "digit.h"
#include "event.h"

template <class T>
class Converter{
    public:
        Converter(std::vector<uint8_t>* inputBuffer, std::vector<T>* outputBuffer);
        void swapEndianess();
        void setIgnoredDelimiters(std::string igdelm){ ignoreddelimiters = igdelm; }
        void setIgnoreSigns(bool ignigns) { ignoresigns = ignigns; };
        void setBase(uint32_t nbase) { base = nbase; };
        void convertToBinary();

    private:
        std::string ignoreddelimiters;
        std::vector<uint8_t>* inputdata;
        std::vector<T>* outputdata;
        uint32_t base;
        bool ignoresigns;
};

template <class T>
Converter<T>::Converter(std::vector<uint8_t>* inputBufferAddr, std::vector<T>* outputBufferAddr) : inputdata(inputBufferAddr), outputdata(outputBufferAddr), ignoreddelimiters("") {};

template <class T>
void Converter<T>::swapEndianess() {
    Event event("Swapping Endieness");
    for (T &primenumber : *outputdata)
        primenumber = htonl(primenumber);
    event.stop();
}

template <class T>
/*This is gross. Make sure to compile with optimization (-O3)*/
void Converter<T>::convertToBinary(){
    Event event("Converting");
    T result = 0;
    bool pushnumber = false;
    bool calculateDecimal = false;
    uint32_t decimalindex = base;
    char sign = 1;
    for (uint8_t digit : *inputdata){
        bool ignore = false;
        for (auto delimiter : ignoreddelimiters)
            if (digit == delimiter) ignore = true;
        if (ignore) continue;
        
        if (digit == '-' && !ignoresigns){
            sign = -1;
        } else if (digit == '.'){
            calculateDecimal = true;
        } else if(AlphaNumeric::isNumeric(digit) && (digit - '0') < base){
            if (!calculateDecimal){
                result *= base;
                result += (T)(digit-'0');
            } else {
                result += ((T)(digit-'0') / ((T)decimalindex));
                decimalindex*=base;
            }
            pushnumber = true;
        } else if (AlphaNumeric::isLowerAlphabetic(digit) && digit - 'a'+10 < base){
            if (!calculateDecimal){
                result *= base;
                result += (T)(digit-'a'+10);
            } else {
                result += ((T)(digit-'a'+10) / ((T)decimalindex));
                decimalindex*=base;
            }
            pushnumber = true;
        } else if (AlphaNumeric::isUpperAlphabetic(digit) && digit - 'A'+10 < base){
            if (!calculateDecimal){
                result *= base;
                result += (T)(digit-'A'+10);
            } else {
                result += ((T)(digit-'A'+10) / ((T)decimalindex));
                decimalindex*=base;
            }
            pushnumber = true;
        } else if (pushnumber){
            outputdata->push_back(sign * result);
            result = 0;
            calculateDecimal = false;
            decimalindex = base;
            pushnumber = false;
            sign = 1;
        }
    }
    if (pushnumber){
        outputdata->push_back(result);
        result = 0;
        pushnumber = false;
    }
    event.stop();
}