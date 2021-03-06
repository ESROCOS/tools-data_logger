#include "StreamHeader.hpp"

data_logger::StreamHeader::StreamHeader() :
    nSamples(0),
    dataModel("no data model specified"),
    dataMetaModel("not data meta model specified"),
    encodingHint("no encoding hint given")
{
    dataModel = "bla";
}

data_logger::StreamHeader::~StreamHeader()
{

}

uint64_t data_logger::StreamHeader::serializedSize()
{
    szDataMetaModel = dataMetaModel.size()*sizeof(char);
    szDataModel = dataModel.size()*sizeof(char);
    szEncodingHint = encodingHint.size()*sizeof(char);
    szEndianness = endianness.size()*sizeof(char);
    //Memory:
    //[ szHeader | nSamples | szDataModel | DataModel | szDataMetaModel |
    //  DataMetaModel | szEncodingHint | encodingHint ]
    return(sizeof(size_t) + sizeof(size_t) + sizeof(size_t) + szDataModel +
           sizeof(size_t) + szDataMetaModel + sizeof(size_t) + szEncodingHint +
           sizeof(size_t) + szEndianness);
}

Buffer data_logger::StreamHeader::serialize()
{
    size_t szHeader = serializedSize();
    _buffer.resize(szHeader);
    BufferIt it = _buffer.begin();

    //Memory:
    //[ szHeader | nSamples | szDataModel | DataModel | szDataMetaModel |
    //  DataMetaModel | szEncodingHint | encodingHint ]

    //Copy szHeader
    uint8_t* s = (uint8_t*) &szHeader;
    it = std::copy(s, s+sizeof(szHeader), it);

    //Copy nSamples
    s = (uint8_t*) &nSamples;
    it = std::copy(s, s+sizeof(nSamples), it);

    //Copy szDataModel
    s = (uint8_t*) &szDataModel;
    it = std::copy(s, s+sizeof(szDataModel), it);

    //Copy DataModel
    s = (uint8_t*) &dataModel[0];
    it = std::copy(s, s+szDataModel, it);

    //Copy szDataMetaModel
    s = (uint8_t*) &szDataMetaModel;
    it = std::copy(s, s+sizeof(szDataMetaModel), it);

    //Copy DataMetaModel
    s = (uint8_t*) &dataMetaModel[0];
    it = std::copy(s, s+szDataMetaModel, it);

    //Copy szEncodingHint
    s = (uint8_t*) &szEncodingHint;
    it = std::copy(s, s+sizeof(szEncodingHint), it);

    //Copy encodingHint
    s = (uint8_t*) &encodingHint[0];
    it = std::copy(s, s+szEncodingHint, it);

    //Copy szEndianess
    s = (uint8_t*) &szEndianness;
    it = std::copy(s, s+sizeof(szEndianness), it);

    //Copy Endianness
    s = (uint8_t*) &endianness[0];
    it = std::copy(s, s+szEndianness, it);

    return _buffer;
}

BufferConstIt data_logger::StreamHeader::deserialize(BufferConstIt it)
{
    //Memory:
    //[ szHeader | nSamples | szDataModel | DataModel | szDataMetaModel |
    //  DataMetaModel | szEncodingHint | encodingHint ]

    size_t szHeader;
    //Copy szHeader
    it = deserialize_var<size_t>(it, szHeader);

    //Copy nSamples
    it = deserialize_var<size_t>(it, nSamples);

    //Copy szDataModel
    it = deserialize_var<size_t>(it, szDataModel);
    //Copy DataModel
    dataModel.resize(szDataModel);
    it = deserialize_container<std::string::iterator>(it, dataModel.begin(), szDataModel);

    //Copy szDataMetaModel
    it = deserialize_var<size_t>(it, szDataMetaModel);
    //Copy DataMetaModel
    dataMetaModel.resize(szDataMetaModel);
    it = deserialize_container<std::string::iterator>(it, dataMetaModel.begin(), szDataMetaModel);

    //Copy szEncodingHint
    it = deserialize_var<size_t>(it, szEncodingHint);
    //Copy encodingHint
    encodingHint.resize(szEncodingHint);
    it = deserialize_container<std::string::iterator>(it, encodingHint.begin(), szEncodingHint);

    //Copy szEndianness
    it = deserialize_var<size_t>(it, szEndianness);
    //Copy endianness
    endianness.resize(szEndianness);
    it = deserialize_container<std::string::iterator>(it, endianness.begin(), szEndianness);

    return it;
}

size_t data_logger::StreamHeader::deserialize(char* ptr)
{
    size_t szHeader;
    char* hp = (char*) &szHeader;
    std::copy(ptr, ptr+sizeof(size_t), hp);

    _buffer.resize(szHeader);
    std::copy(ptr, ptr+szHeader, &_buffer[0]);
    deserialize(_buffer.begin());

    return szHeader;
}

std::ostream& operator<< (std::ostream& stream, const data_logger::StreamHeader& rhs)
{
    stream << "Data Model:        " << rhs.dataMetaModel << "\n";
    stream << "Encoding Hint:     " << rhs.encodingHint << "\n";
    stream << "Endianess:         " << rhs.endianness << "\n";
    stream << "Number of samples: " << rhs.nSamples<<"\n";
    return stream;
}
