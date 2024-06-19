# CMakeList.txt : CMake project for AMS2JSonReader, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/curl_x64-windows/include)
if(CMAKE_BUILD_TYPE EQUAL Debug)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/curl_x64-windows/debug/lib)
    file(COPY ${CMAKE_CURRENT_LIST_DIR}/curl_x64-windows/debug/libcurl-d.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
else()
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/curl_x64-windows/lib)
    file(COPY ${CMAKE_CURRENT_LIST_DIR}/curl_x64-windows/bin/libcurl.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

# Add source to this project's executable.
add_executable (AMS2JSonReader "AMS2JSonReader.cpp" "AMS2JSonReader.h")
file(COPY ${CMAKE_CURRENT_LIST_DIR}/jr_stats_data.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_LIST_DIR}/json_reader_config.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_LIST_DIR}/curl_x64-windows/tools/curl/zlib1.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# TODO: Add tests and install targets if needed.
add_definitions(-DUNICODE -D_UNICODE)

target_include_directories(AMS2JSonReader PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/curl_x64-windows/include")
# find_package(CURL REQUIRED)
if(CMAKE_BUILD_TYPE EQUAL Debug)
    target_link_libraries(AMS2JSonReader libcurl-d)
else()
    target_link_libraries(AMS2JSonReader libcurl)
endif()
