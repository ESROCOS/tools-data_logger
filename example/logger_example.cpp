#include "example_types.h"
#include "LogFileWriter.hpp"
#include <unistd.h>
#include <ctime>
#include <ratio>
#include <chrono>
#include <iostream>

using namespace data_logger;
using namespace std::chrono;

template <class T>
void timed_write(LogFileWriter<T>& writer, MySeq& sample)
{
    high_resolution_clock::time_point t1 =
            std::chrono::high_resolution_clock::now();

    writer.write(sample);

    high_resolution_clock::time_point t2 =
            std::chrono::high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "Writing took " << time_span.count() << " seconds.";
}


size_t Encode_ACN_MySeq(BufferByte *pBuffer, size_t iMaxBufferSize, const MySeq *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);

    if (MySeq_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
    fprintf(stderr, "Could not encode MySeq (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}


int main(int argc, char** argv)
{
    MySeq sample;
    LogFileWriter<MySeq> writer("/tmp/logtest.log");
    writer.setEncoding(Encode_ACN_MySeq);
    //writer.setDataModelFromFile("../../example/example_types.asn");
    writer.setDataModelFromString("1234567899");
    writer.setEncodingHint("ACN");
    writer.start();

    std::cout << "Writing 1" << std::endl;
    timed_write(writer, sample);

    usleep(1000);

    std::cout << "Writing 2" << std::endl;
    timed_write(writer, sample);
    usleep(10000);

    std::cout << "Writing 3" << std::endl;
    timed_write(writer, sample);
    usleep(100000);

    std::cout << "Writing 4" << std::endl;
    timed_write(writer, sample);
    usleep(1000000);

    std::cout << "Writing 5" << std::endl;
    timed_write(writer, sample);

    writer.close();
}
