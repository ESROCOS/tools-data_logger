#pragma once
#include "common_defs.hpp"
#include "StreamHeader.hpp"
#include "EncodedSample.hpp"
#include "Stream.hpp"

namespace data_logger {

template <class T>
class LogFileReader{
public:
    LogFileReader(std::string filepath);
    ~LogFileReader();

    void close();
    void setDecoding(size_t (*fn)(T*, BufferByte*, size_t*))
    {
        _stream.setDecoding(fn);
    }

    void getStreamHeader();
    void readNextSample(EncodedSample<T>& sample);
    void rewind();
protected:
    Stream<T> _stream;
    std::ofstream _fstream;
};
}

#include "LogFileReader_impl.hpp"
