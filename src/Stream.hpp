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
    size_t pushSample(std::ofstream& fstream, const T &sample);
    //Read next sample from ifstream
    size_t readNextSample(char* ptr);
    //Jump back to first sample of ifstream
    void rewind(std::ifstream& fstream);

    StreamHeader& header(){return _header;}

    void setEncoding(size_t (*fn)(BufferByte*, size_t, const T*)){currentSample.setEncodeFn(fn);}
    void setDecoding(int (*fn)(T*, BufferByte*, size_t)){currentSample.setDecodeFn(fn);}
    EncodedSample<T> currentSample;
    size_t currentSampleIdx;
protected:
    StreamHeader _header;
};

}

#include "Stream_impl.hpp"
