#include <iostream>
#include <string>
#include "convert.h"

struct Startup{
    bool swapEndianess = false;
    uint32_t base = 10;
} startup;

void help(std::string progname){
    std::cout << "Usage: " << progname << " [filename.txt]" << std::endl;
    std::cout << "\t--base [2-36]\t   interprets digits in different base. Default 10. Digits outside range of base (for instance 9 in base 9, will be skipped)." << std::endl;
    std::cout << "\t--swapendianess\t   saves file in opposite endianess of system. For instance if your system is x86 (little endian), results are saved as big endian." << std::endl;
    exit(-1);
}

std::string parseArgs(int argc, char** argv){
    if (argc < 2) help(argv[0]);
    std::string filename;
    for (int i = 0; i < argc; i++){
        if (std::string(argv[i]).find("--") == std::string::npos)
            filename = argv[i];
        else if (std::string(argv[i]) == "--swapendianess")
            startup.swapEndianess = true;
        else if (std::string(argv[i]) == "--base" && i < argc-1){
            startup.base = std::stoi(argv[i+1]);
            if (startup.base < 2 || startup.base > 36) {
                std::cout << "Base must be equal or between  2 and 36" << std::endl;
                exit(-1);
            }
        }

    }
    if (filename == "") exit(-1);
    return filename;
}

int main(int argc, char** argv){
    std::string filename = parseArgs(argc, argv);
    Converter<float> conv(filename);
    conv.setBase(startup.base);
    conv.convertToBinary();
    if (startup.swapEndianess)
        conv.swapEndianess();
    conv.write();
}