#pragma once

#include "LogFileWriter.hpp"

template<class T>
data_logger::LogFileWriter<T>::LogFileWriter(std::string filepath) :
    _headerWritten(false),
    _targetPath(filepath)
{
    _fstream.open(_targetPath, std::ios_base::out | std::ios_base::binary);
}

template<class T>
data_logger::LogFileWriter<T>::~LogFileWriter()
{
    this->close();
}

template<class T>
void data_logger::LogFileWriter<T>::LogFileWriter::start()
{
    if(_headerWritten){
        usage_error("start has already been called");
    }
    writeHeader();
}

template<class T>
void data_logger::LogFileWriter<T>::LogFileWriter::writeHeader()
{
    Buffer serialized = _stream.header().serialize();
    _fstream.write((char*)&serialized[0], serialized.size()*sizeof(BufferByte));
    _fstream.flush();
    _headerWritten = true;
}



template<class T>
void data_logger::LogFileWriter<T>::write(T &sample)
{
    if(!_headerWritten){
        usage_error("start has not been called");
    }
    _stream.pushSample(_fstream, sample);
}

template<class T>
void data_logger::LogFileWriter<T>::close()
{
    //Rewrite Header to update number of written samples
    //1. We copy the result to tmp
    //2. Create a file which will contain the update info 'updated'
    //3. Write the new header to 'updated'
    //4. Write the remaining data from 'tmp' to 'updated'
    //5. Remplace old log file

    _fstream.close();

    std::string tmp = std::tmpnam(nullptr);
    std::string updated = std::tmpnam(nullptr);
    //1.
    system((std::string()+"cp "+_targetPath+" "+tmp).c_str());
    //2.
    _fstream.open(updated, std::ios_base::out | std::ios_base::binary);
    //3.
    writeHeader();

    //4.
    std::ifstream ifs(tmp, std::ios_base::in | std::ios_base::binary);
    ifs.seekg(_stream.header().serializedSize(), ifs.beg);
    _fstream << ifs.rdbuf();

    _fstream.close();
    ifs.close();

    //5.
    system((std::string()+"mv "+updated+" "+_targetPath).c_str());
}

template<class T>
void data_logger::LogFileWriter<T>::setDataModelFromString(
        std::string str,
        std::string metaModel)
{
    _stream.header().dataModel = str;
    _stream.header().dataMetaModel = metaModel;
}

template<class T>
void data_logger::LogFileWriter<T>::setDataModelFromFile(
        std::string path,
        std::string metaModel)
{
    std::ifstream t(path);
    if(!t.is_open()){
        usage_error("The file "+path+" is not valid.");
    }
    std::string str((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
    setDataModelFromString(str);
    t.close();
}

template<class T>
void data_logger::LogFileWriter<T>::setEncodingHint(
        std::string str)
{
    _stream.header().encodingHint = str;
}
