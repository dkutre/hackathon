cmake_minimum_required(VERSION 3.8)
project(hackathon)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE RELEASE)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

#including nodejs-http-parser
include_directories (${PROJECT_SOURCE_DIR}/libs/http-parser)
add_subdirectory (libs)
set(LIBS ${LIBS} http_parser_lib)

#including Boost.Asio
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost 1.58.0 REQUIRED filesystem system thread)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    set(Boost_USE_MULTITHREADED ON)
    set(LIBS ${LIBS} ${Boost_LIBRARIES})
endif()

include_directories(headers)
set(SOURCE_FILES sources/main.cpp  sources/Server.cpp         sources/Connection.cpp
                 sources/Reply.cpp sources/ConnectionHandler.cpp
                 sources/mimeTypes.cpp sources/HttpParser.cpp sources/Requets.cpp)

add_executable(hackathon ${SOURCE_FILES})
target_link_libraries(hackathon -lpthread ${LIBS})