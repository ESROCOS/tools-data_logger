#include "SampleHeader.hpp"

data_logger::SampleHeader::SampleHeader()
{
    //It's okay to resize here as long as the size of SampleHeader is static
    _buffer.resize(serializedSize());
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
    BufferIt it = _buffer.begin();

    it++;

    //Copy timestamp
    uint8_t* s = (uint8_t*) &writeTimeStamp;
    it = std::copy(s, s+sizeof(writeTimeStamp), it);

    //Copy payload size
    s = (uint8_t*) &payloadSize;
    it = std::copy(s, s+sizeof(payloadSize), it);

    return _buffer;
}

BufferConstIt data_logger::SampleHeader::deserialize(BufferConstIt it)
{
    /*//Copy timestamp
    uint8_t* g = (uint8_t*) &writeTimeStamp;
    std::copy(it, it+sizeof(writeTimeStamp), g);
    std::advance(it, sizeof(writeTimeStamp));
    //Copy payload size
    g = (uint8_t*) &payloadSize;
    std::copy(it, it+sizeof(payloadSize), g);
    std::advance(it, sizeof(payloadSize));*/
    it = deserialize_var<int64_t>(it, writeTimeStamp.microseconds);
    it = deserialize_var<uint64_t>(it, payloadSize);
    return it;
}

void data_logger::SampleHeader::deserialize(std::istream is)
{
    is.readsome((char*)&_buffer[0], serializedSize());
    deserialize(_buffer.begin());
}
