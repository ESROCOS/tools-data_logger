#pragma once
#include "Serializable.hpp"
#include <iostream>

namespace data_logger{

class StreamHeader : Serializable {
public:
    StreamHeader();
    ~StreamHeader();
    size_t nSamples;
    std::string dataModel; //e.g. Text of ASN.1 specification
    std::string dataMetaModel; //e.g. ASN.1
    std::string encodingHint; //e.g. Text of ACN file
    std::string endianness; //e.g. O32_LITTLE_ENDIAN

    uint64_t serializedSize() override;
    Buffer serialize() override;
    BufferConstIt deserialize(BufferConstIt it);
    size_t deserialize(char* ptr);


protected:
    size_t szDataModel;
    size_t szDataMetaModel;
    size_t szEncodingHint;
    size_t szEndianness;

};
}

std::ostream& operator<< (std::ostream& stream, const data_logger::StreamHeader& rhs);
