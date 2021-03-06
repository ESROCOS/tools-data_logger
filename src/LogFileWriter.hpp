#pragma once
#include "common_defs.hpp"
#include <fstream>
#include "Stream.hpp"
#include <thread>
#include <queue>
#include <mutex>

#define QUEUE_SIZE 100

namespace data_logger {
template <class T>
class LogFileWriter{
public:
    LogFileWriter(std::string filepath);
    ~LogFileWriter();

    void write(const T& sample);
    void close();
    void setEncoding(size_t (*fn)(BufferByte*, size_t, const T*))
    {
        _stream.setEncoding(fn);
    }

    void setDataModelFromString(std::string str, std::string metaModel = "ASN.1");
    void setDataModelFromFile(std::string path, std::string metaModel = "ASN.1");
    void setEncodingHint(std::string str);

    void start();
    void writeHeader();

protected:
    std::ofstream _fstream;
    Stream<T> _stream;
    bool _headerWritten;
    std::string _targetPath;
    std::mutex _queue_mutex;
    bool _running;
    std::queue<T> _write_queue;
    std::thread _write_thread;
    void writeThread();
};
}


#include "LogFileWriter_impl.hpp"
