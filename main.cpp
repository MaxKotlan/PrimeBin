#include <iostream>
#include <string>
#include "convert.h"
#include "file.h"

typedef enum Primitive{
    uint8,
    uint16,
    uint32,
    uint64,
    float32,
    float64
} Primitive;

struct Startup{
    bool swapEndianess = false;
    uint32_t base = 10;
    Primitive writeprim = uint32;
} startup;

void help(std::string progname){
    std::cout << "Usage: " << progname << " [filename.txt]" << std::endl;
    std::cout << "\t-b, --base [2-36]\t   interprets digits in different base. Default 10. Digits outside range of base (for instance 9 in base 9, will be skipped)." << std::endl;
    std::cout << "\t-p, --primitive [type]\t   select primitive of output memory map. If number is too large standard overflow will occur." << std::endl;
    std::cout << "\t\tuint8\t(1 bytes)" << std::endl;
    std::cout << "\t\tuint16\t(2 bytes)" << std::endl;
    std::cout << "\t\tuint32\t(4 bytes)" << std::endl;
    std::cout << "\t\tuint64\t(8 bytes)" << std::endl;
    std::cout << "\t\tfloat32\t(4 bytes)" << std::endl;
    std::cout << "\t\tfloat64\t(8 bytes)" << std::endl;
    std::cout << "\t-e, --swapendianess\t   saves file in opposite endianess of system. For instance if your system is x86 (little endian), results are saved as big endian." << std::endl;
    exit(-1);
}

std::string parseArgs(int argc, char** argv){
    if (argc < 2) help(argv[0]);
    std::string filename;
    for (int i = 0; i < argc; i++){
        if (std::string(argv[i]).find("--") == std::string::npos)
            filename = argv[i];
        else if (std::string(argv[i]) == "--swapendianess" || std::string(argv[i]) == "-e")
            startup.swapEndianess = true;
        else if ((std::string(argv[i]) == "--base" || std::string(argv[i]) == "-b") && i < argc-1){
            startup.base = std::stoi(argv[i+1]);
            if (startup.base < 2 || startup.base > 36) {
                std::cout << "Base must be equal or between  2 and 36" << std::endl;
                exit(-1);
            }
        }
        else if ((std::string(argv[i]) == "--primitive" || std::string(argv[i]) == "-p") && i < argc-1){
            if (std::string(argv[i+1]) == "uint8")  startup.writeprim = uint8;
            if (std::string(argv[i+1]) == "uint16") startup.writeprim = uint16;
            if (std::string(argv[i+1]) == "uint32") startup.writeprim = uint32;
            if (std::string(argv[i+1]) == "uint64") startup.writeprim = uint64;
            if (std::string(argv[i+1]) == "float32") startup.writeprim = float32;
            if (std::string(argv[i+1]) == "float64") startup.writeprim = float64;
        }

    }
    if (filename == "") exit(-1);
    return filename;
}

template <class T>
void TransformAndWrite(std::string filename, std::vector<uint8_t>* inputbuffer){
    Converter<T> conv(inputbuffer);
    conv.setBase(startup.base);
    conv.convertToBinary();
    if (startup.swapEndianess)
        conv.swapEndianess();
        
    std::string outputfilename = filename.substr(0, filename.find('.')) + ".bin";
    WriteFile(outputfilename, conv.outputdata);
}

int main(int argc, char** argv){
    std::string filename = parseArgs(argc, argv);
    std::vector<uint8_t> inputbuffer = ReadFile(filename);
    
    switch (startup.writeprim){
        case uint8: 
            TransformAndWrite<uint8_t>(filename, &inputbuffer); break;
        case uint16: 
            TransformAndWrite<uint16_t>(filename, &inputbuffer); break;
        case uint32: 
            TransformAndWrite<uint32_t>(filename, &inputbuffer); break;
        case uint64: 
            TransformAndWrite<uint64_t>(filename, &inputbuffer); break;
        case float32: 
            TransformAndWrite<float>(filename, &inputbuffer); break;
        case float64: 
            TransformAndWrite<double>(filename, &inputbuffer); break;

    }

}