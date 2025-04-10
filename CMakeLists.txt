cmake_minimum_required(VERSION 3.20)
project(micro-json-encoder C CXX)

add_library(micro-json-encoder STATIC encoder.c)
target_include_directories(micro-json-encoder SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})