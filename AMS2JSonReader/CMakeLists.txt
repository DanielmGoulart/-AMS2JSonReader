# CMakeList.txt : CMake project for AMS2JSonReader, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (AMS2JSonReader "AMS2JSonReader.cpp" "AMS2JSonReader.h")
file(COPY ${CMAKE_CURRENT_LIST_DIR}/jr_stats_data.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# TODO: Add tests and install targets if needed.
add_definitions(-DUNICODE -D_UNICODE)