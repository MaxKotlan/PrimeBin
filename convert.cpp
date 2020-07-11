#pragma once
#include <iostream>
#include <fstream>
#include <stack>
#include "convert.h"
#include <arpa/inet.h>


Converter::Converter(std::string filename) : inputfilename(filename){
    read();
};

void Converter::swapEndianess() {
    std::cout << "Swapping Endieness... " << std::flush; 
    for (uint32_t &primenumber : outputdata)
        primenumber = htonl(primenumber);
    std::cout << "Done!" << std::endl;
}

void Converter::read(){
    std::cout << "Reading " << inputfilename << " Into Memory... " << std::flush; 
    input = fopen(inputfilename.c_str(), "rb+");
    if (!input){
        std::cout << std::endl << "Error opening " << inputfilename << ". Exiting..." << std::endl;
        exit(-1);
    }
    fseek(input, 0L, SEEK_END);
    uint64_t size = ftell(input);
    fseek(input, 0L, SEEK_SET);
    buffer.resize(size);
    fread(buffer.data(), buffer.size(), 1, input);
    fclose(input);
    std::cout << "Done!" << std::endl;
}

void Converter::write(){
    std::string outputfilename = inputfilename.substr(0, inputfilename.find('.')) + ".bin";
    std::cout << "Writing to " << outputfilename << "... " << std::flush; 
    output = fopen(outputfilename.c_str(), "wb+");
    if (!output){
        std::cout << std::endl << "Error opening " << outputfilename << ". Exiting..." << std::endl;
        exit(-1);
    }
    fwrite(outputdata.data(), outputdata.size(), sizeof(uint32_t), output);
    fclose(output);
    std::cout << "Done!" << std::endl;
}

void Converter::convertToBinary(){
    std::cout << "Converting... " << std::flush; 
    uint32_t result = 0;
    bool pushnumber = false;
    for (uint64_t i = 0; i < buffer.size(); i++){
        if(buffer[i] >= '0' && buffer[i] <= '9'){
            result *= base;
            result += (uint32_t)(buffer[i]-'0');
            pushnumber = true;
        } else if (buffer[i] >= 'a' && buffer[i] <= 'z' && buffer[i] - 'a'+10 < base){
            result *= base;
            result += (uint32_t)(buffer[i]-'a'+10);
            pushnumber = true;
        } else if (buffer[i] >= 'A' && buffer[i] <= 'Z' && buffer[i] - 'A'+10 < base){
            result *= base;
            result += (uint32_t)(buffer[i]-'A'+10);
            pushnumber = true;
        } else if (pushnumber){
            outputdata.push_back(result);
            result = 0;
            pushnumber = false;
        }
    }
    std::cout << "Done!" << std::endl;    
}