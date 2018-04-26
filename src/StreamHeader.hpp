#pragma once
#include "Serializable.hpp"

namespace data_logger{

class StreamHeader : Serializable {
public:
    StreamHeader();
    ~StreamHeader();
    size_t nSamples;
    std::string dataModel; //e.g. Text of ASN.1 specification
    std::string dataMetaModel; //e.g. ASN.1
    std::string encodingHint; //e.g. Text of ACN file

    uint64_t serializedSize() override;
    Buffer serialize() override;
    BufferConstIt deserialize(BufferConstIt it) override;

protected:
    size_t szDataModel;
    size_t szDataMetaModel;
    size_t szEncodingHint;

};
}
