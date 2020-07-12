#pragma once
#include <vector>
#include <iostream>
#include <fstream>

std::vector<uint8_t> ReadFile(std::string filename){
    std::cout << "Reading " << filename << " Into Memory... " << std::flush; 
    FILE* input = fopen(filename.c_str(), "rb+");
    if (!input){
        std::cout << std::endl << "Error opening " << filename << ". Exiting..." << std::endl;
        exit(-1);
    }
    fseek(input, 0L, SEEK_END);
    uint64_t size = ftell(input);
    fseek(input, 0L, SEEK_SET);
    std::vector<uint8_t> buffer(size);
    fread(buffer.data(), buffer.size(), 1, input);
    fclose(input);
    std::cout << "Done!" << std::endl;
    return std::move(buffer);
}

template <class T>
void WriteFile(std::string filename, std::vector<T> &outputdata){
    std::cout << "Writing to " << filename << "... " << std::flush; 
    FILE* output = fopen(filename.c_str(), "wb+");
    if (!output){
        std::cout << std::endl << "Error opening " << filename << ". Exiting..." << std::endl;
        exit(-1);
    }
    fwrite(outputdata.data(), outputdata.size(), sizeof(T), output);
    fclose(output);
    std::cout << "Done!" << std::endl;
}
