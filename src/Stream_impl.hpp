#pragma once
#include "Stream.hpp"
#include <fstream>

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
    currentSample.setPayload(sample);
    Buffer serialized = currentSample.serialize();
    fstream.write((char*)&serialized[0], serialized.size()*sizeof(BufferByte));
    _currentSampleIdx++;
    _header.nSamples++;
}

template<class T>
size_t data_logger::Stream<T>::readNextSample(
        std::ifstream &fstream)
{
    //Using _currentSample.header.serializedSize() is okay, since SampleHeader is of static size
    currentSample.deserialize(fstream);
}

