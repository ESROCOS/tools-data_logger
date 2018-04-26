#pragma once
#include <vector>
#include "time.hpp"
#include <string>
#include <iostream>

typedef uint8_t BufferByte;
typedef std::vector<BufferByte> Buffer;
typedef std::vector<BufferByte>::iterator BufferIt;
typedef std::vector<BufferByte>::const_iterator BufferConstIt;

inline void usage_error(std::string s){
    std::cerr << "[Data Logger] ERROR: " << s <<std::endl;
    throw(std::runtime_error(s));
}
inline void runtime_error(std::string s){
    std::cerr << "[Data Logger] RUNTIME ERROR: " << s <<std::endl;
    throw(std::runtime_error(s));
}

template<typename TargetType>
BufferConstIt deserialize_var(BufferConstIt it, TargetType& target){
    uint8_t* g = (uint8_t*) &target;
    std::copy(it, it+sizeof(target), g);
    std::advance(it, sizeof(target));
    return it;
}

template<typename TargetIterator>
BufferConstIt deserialize_container(BufferConstIt it, TargetIterator target, size_t sz){
    std::copy(it, it+sz, target);
    std::advance(it, sz);
    return it;
}

#define MAX_SAMPLE_SIZE 3000000
