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
            host_order.hpp
            time.hpp)
set(SOURCES SampleHeader.cpp StreamHeader.cpp time.cpp)

add_library(data_logger ${SOURCES} ${HEADERS})

add_executable(data_logger-analyze data_logger-analyze.cpp)
target_link_libraries(data_logger-analyze data_logger)

install(TARGETS data_logger data_logger-analyze
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
install(FILES ${HEADERS} 
        DESTINATION include/data_logger)

CONFIGURE_FILE("data_logger.pc.in" "data_logger.pc" @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/data_logger.pc
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig
)

