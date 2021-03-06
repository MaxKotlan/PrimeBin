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
#include "file.h"

template <class T>
class Converter{
    public:
        Converter(FileManager<T>* fm);
        void swapEndianess();
        void setIgnoredDelimiters(std::string igdelm){ ignoreddelimiters = igdelm; }
        void setIgnoreSigns(bool ignigns) { ignoresigns = ignigns; };
        void setBase(uint32_t nbase) { base = nbase; decimalindex = base; };
        void convertToBinary();

    private:
        std::string ignoreddelimiters;
        FileManager<T>* _fm;
        uint32_t base = 10;
        bool ignoresigns;

        /*Conversion*/
        T result = 0;
        bool pushnumber = false;
        bool calculateDecimal = false;
        uint32_t decimalindex = base;
        int8_t sign = 1;
};

template <class T>
Converter<T>::Converter(FileManager<T>* fm) : ignoreddelimiters(""), _fm(fm) {
    base = 10;
    result = 0;
    pushnumber = false;
    calculateDecimal = false;
    decimalindex = base;
    sign = 1;
};

template <class T>
void Converter<T>::swapEndianess() {
    Event event("Swapping Endieness");
    for (T &primenumber : *_fm->getOutputBufferRef())
        primenumber = htonl(primenumber);
    event.stop();
}

template <class T>
/*This is gross. Make sure to compile with optimization (-O3)*/
void Converter<T>::convertToBinary(){
    Event event("Converting", true);

    for (uint8_t digit : *_fm->getInputBufferRef()){
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
            //outputdata->push_back(sign * result);
            _fm->PushOutputData(sign * result);
            result = 0;
            calculateDecimal = false;
            decimalindex = base;
            pushnumber = false;
            sign = 1;
        }
    }
    if (pushnumber && !_fm->ChunksRemain()){
        _fm->PushOutputData(sign * result);
        result = 0;
        pushnumber = false;
    }
    event.stop();
}