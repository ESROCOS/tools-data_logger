set(ASN1_FILES ${CMAKE_CURRENT_SOURCE_DIR}/example_types.asn)
set(GENERATED_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/example_types.c
    ${CMAKE_CURRENT_SOURCE_DIR}/example_types.h
    ${CMAKE_CURRENT_SOURCE_DIR}/acn.c
    ${CMAKE_CURRENT_SOURCE_DIR}/asn1crt.h
    ${CMAKE_CURRENT_SOURCE_DIR}/asn1crt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/real.c)

add_custom_command(
	OUTPUT ${GENERATED_FILES}
        COMMAND asn1.exe -c -ACN ${ASN1_FILES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_library(example_types ${GENERATED_FILES})

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(logger_example logger_example.cpp)
target_link_libraries(logger_example example_types data_logger)
