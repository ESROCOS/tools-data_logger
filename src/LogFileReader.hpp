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
};
}

#include "LogFileReader_impl.hpp"
