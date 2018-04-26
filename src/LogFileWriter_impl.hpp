#pragma once

#include "LogFileWriter.hpp"

template<class T>
data_logger::LogFileWriter<T>::LogFileWriter(std::string filepath) : _headerWritten(false)
{
    _fstream.open(filepath, std::ios_base::out | std::ios_base::binary);
}

template<class T>
data_logger::LogFileWriter<T>::~LogFileWriter()
{
    this->close();
}

template<class T>
void data_logger::LogFileWriter<T>::LogFileWriter::start()
{
    if(_headerWritten){
        usage_error("start has already been called");
    }
    Buffer serialized = _stream.header().serialize();
    _fstream.write((char*)&serialized[0], serialized.size()*sizeof(BufferByte));
    _fstream.flush();
    _headerWritten = true;
}

template<class T>
void data_logger::LogFileWriter<T>::write(T &sample)
{
    if(!_headerWritten){
        usage_error("start has not been called");
    }
    _stream.pushSample(_fstream, sample);
}

template<class T>
void data_logger::LogFileWriter<T>::close()
{
    _fstream.close();
}
