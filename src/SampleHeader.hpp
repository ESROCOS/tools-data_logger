#pragma once
#include "Serializable.hpp"
#include "time.hpp"

namespace data_logger{

class SampleHeader : public Serializable{
public:
    SampleHeader();
    ~SampleHeader();

    Time writeTimeStamp;
    uint64_t payloadSize;

    uint64_t serializedSize() override;
    Buffer serialize() override;
    BufferConstIt deserialize(BufferConstIt it) override;
    void deserialize(std::istream is);

protected:
};
}
