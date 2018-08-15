#pragma once

#include "LogFileWriter.hpp"
#include "host_order.hpp"
#include <iostream>
#include <unistd.h>
#include <chrono>

template<class T>
data_logger::LogFileWriter<T>::LogFileWriter(std::string filepath) :
    _headerWritten(false), _running(false), _targetPath(filepath)
{
    _stream.header().endianness = O32_ENDIANESS_TO_STRING(O32_HOST_ORDER);
    std::cout << "System is " << _stream.header().endianness << "\n";

    _fstream.open(_targetPath, std::ios_base::out | std::ios_base::binary);
    if (errno != 0)
        throw std::system_error(errno, std::system_category());
    if (!_fstream)
        runtime_error("IO_FAIL");
}

template<class T>
data_logger::LogFileWriter<T>::~LogFileWriter()
{
    if(_running){
        this->close();
    }
}


template<class T>
void data_logger::LogFileWriter<T>::LogFileWriter::start()
{
    if(_headerWritten){
        usage_error("start has already been called");
    }
    writeHeader();
    _running = true;
    _write_thread = std::thread(&LogFileWriter::writeThread, this);
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
void data_logger::LogFileWriter<T>::write(const T &sample)
{
    auto begin = std::chrono::high_resolution_clock::now();
    if(!_headerWritten){
        usage_error("start has not been called");
    }
    _queue_mutex.lock();
    _write_queue.push(sample);
    if((_write_queue.size() % 5) == 0){
        std::cout << "Write buffer size for" << _targetPath << ": " << _write_queue.size() << std::endl;
    }
    _queue_mutex.unlock();
    auto end = std::chrono::high_resolution_clock::now();
    double dur = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
    if(dur > 1){
        std::cout << dur << "ms" << std::endl;
    }
}

template<class T>
void data_logger::LogFileWriter<T>::writeThread()
{
    while(_running){
        _queue_mutex.lock(); //POS 1
       if(!_write_queue.empty()){
           while (!_write_queue.empty()) {
               //Local copy of sample not not let writing to file block the exceution
               T sample = _write_queue.front();
               _queue_mutex.unlock(); //Unlocks POS 1 or POS 2

               //This is the slow operation and should be performed unlocked
               _stream.pushSample(_fstream, sample);

               _queue_mutex.lock();   //POS 2
               _write_queue.pop();
               //Mutex kept locked for !_write_queue.empty() in while
           }
       }
       _queue_mutex.unlock(); //Unlocks POS 1 or POS 2

        usleep(50);
    }
    std::cout << "Write thread for " << _targetPath << " closed." << std::endl;
}

template<class T>
void data_logger::LogFileWriter<T>::close()
{
    std::cout << "Clearing write buffer for " << _targetPath << std::endl;
    _running = false;
    std::cout << "join" << _targetPath << std::endl;
    try{
        _write_thread.join();
    }catch(std::system_error ex){
        std::cerr << "Thread error: " << ex.what() <<std::endl;
    }

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
    std::cout << "Log file " << _targetPath << " closed" << std::endl;
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
    setDataModelFromString(str, metaModel);
    t.close();
}

template<class T>
void data_logger::LogFileWriter<T>::setEncodingHint(
        std::string str)
{
    _stream.header().encodingHint = str;
}
