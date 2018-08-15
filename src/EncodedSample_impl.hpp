#pragma once
#include "EncodedSample.hpp"
#include <chrono>

using namespace std::chrono;


template<class T>
data_logger::EncodedSample<T>::EncodedSample() :
    _payloadBuffer(MAX_SAMPLE_SIZE),
    _encodePayload(nullptr),
    _decodePayload(nullptr)
{

}

template<class T>
data_logger::EncodedSample<T>::~EncodedSample()
{

}

template<class T>
void data_logger::EncodedSample<T>::setPayload(const T &sample)
{
    _payloadSample = sample;
    if(!_encodePayload){
        runtime_error("No function for Payload sample encoding set");
    }
    BufferByte* ptr = (BufferByte*) &_payloadBuffer[0];
    header.payloadSize = _encodePayload(ptr, MAX_SAMPLE_SIZE, &_payloadSample);
}

template<class T>
T data_logger::EncodedSample<T>::getPayload()
{
    int st = _decodePayload(&_payloadSample, &_payloadBuffer[0], header.payloadSize);
    if(st != 0){
        runtime_error("Error decoding sample");
    }
    return _payloadSample;
}

template<class T>
uint64_t data_logger::EncodedSample<T>::serializedSize()
{
    return header.serializedSize() + header.payloadSize;
}


template<class T>
Buffer data_logger::EncodedSample<T>::serialize()
{
    _buffer.resize(serializedSize());
    BufferIt it = _buffer.begin();

    //Copy header
    Buffer b = header.serialize();
    size_t ss = header.serializedSize();
    uint8_t* s = (uint8_t*) &b[0];
    it = std::copy(s, s+header.serializedSize(), it);

    //Copy Payload
    s = (uint8_t*) &_payloadBuffer[0];
    it = std::copy(s, s+header.payloadSize, it);

    return _buffer;
}


template<class T>
size_t data_logger::EncodedSample<T>::deserialize(char* ptr)
{
    //Deserialize header
    size_t header_size = header.deserialize(ptr);
    //Copy payload
    std::copy(ptr+header_size, ptr+header_size+header.payloadSize, &_payloadBuffer[0]);

    return header_size+header.payloadSize;
}
