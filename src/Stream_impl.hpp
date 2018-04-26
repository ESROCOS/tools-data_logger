#pragma once
#include "Stream.hpp"

template<class T>
data_logger::Stream<T>::Stream() : _currentSampleIdx(-1)
{

}

template<class T>
data_logger::Stream<T>::~Stream()
{
}

template<class T>
size_t data_logger::Stream<T>::pushSample(
        std::ofstream &fstream, T sample)
{
    _currentSample.setPayload(sample);
    Buffer serialized = _currentSample.serialize();
    fstream.write((char*)&serialized[0], serialized.size()*sizeof(BufferByte));
    _currentSampleIdx++;
    _header.nSamples++;
}
