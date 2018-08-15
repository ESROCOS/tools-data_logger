#pragma once
#include "LogFileReader.hpp"
#include "StreamHeader.hpp"
#include <exception>
#include <stdlib.h>
#include <sys/stat.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

size_t getFilesize(const char* filename) {
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}

template<class T>
data_logger::LogFileReader<T>::LogFileReader(std::string filepath)
{
    errno = 0;

    //memory map file
    filesize = getFilesize(filepath.c_str());
    std::cout << filepath <<std::endl;
    fd = open(filepath.c_str(), O_RDONLY, 0);
    if(fd == -1){
        std::cerr << "opening log file failed: " << strerror(errno) << std::endl;
        runtime_error("IO_FAIL");
    }
    mmappedData = (char*)mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
    if(mmappedData == MAP_FAILED){
        std::cerr << "Memory Mapping failed: " << strerror(errno) << std::endl;
        runtime_error("IO_FAIL");
    }
    mem_offset_payload = -1;

    //_fstream.open(filepath, std::ios_base::in | std::ios_base::binary);
    //if (errno != 0)
    //    throw std::system_error(errno, std::system_category());
    //if (!_fstream)
    //    runtime_error("IO_FAIL");
}

template<class T>
data_logger::LogFileReader<T>::~LogFileReader()
{
    if(mmappedData){
        this->closeLogfile();
    }
}

template<class T>
void data_logger::LogFileReader<T>::closeLogfile()
{
    int rc = munmap(mmappedData, filesize);
    mmappedData = 0;
    close(fd);
    //_fstream.close();
}

template<class T>
data_logger::StreamHeader data_logger::LogFileReader<T>::readStreamHeader()
{
    //_fstream.seekg(0, _fstream.beg);
    StreamHeader& header = _stream.header();
    size_t bytes_read = header.deserialize(mmappedData);
    mem_offset_payload = bytes_read;
    mem_offset = 0;
    return _stream.header();
}

template<class T>
size_t data_logger::LogFileReader<T>::readNextSample(
        EncodedSample<T> &sample)
{
    /*if(_fstream.eof()){
        std::cout << "END OF FILE!" <<std::endl;
    }
    if(!_fstream.good()){
        std::cout << "NOT GOOD!" <<std::endl;
    }*/
    if(mem_offset_payload == -1){
        runtime_error("readStreamHeader was not called before starting calling readNextSample");
    }
    size_t bytes_read = _stream.readNextSample(mmappedData + mem_offset_payload + mem_offset);
    if(bytes_read){
        sample = _stream.currentSample;
        mem_offset += bytes_read;
        return _stream.currentSampleIdx;
    }else{
        return -1;
    }
}

template<class T>
void data_logger::LogFileReader<T>::rewind()
{
    _fstream.seekg(0, _fstream.beg);
}


