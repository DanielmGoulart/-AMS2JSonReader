# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("AMS2JSonReader")

# Include sub-projects.
add_subdirectory ("AMS2JSonReader")

add_definitions(-DUNICODE -D_UNICODE)