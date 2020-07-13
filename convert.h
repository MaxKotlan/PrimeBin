#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <arpa/inet.h>

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
    std::cout << "Swapping Endieness... " << std::flush; 
    for (T &primenumber : outputdata)
        primenumber = htonl(primenumber);
    std::cout << "Done!" << std::endl;
}

template <class T>
void Converter<T>::convertToBinary(){
    std::cout << "Converting... " << std::flush; 
    T result = 0;
    bool pushnumber = false;
    for (uint64_t i = 0; i < inputdata->size(); i++){
        if((*inputdata)[i] >= '0' && (*inputdata)[i] <= '9' && (*inputdata)[i] - '0' < base){
            result *= base;
            result += (T)((*inputdata)[i]-'0');
            pushnumber = true;
        } else if ((*inputdata)[i] >= 'a' && (*inputdata)[i] <= 'z' && (*inputdata)[i] - 'a'+10 < base){
            result *= base;
            result += (T)((*inputdata)[i]-'a'+10);
            pushnumber = true;
        } else if ((*inputdata)[i] >= 'A' && (*inputdata)[i] <= 'Z' && (*inputdata)[i] - 'A'+10 < base){
            result *= base;
            result += (T)((*inputdata)[i]-'A'+10);
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
    std::cout << "Done!" << std::endl;    
}