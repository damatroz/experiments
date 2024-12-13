cmake_minimum_required(VERSION 3.24)
project(Experiments CXX)

set (CMAKE_CXX_STANDARD 20)

find_package(RapidJSON REQUIRED)

add_library(lib
    lib/Json.cpp
    lib/Json.h
    lib/Preprocessor.h
    lib/ReflectiveStruct.h)
target_include_directories(lib INTERFACE lib)
target_link_libraries(lib rapidjson)

add_executable(Reflective src/Reflective.cpp)
target_link_libraries(Reflective lib)

add_executable(Subscriber src/Subscriber.cpp)
target_link_libraries(Subscriber lib)

add_executable(Configuration src/Configuration.cpp)
target_link_libraries(Configuration lib)

add_executable(Decorators src/Decorators.cpp)

add_executable(VariantFromTemplate src/VariantFromTemplate.cpp)
