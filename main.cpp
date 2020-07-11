#include <iostream>
#include "convert.h"

int main(int argc, char** argv){
    Converter conv("primes.txt");
    conv.swapEndianess();
    conv.write();

    //test reading the file
    FILE* test = fopen("primes.bin", "rb+");
    fseek(test, 0L, SEEK_END);
    uint64_t size = ftell(test);
    fseek(test, 0L, SEEK_SET);
    std::vector<uint32_t> rbuf(size);
    fread(rbuf.data(), rbuf.size(), sizeof(uint32_t), test);
    fclose(test);
    for (uint64_t i = 0; i < rbuf.size(); i++){
        std::cout << rbuf[i] << std::endl;
        if (i > 10) break;
    }


}