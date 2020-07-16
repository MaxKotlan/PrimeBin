#include <iostream>
#include <string>
#include "analyze.h"
#include "convert.h"
#include "preview.h"
#include "file.h"

typedef enum Primitive{
    uint8,
    uint16,
    uint32,
    uint64,
    int8,
    int16,
    int32,
    int64,
    float32,
    float64
} Primitive;

struct Startup{
    bool ignoreSigns   = false;
    bool swapEndianess = false;
    bool verbose = false;
    uint32_t base = 10;
    uint64_t maxmemory = 536870912;
    Primitive writeprim = uint32;
    std::string ignore = "";
} startup;

void help(std::string progname){
    std::cout << "Usage: " << progname << " [filename.txt]" << std::endl;
    std::cout << "\t-h, --help\t                 shows this prompt" << std::endl;
    std::cout << "\t-b, --base [2-36]\t         interprets digits in different base. Default 10. Digits outside range of base (for instance 9 in base 9, will be skipped)." << std::endl;
    std::cout << "\t-u, --unsigned\t                 treat all numbers as unsigned even if a number sign is present. example: -2 would equal 0x2" << std::endl;
    std::cout << "\t-i, --ignore [string]\t         ignores delimiters or digits. example ignore ',': 1,2 = 12 or 0xC" << std::endl;
    std::cout << "\t-v, --verbose\t                 print to the console more frequently (can impact performance if memory is too small)" << std::endl;
    std::cout << "\t-m, --memory [integer]\t         maximum ram used in bytes" << std::endl;
    std::cout << "\t\texample: -i \"a1 ,*\"\t will ignore a 1 space ," << std::endl;
    std::cout << "\t-p, --primitive [type]\t         select primitive of output memory map. If number is too large standard overflow will occur." << std::endl;
    std::cout << "\t\tuint8\t(1 bytes)" << std::endl;
    std::cout << "\t\tuint16\t(2 bytes)" << std::endl;
    std::cout << "\t\tuint32\t(4 bytes)" << std::endl;
    std::cout << "\t\tuint64\t(8 bytes)" << std::endl;
    std::cout << "\t\tint8\t(1 bytes)" << std::endl;
    std::cout << "\t\tint16\t(2 bytes)" << std::endl;
    std::cout << "\t\tint32\t(4 bytes)" << std::endl;
    std::cout << "\t\tint64\t(8 bytes)" << std::endl;
    std::cout << "\t\tfloat32\t(4 bytes)" << std::endl;
    std::cout << "\t\tfloat64\t(8 bytes)" << std::endl;
    std::cout << "\t-e, --swapendianess\t         saves file in opposite endianess of system. For instance if your system is x86 (little endian), results are saved as big endian." << std::endl;
    exit(-1);
}

std::string parseArgs(int argc, char** argv){
    if (argc < 2) help(argv[0]);
    std::string filename;
    for (int i = 0; i < argc; i++){
        if (std::string(argv[i]).find("-") == std::string::npos)
            filename = argv[i];
        else if (std::string(argv[i]) == "--help" || std::string(argv[i]) == "-h")
            help(argv[0]);
        else if (std::string(argv[i]) == "--unsigned" || std::string(argv[i]) == "-u")
            startup.ignoreSigns = true;
        else if (std::string(argv[i]) == "--swapendianess" || std::string(argv[i]) == "-e")
            startup.swapEndianess = true;
        else if ((std::string(argv[i]) == "--verbose" || std::string(argv[i]) == "-v")){
            startup.verbose = true;
        }else if ((std::string(argv[i]) == "--base" || std::string(argv[i]) == "-b") && i < argc-1){
            startup.base = std::stoi(argv[i+1]);
            if (startup.base < 2 || startup.base > 36) {
                std::cout << "Base must be equal or between  2 and 36" << std::endl;
                exit(-1);
            }
        }else if ((std::string(argv[i]) == "--memory" || std::string(argv[i]) == "-m") && i < argc-1){
            startup.maxmemory = std::stoi(std::string(argv[i+1]));
            if (startup.maxmemory < 1024) {
                std::cout << "Max memory must be at least 1024 bytes" << std::endl;
                exit(-1);
            }
        }else if ((std::string(argv[i]) == "--ignore" || std::string(argv[i]) == "-i") && i < argc-1){
            startup.ignore = std::string(argv[i+1]);
        } else if ((std::string(argv[i]) == "--primitive" || std::string(argv[i]) == "-p") && i < argc-1){
            if (std::string(argv[i+1]) == "uint8")   startup.writeprim = uint8;
            if (std::string(argv[i+1]) == "uint16")  startup.writeprim = uint16;
            if (std::string(argv[i+1]) == "uint32")  startup.writeprim = uint32;
            if (std::string(argv[i+1]) == "uint64")  startup.writeprim = uint64;
            if (std::string(argv[i+1]) == "int8")    startup.writeprim = int8;
            if (std::string(argv[i+1]) == "int16")   startup.writeprim = int16;
            if (std::string(argv[i+1]) == "int32")   startup.writeprim = int32;
            if (std::string(argv[i+1]) == "int64")   startup.writeprim = int64;
            if (std::string(argv[i+1]) == "float32") startup.writeprim = float32;
            if (std::string(argv[i+1]) == "float64") startup.writeprim = float64;
        }

    }
    if (filename == "") exit(-1);
    return filename;
}

template <class T>
void TransformFile(std::string inputfile){
    Event event("Converting File", startup.verbose, true);

    FileManager<T> fm(inputfile, startup.maxmemory/2);
    Converter<T> conv(&fm);
    conv.setIgnoreSigns(startup.ignoreSigns);
    conv.setIgnoredDelimiters(startup.ignore);
    conv.setBase(startup.base);

    do{
        fm.ReadChunk();
        conv.convertToBinary();
        if (startup.swapEndianess)
            conv.swapEndianess();
    } while (fm.ChunksRemain());
    fm.WriteChunk();
    fm.Close();
    event.stop();
    //std::string outputfilename = filename.substr(0, filename.find('.')) + ".bin";
    //WriteFile(outputfilename, conv.outputdata);
    
    //Previewer<T> preview(fm.getOutputBufferRef());
    //preview.PrintTop();
}

int main(int argc, char** argv){
    std::string filename = parseArgs(argc, argv);
    //std::vector<uint8_t> inputbuffer = ReadFile(filename);
    
    if (startup.verbose)
        Event::DisplayAll();
    //Analyzer analyze(&inputbuffer);

    switch (startup.writeprim){
        case uint8: 
            TransformFile<uint8_t>(filename); break;
        case uint16: 
            TransformFile<uint16_t>(filename); break;
        case uint32: 
            TransformFile<uint32_t>(filename); break;
        case uint64: 
            TransformFile<uint64_t>(filename); break;
        case int8: 
            TransformFile<int8_t>(filename); break;
        case int16: 
            TransformFile<int16_t>(filename); break;
        case int32: 
            TransformFile<int32_t>(filename); break;
        case int64: 
            TransformFile<int64_t>(filename); break;
        case float32: 
            TransformFile<float>(filename); break;
        case float64: 
            TransformFile<double>(filename); break;

    }

}