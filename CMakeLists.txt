cmake_minimum_required(VERSION 3.10)
project(http_downloader)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(load main.cpp src/FileSaver.cpp
                             src/UrlParser.cpp
                             src/Tcp.cpp
                             src/utils.cpp
                             src/HttpDownloader.cpp)