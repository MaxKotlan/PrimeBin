#pragma once
#include <iostream>
#include <fstream>
#include <stack>
#include "convert.h"
#include <arpa/inet.h>


Converter::Converter(std::string filename) : inputfilename(filename){
    input = fopen(filename.c_str(), "rb+");
    fseek(input, 0L, SEEK_END);
    uint64_t size = ftell(input);
    fseek(input, 0L, SEEK_SET);
    buffer.resize(size);
    fread(buffer.data(), buffer.size(), 1, input);
    fclose(input);
    asciiToBinary();
};

void Converter::swapEndianess() {
    std::cout << "Swapping Endieness..." << std::endl; 
    for (uint32_t &primenumber : outputdata)
        primenumber = htonl(primenumber);
}

void Converter::write(){
    output = fopen((inputfilename.substr(0, inputfilename.find('.')) + ".bin").c_str(), "wb+");
    fwrite(outputdata.data(), outputdata.size(), sizeof(uint32_t), output);
    fclose(output);
}

void Converter::asciiToBinary(){
    uint32_t result = 0;
    for (uint64_t i = 0; i < buffer.size(); i++){
        if(buffer[i] >= '0' && buffer[i] <= '9'){
            result *= 10;
            result += (uint32_t)(buffer[i]-'0');
        } else {
            outputdata.push_back(result);
            result = 0;
        }
    }
    
}