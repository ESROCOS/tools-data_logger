#pragma once
#include "common_defs.hpp"

namespace data_logger{

class Serializable{
public:
    Serializable(){}
    ~Serializable(){}

    virtual uint64_t serializedSize() = 0;
    virtual Buffer serialize() = 0;

protected:
    Buffer _buffer;
};
}
