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
    //Memory:
    //[ nSamples | szDataModel | DataModel | szDataMetaModel | DataMetaModel |
    //szEncodingHint | encodingHint ]
    return(sizeof(size_t) + sizeof(size_t)+szDataModel + sizeof(size_t)+szDataMetaModel +
           sizeof(size_t) + szEncodingHint);
}

Buffer data_logger::StreamHeader::serialize()
{
    _buffer.resize(serializedSize());
    BufferIt it = _buffer.begin();

    //Memory:
    //[ nSamples | szDataModel | DataModel | szDataMetaModel | DataMetaModel |
    //szEncodingHint | encodingHint ]

    //Copy nSamples
    uint8_t* s = (uint8_t*) &nSamples;
    std::cout << sizeof(nSamples) << ", ";
    it = std::copy(s, s+sizeof(nSamples), it);

    //Copy szDataModel
    s = (uint8_t*) &szDataModel;
    it = std::copy(s, s+sizeof(szDataModel), it);
    std::cout << sizeof(szDataModel) << ", ";

    //Copy DataModel
    s = (uint8_t*) &dataModel[0];
    it = std::copy(s, s+szDataModel, it);
    std::cout << szDataModel << ", ";

    //Copy szDataMetaModel
    s = (uint8_t*) &szDataMetaModel;
    it = std::copy(s, s+sizeof(szDataMetaModel), it);
    std::cout << sizeof(szDataMetaModel) << ", ";

    //Copy DataMetaModel
    s = (uint8_t*) &dataMetaModel[0];
    it = std::copy(s, s+szDataMetaModel, it);
    std::cout << szDataMetaModel << ",";

    //Copy szEncodingHint
    s = (uint8_t*) &szEncodingHint;
    it = std::copy(s, s+sizeof(szEncodingHint), it);
    std::cout << sizeof(szEncodingHint) << ", ";

    //Copy encodingHint
    s = (uint8_t*) &encodingHint[0];
    it = std::copy(s, s+szEncodingHint, it);
    std::cout << szEncodingHint << std::endl;

    return _buffer;
}

BufferConstIt data_logger::StreamHeader::deserialize(BufferConstIt it)
{
    //Memory:
    //[ nSamples | szDataModel | DataModel | szDataMetaModel | DataMetaModel |
    //szEncodingHint | encodingHint ]

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

    return it;
}
