#pragma once
#include <vector>
#include <iostream>
#include <fstream>

template <class T>
class FileManager{
    public:

        FileManager(std::string infilename, uint64_t chunksize);
        ~FileManager();
        void Open();
        void Close();
        void CloseInputFile();
        void CloseOutputFile();
        void OpenInputFile();
        void OpenOutputFile();
        std::string createOutputFilename();
        void ReadChunk();
        void WriteChunk();
        bool ChunksRemain() {return (_readchunkindex)*_chunksize <= _filesize; };
        void PushOutputData(T output);
        std::vector<uint8_t>* getInputBufferRef() { return &_inputBuffer; }
        std::vector<T>*       getOutputBufferRef() { return &_outputBuffer; }

    private:
        std::vector<uint8_t> _inputBuffer;
        std::vector<T>       _outputBuffer;
        std::string _infilename;
        std::string _outfilename;
        FILE*       _infile;
        FILE*       _outfile;
        uint64_t    _filesize;
        uint64_t    _chunksize;
        uint64_t    _readchunkindex;
        uint64_t    _writechunkindex;
};

template <class T>
FileManager<T>::FileManager(std::string infilename, uint64_t chunksize) : _chunksize(chunksize), _readchunkindex(0),  _writechunkindex(0), _infilename(infilename), _outfilename(createOutputFilename()), _inputBuffer(chunksize){
     _outputBuffer.reserve(chunksize/sizeof(T));
    Open();
}

template <class T>
void FileManager<T>::Open(){
    OpenInputFile();
    OpenOutputFile();
}

template <class T>
void FileManager<T>::Close(){
    CloseInputFile();
    CloseOutputFile();
}

template <class T>
void FileManager<T>::CloseInputFile(){
    if (_infile){
        Event event("Closing " + _infilename);
        fclose(_infile);
        _infile = nullptr;
        event.stop();
    }
}

template <class T>
void FileManager<T>::CloseOutputFile(){
    if (_outfile){
        Event event("Closing " + _outfilename);
        fclose(_outfile);
        _outfile = nullptr;
        event.stop();
    }
}


template <class T>
FileManager<T>::~FileManager(){
    Close();
}


template <class T>
std::string FileManager<T>::createOutputFilename(){
    return _infilename.substr(0, _infilename.find('.')) + ".bin";
}


template <class T>
void FileManager<T>::OpenInputFile(){
    Event event("Opening " + _infilename);
    _infile = fopen(_infilename.c_str(), "rb+");
    if (!_infile){
        std::cout << std::endl << "Error opening " << _infilename << ". Exiting..." << std::endl;
        exit(-1);
    }
    fseek(_infile, 0L, SEEK_END);
    _filesize = ftell(_infile);
    fseek(_infile, 0L, SEEK_SET);
    event.stop();
}

template <class T>
void FileManager<T>::OpenOutputFile(){
    Event event("Opening " + _outfilename);
    _outfile = fopen(_outfilename.c_str(), "wb+");
    if (!_outfile){
        std::cout << std::endl << "Error opening " << _outfilename << ". Exiting..." << std::endl;
        exit(-1);
    }
    event.stop();
}

template <class T>
void FileManager<T>::ReadChunk(){
    Event event("Reading chunk " + std::to_string(_readchunkindex)  + " (" + std::to_string(_inputBuffer.size()) + ") bytes: ");
    
    if (_filesize - _chunksize*_readchunkindex < _chunksize)
        _inputBuffer.resize(_filesize - _chunksize*_readchunkindex);

    fread(_inputBuffer.data(), _inputBuffer.size(), sizeof(uint8_t), _infile);

    _readchunkindex++;
    event.stop();
}

template <class T>
void FileManager<T>::PushOutputData(T data){
    _outputBuffer.push_back(data);
    if (_outputBuffer.size()*sizeof(T) >= _chunksize)
        WriteChunk();
}


template <class T>
void FileManager<T>::WriteChunk(){
    Event event("Writing chunk " + std::to_string(_writechunkindex) + " (" + std::to_string(_outputBuffer.size()*sizeof(T)) + ") bytes: ");

    fwrite(_outputBuffer.data(), _outputBuffer.size(), sizeof(T), _outfile);

    _outputBuffer.clear();
    _writechunkindex++;
    event.stop();
}