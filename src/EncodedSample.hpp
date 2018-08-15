#pragma once

#include "Serializable.hpp"
#include "SampleHeader.hpp"

namespace data_logger {

template <class T>
class EncodedSample : Serializable {
public:
    EncodedSample();
    ~EncodedSample();

    void setPayload(const T& sample);
    T getPayload();
    void setEncodeFn(size_t (*fn)(BufferByte*, size_t, const T*)){_encodePayload = fn;}
    void setDecodeFn(int (*fn)(T*, BufferByte*, size_t)){_decodePayload = fn;}

    uint64_t serializedSize() override;
    Buffer serialize() override;
    //BufferConstIt deserialize(BufferConstIt it) override;
    size_t deserialize(char* ptr);
    SampleHeader header;

protected:
    Buffer _payloadBuffer;
    T _payloadSample;
    //size_t Encode_UPER_MySeq(void *pBuffer, size_t iMaxBufferSize, const asn1SccMySeq *pSrc);
    size_t (*_encodePayload)(BufferByte*, size_t, const T*);
    //int Decode_NATIVE_MyEnum(asn1SccMyEnum *pDst, void *pBuffer, size_t iBufferSize);
    int (*_decodePayload)(T*, BufferByte*, size_t);
};

}
#include "EncodedSample_impl.hpp"
