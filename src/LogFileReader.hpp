#pragma once
#include "common_defs.hpp"
#include "StreamHeader.hpp"
#include "EncodedSample.hpp"
#include "Stream.hpp"
#include <vector>

namespace data_logger {

template <class T>
class LogFileReader{
public:
    LogFileReader(std::string filepath);
    ~LogFileReader();

    void closeLogfile();
    void setDecoding(int (*fn)(T*, BufferByte*, size_t))
    {
        _stream.setDecoding(fn);
    }

    StreamHeader readStreamHeader();
    /*!
     * \brief readNextSample
     * \param sample
     * \return Index of the sample read. Returns -1 if the reading did not succeed.
     */
    size_t readNextSample(EncodedSample<T>& sample);
    void rewind();
protected:
    Stream<T> _stream;
    std::ifstream _fstream;
    char* mmappedData; //Memory mapped file will be stored here
    int fd; //File descriptor of log file
    size_t mem_offset; //Current offset from mmappedData
    size_t filesize;
    size_t mem_offset_payload;

};
}

#include "LogFileReader_impl.hpp"
