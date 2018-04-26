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

#define MAX_SAMPLE_SIZE 3000000
