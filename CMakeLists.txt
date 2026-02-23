cmake_minimum_required(VERSION 3.10)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()


project(NMEA_Parser_Project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LibKML CONFIG REQUIRED)
include_directories(include)

add_executable(NMEAParser 
    src/NMEAParser.cpp
    src/main.cpp
    src/HelperFunctions.cpp
    src/NMEAExporter.cpp
    src/NMEAValidator.cpp
)

target_link_libraries(NMEAParser PRIVATE
kmlbase
kmlengine
kmldom
)