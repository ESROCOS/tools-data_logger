#pragma once
#include "LogFileReader.hpp"
#include "StreamHeader.hpp"


template<class T>
data_logger::LogFileReader<T>::LogFileReader(std::string filepath)
{
    _fstream.open(filepath, std::ios_base::in | std::ios_base::binary);
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
    int curpos = _fstream.tellg();
    _fstream.seekg(0, _fstream.beg);
    StreamHeader& header = _stream.header();
    header.deserialize(_fstream);
    _fstream.seekg(curpos, _fstream.beg);
    return _stream.header();
}

template<class T>
void data_logger::LogFileReader<T>::readNextSample(
        EncodedSample<T> &sample)
{
    if(_fstream.eof()){
        std::cerr << "END OF FILE!" <<std::endl;
    }
    _stream.readNextSample(_fstream);
    sample = _stream.currentSample.getPayload();
}

template<class T>
void data_logger::LogFileReader<T>::rewind()
{
    _fstream.seekg(0, _fstream.beg);
}


