#pragma once

#include "common_defs.hpp"
#include "EncodedSample.hpp"
#include "StreamHeader.hpp"

namespace data_logger {

template <class T>
class Stream {
public:
    Stream();
    ~Stream();

    //Write given sample to ofstream
    size_t pushSample(std::ofstream& fstream, T sample);
    //Read next sample from ifstream
    size_t readNextSample(std::ifstream& fstream, T& sample);
    //Jump back to first sample of ifstream
    void rewind(std::ifstream& fstream);

    StreamHeader& header(){return _header;}

    void setEncoding(size_t (*fn)(BufferByte*, size_t, const T*)){_currentSample.setEncodeFn(fn);}
    void setDecoding(int (*fn)(T*, BufferByte*, size_t)){_currentSample.setDecodeFn(fn);}
protected:
    size_t _currentSampleIdx;
    EncodedSample<T> _currentSample;
    StreamHeader _header;
};

}

#include "Stream_impl.hpp"
