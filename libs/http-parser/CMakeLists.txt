cmake_minimum_required(VERSION 3.8)
project(http_parser_lib)

message("create http_parser_lib")
add_library(http_parser_lib ${CMAKE_CURRENT_SOURCE_DIR}/http_parser.c)
