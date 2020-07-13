#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <arpa/inet.h>
#include "digit.h"
#include "event.h"

template <class T>
class Converter{
    public:
        Converter(std::vector<uint8_t>* buffer);
        void swapEndianess();
        void setBase(uint32_t nbase) { base = nbase; };
        void convertToBinary();
        std::vector<T> outputdata;

    private:
        std::vector<uint8_t>* inputdata;
        uint32_t base;
};

template <class T>
Converter<T>::Converter(std::vector<uint8_t>* bufferaddress) : inputdata(bufferaddress) {};

template <class T>
void Converter<T>::swapEndianess() {
    Event event("Swapping Endieness");
    for (T &primenumber : outputdata)
        primenumber = htonl(primenumber);
    event.stop();
}

template <class T>
void Converter<T>::convertToBinary(){
    Event event("Converting");
    T result = 0;
    bool pushnumber = false;
    bool calculateDecimal = false;
    for (uint8_t digit : *inputdata){
        if(AlphaNumeric::isNumeric(digit)){
            result *= base;
            result += (T)(digit-'0');
            pushnumber = true;
        } else if (AlphaNumeric::isLowerAlphabetic(digit) && digit - 'a'+10 < base){
            result *= base;
            result += (T)(digit-'a'+10);
            pushnumber = true;
        } else if (AlphaNumeric::isUpperAlphabetic(digit) && digit - 'A'+10 < base){
            result *= base;
            result += (T)(digit-'A'+10);
            pushnumber = true;
        } else if (pushnumber){
            outputdata.push_back(result);
            result = 0;
            pushnumber = false;
        }
    }
    if (pushnumber){
        outputdata.push_back(result);
        result = 0;
        pushnumber = false;
    }
    event.stop();
}