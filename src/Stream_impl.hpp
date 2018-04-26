#pragma once
#include "Stream.hpp"
#include <fstream>

template<class T>
data_logger::Stream<T>::Stream() : currentSampleIdx(-1)
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
    currentSample.header.writeTimeStamp = Time::now();
    currentSample.setPayload(sample);
    Buffer serialized = currentSample.serialize();
    fstream.write((char*)&serialized[0], serialized.size()*sizeof(BufferByte));
    currentSampleIdx++;
    _header.nSamples++;
}

template<class T>
bool data_logger::Stream<T>::readNextSample(
        std::ifstream &fstream)
{
    //Using _currentSample.header.serializedSize() is okay, since SampleHeader is of static size
    if(_header.nSamples > currentSampleIdx+1){
        currentSample.deserialize(fstream);
        currentSampleIdx++;
    }else{
        return false;
    }
}

