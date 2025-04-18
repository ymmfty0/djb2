cmake_minimum_required(VERSION 3.30)
project(djb2)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -static -fpermissive")

set(CMAKE_CXX_STANDARD 20)

add_executable(djb2 main.cpp)
