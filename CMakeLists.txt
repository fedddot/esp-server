cmake_minimum_required(VERSION 3.16)
include($ENV{IDF_PATH}/tools/cmake/idf.cmake)
project(esp_server)

idf_build_process(esp32s2)
idf_component_register(SRCS ${CMAKE_CURRENT_LIST_DIR}/src/esp_server.cpp INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/include)