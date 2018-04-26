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
    void* ptr = (void*) _payloadBuffer.begin();
    int st = _decodePayload(&_payloadSample, _payloadBuffer, header.payloadSize);
    if(!st){
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
BufferConstIt data_logger::EncodedSample<T>::deserialize(BufferConstIt it)
{
    //Deserialize header
    it = header.deserialize(it);
    //Copy payload
    it = deserialize_container<BufferIt>(it, _payloadBuffer.begin(), header.payloadSize);
    /*uint8_t* g = (uint8_t*) &_payloadBuffer[0];
    std::copy(it, it+_header.payloadSize, g);
    std::advance(it, _header.payloadSize);*/
    return it;
}

template<class T>
void data_logger::EncodedSample<T>::deserialize(std::istream is)
{
    //Deserialize header
    header.deserialize(is);
    //Copy payload
    is.readsome((char*)_payloadBuffer[0], header.payloadSize);
}
