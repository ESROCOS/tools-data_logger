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

    //Copy timestamp
    uint8_t* s = (uint8_t*) &(writeTimeStamp.microseconds);
    it = std::copy(s, s+sizeof(int64_t), it);

    //Copy payload size
    s = (uint8_t*) &payloadSize;
    it = std::copy(s, s+sizeof(payloadSize), it);

    return _buffer;
}

BufferConstIt data_logger::SampleHeader::deserialize(BufferConstIt it)
{
    it = deserialize_var<int64_t>(it, writeTimeStamp.microseconds);
    it = deserialize_var<uint64_t>(it, payloadSize);
    return it;
}

size_t data_logger::SampleHeader::deserialize(char* ptr)
{
    std::copy(ptr, ptr+serializedSize(), &_buffer[0]);
    deserialize(_buffer.begin());

    return serializedSize();
}
