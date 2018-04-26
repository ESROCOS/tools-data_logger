set(HEADERS common_defs.hpp
            EncodedSample.hpp
            EncodedSample_impl.hpp
            LogFileReader.hpp
            LogFileReader_impl.hpp
            LogFileWriter.hpp
            LogFileWriter_impl.hpp
            SampleHeader.hpp
            Serializable.hpp
            Stream.hpp
            Stream_impl.hpp
            StreamHeader.hpp
            time.hpp)
set(SOURCES SampleHeader.cpp StreamHeader.cpp time.cpp)

add_library(data_logger ${SOURCES} ${HEADERS})

