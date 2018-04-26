#include "example_types.h"
#include "LogFileWriter.hpp"
#include "LogFileReader.hpp"
#include <unistd.h>
#include <ctime>
#include <ratio>
#include <chrono>
#include <iostream>

#ifndef ORDER32_H
#define ORDER32_H

#include <limits.h>
#include <stdint.h>

#if CHAR_BIT != 8
#error "unsupported char size"
#endif

enum
{
    O32_LITTLE_ENDIAN = 0x03020100ul,
    O32_BIG_ENDIAN = 0x00010203ul,
    O32_PDP_ENDIAN = 0x01000302ul,      /* DEC PDP-11 (aka ENDIAN_LITTLE_WORD) */
    O32_HONEYWELL_ENDIAN = 0x02030001ul /* Honeywell 316 (aka ENDIAN_BIG_WORD) */
};

static const union { unsigned char bytes[4]; uint32_t value; } o32_host_order =
    { { 0, 1, 2, 3 } };

#define O32_HOST_ORDER (o32_host_order.value)

#endif

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
    std::cout << "Writing took " << time_span.count() << " seconds." <<std::endl;
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
    if(O32_HOST_ORDER == O32_LITTLE_ENDIAN){
        std::cout << "System is little endian" << std::endl;
    }else if(O32_HOST_ORDER == O32_BIG_ENDIAN){
        std::cout << "System is big endian" << std::endl;
    }else if(O32_HOST_ORDER == O32_PDP_ENDIAN){
        std::cout << "System is PDP endian" << std::endl;
    }else if(O32_HOST_ORDER == O32_HONEYWELL_ENDIAN){
        std::cout << "System is Honeywell endian" << std::endl;
    }
    else{
        std::cout << "dunno" << std::endl;
    }

    MySeq sample;
    sample.input_data = 1;
    sample.output_data = 11;
    sample.validity = valid;

    LogFileWriter<MySeq> writer("/tmp/logtest.log");
    writer.setEncoding(Encode_ACN_MySeq);
    //writer.setDataModelFromFile("../../example/example_types.asn");
    writer.setDataModelFromString("1234567899");
    writer.setEncodingHint("ACN");
    writer.start();

    std::cout << "Writing 1" << std::endl;
    timed_write(writer, sample);


    sample.input_data = 2;
    sample.output_data = 22;
    sample.validity = valid;

    std::cout << "Writing 2" << std::endl;
    timed_write(writer, sample);

    sample.input_data = 3;
    sample.output_data = 33;
    sample.validity = valid;

    std::cout << "Writing 3" << std::endl;
    timed_write(writer, sample);

    sample.input_data = 4;
    sample.output_data = 44;
    sample.validity = valid;

    std::cout << "Writing 4" << std::endl;
    timed_write(writer, sample);

    sample.input_data = 5;
    sample.output_data = 55;
    sample.validity = valid;

    std::cout << "Writing 5" << std::endl;
    timed_write(writer, sample);

    writer.close();





     LogFileReader<MySeq> reader("/tmp/logtest.log");
     reader.setDecoding(MySeq_ACN_Decode());
}
