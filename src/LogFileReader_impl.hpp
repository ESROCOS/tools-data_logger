#pragma once
#include "LogFileReader.hpp"
#include "StreamHeader.hpp"
#include <exception>

template<class T>
data_logger::LogFileReader<T>::LogFileReader(std::string filepath)
{
    errno = 0;
    _fstream.open(filepath, std::ios_base::in | std::ios_base::binary);
    if (errno != 0)
        throw std::system_error(errno, std::system_category());
    if (!_fstream)
        runtime_error("IO_FAIL");
}

template<class T>
data_logger::LogFileReader<T>::~LogFileReader()
{
    this->close();
}

template<class T>
void data_logger::LogFileReader<T>::close()
{
    _fstream.close();
}

template<class T>
data_logger::StreamHeader data_logger::LogFileReader<T>::readStreamHeader()
{
    _fstream.seekg(0, _fstream.beg);
    StreamHeader& header = _stream.header();
    header.deserialize(_fstream);
    return _stream.header();
}

template<class T>
size_t data_logger::LogFileReader<T>::readNextSample(
        EncodedSample<T> &sample)
{
    if(_fstream.eof()){
        std::cout << "END OF FILE!" <<std::endl;
    }
    if(!_fstream.good()){
        std::cout << "NOT GOOD!" <<std::endl;
    }
    bool st = _stream.readNextSample(_fstream);
    if(st){
        sample = _stream.currentSample;
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


