#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <arpa/inet.h>

template <class T>
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
        std::vector<T> outputdata;
        uint32_t base;
};

template <class T>
Converter<T>::Converter(std::string filename) : inputfilename(filename){
    read();
};

template <class T>
void Converter<T>::swapEndianess() {
    std::cout << "Swapping Endieness... " << std::flush; 
    for (T &primenumber : outputdata)
        primenumber = htonl(primenumber);
    std::cout << "Done!" << std::endl;
}

template <class T>
void Converter<T>::read(){
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

template <class T>
void Converter<T>::write(){
    std::string outputfilename = inputfilename.substr(0, inputfilename.find('.')) + ".bin";
    std::cout << "Writing to " << outputfilename << "... " << std::flush; 
    output = fopen(outputfilename.c_str(), "wb+");
    if (!output){
        std::cout << std::endl << "Error opening " << outputfilename << ". Exiting..." << std::endl;
        exit(-1);
    }
    fwrite(outputdata.data(), outputdata.size(), sizeof(T), output);
    fclose(output);
    std::cout << "Done!" << std::endl;
}

template <class T>
void Converter<T>::convertToBinary(){
    std::cout << "Converting... " << std::flush; 
    T result = 0;
    bool pushnumber = false;
    for (uint64_t i = 0; i < buffer.size(); i++){
        if(buffer[i] >= '0' && buffer[i] <= '9' && buffer[i] - '0' < base){
            result *= base;
            result += (T)(buffer[i]-'0');
            pushnumber = true;
        } else if (buffer[i] >= 'a' && buffer[i] <= 'z' && buffer[i] - 'a'+10 < base){
            result *= base;
            result += (T)(buffer[i]-'a'+10);
            pushnumber = true;
        } else if (buffer[i] >= 'A' && buffer[i] <= 'Z' && buffer[i] - 'A'+10 < base){
            result *= base;
            result += (T)(buffer[i]-'A'+10);
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