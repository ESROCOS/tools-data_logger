#pragma once
#include "EncodedSample.hpp"


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

    _header.payloadSize = _encodePayload(ptr, MAX_SAMPLE_SIZE, &_payloadSample);
}

template<class T>
T data_logger::EncodedSample<T>::getPayload()
{
    void* ptr = (void*) _payloadBuffer.begin();
    int st = _decodePayload(&_payloadSample, _payloadBuffer, _header.payloadSize);
    if(!st){
        runtime_error("Error decoding sample");
    }
    return _payloadSample;
}

template<class T>
uint64_t data_logger::EncodedSample<T>::serializedSize()
{
    return _header.serializedSize() + _header.payloadSize;
}


template<class T>
Buffer data_logger::EncodedSample<T>::serialize()
{
    _buffer.resize(serializedSize());
    BufferIt it = _buffer.begin();

    //Copy header
    uint8_t* s = (uint8_t*) &_header.serialize()[0];
    it = std::copy(s, s+sizeof(_header.serializedSize()), it);

    //Copy Payload
    s = (uint8_t*) &_payloadBuffer[0];
    it = std::copy(s, s+sizeof(_header.payloadSize), it);

    return _buffer;
}

template<class T>
void data_logger::EncodedSample<T>::deserialize(Buffer &buffer)
{
    runtime_error("not implemented");
}
