#pragma once
#include <vector>
#include <iostream>
#include <iomanip>
#include "event.h"

template <class T>
class Previewer{
    public:

        Previewer(std::vector<T>* outputdata);
        void PrintTop();

    private:
        uint32_t rows = 1;
        uint32_t bytepeeksize = 32;
        std::vector<T>* _outputdata;
};

template <class T>
Previewer<T>::Previewer(std::vector<T>* outputdata) : _outputdata(outputdata) {};

template <class T>
void Previewer<T>::PrintTop(){
    std::cout << std::endl <<"Map Preview: " << std::endl;
    for (uint32_t i = 0; i < _outputdata->size() && i < bytepeeksize; i++){
        std::cout << "\t" << std::hex << std::hexfloat << std::setfill('0') << std::setw(2) << i*sizeof(T) << " " << std::setw(sizeof(T)*2) << _outputdata->operator[](i) << " (" << std::dec << std::defaultfloat << _outputdata->operator[](i) << ")" << std::endl;
    }
    
};
