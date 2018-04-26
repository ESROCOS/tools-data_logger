#include "SampleHeader.hpp"

data_logger::SampleHeader::SampleHeader()
{

}

data_logger::SampleHeader::~SampleHeader()
{

}

uint64_t data_logger::SampleHeader::serializedSize()
{
    //timestamp + payloadSize
    return(sizeof(uint64_t)+sizeof(uint64_t));
}

Buffer data_logger::SampleHeader::serialize()
{
    _buffer.resize(serializedSize());
    BufferIt it = _buffer.begin();

    //Copy timestamp
    uint8_t* s = (uint8_t*) &writeTimeStamp;
    it = std::copy(s, s+sizeof(writeTimeStamp), it);

    //Copy payload size
    s = (uint8_t*) &payloadSize;
    it = std::copy(s, s+sizeof(payloadSize), it);

    return _buffer;
}

void data_logger::SampleHeader::deserialize(Buffer &buffer)
{
    BufferIt it = buffer.begin();

    //Copy timestamp
    uint8_t* g = (uint8_t*) &writeTimeStamp;
    std::copy(it, it+sizeof(writeTimeStamp), g);
    std::advance(it, sizeof(writeTimeStamp));
    //Copy payload size
    g = (uint8_t*) &payloadSize;
    std::copy(it, it+sizeof(payloadSize), g);
}
