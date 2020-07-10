#pragma once
#include <iostream>
#include <fstream>
#include <stack>
#include "convert.h"

size_t endian_swap_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    unsigned char *buffer_src = (unsigned char*)ptr;
    unsigned char *buffer_dst = new unsigned char[size*nmemb];
    for (size_t i = 0; i < nmemb; i++)
    {
        for (size_t ix = 0; ix < size; ix++) {
            buffer_dst[size * i + (size - 1 - ix)] = buffer_src[size * i + ix];
        }
    }
    size_t result = fwrite(buffer_dst, size, nmemb, stream);
    delete buffer_dst;
    return result;
}

Converter::Converter(std::string filename){
    input = fopen(filename.c_str(), "rb+");
    fseek(input, 0L, SEEK_END);
    uint64_t size = ftell(input);
    fseek(input, 0L, SEEK_SET);
    buffer.resize(size);
    fread(buffer.data(), buffer.size(), 1, input);
    fclose(input);
    asciiToBinary();
    output = fopen((filename.substr(0, filename.find('.')) + ".bin").c_str(), "wb+");
    fwrite(outputdata.data(), outputdata.size(), sizeof(uint32_t), output);
    fclose(output);
};

void Converter::asciiToBinary(){
    uint32_t result = 0;
    for (uint64_t i = 0; i < buffer.size(); i++){
        if(buffer[i] == '\n') {
            outputdata.push_back(result);
            result = 0;
        } else {
            result *= 10;
            result += (uint32_t)(buffer[i]-'0');
        }
    }
    
}