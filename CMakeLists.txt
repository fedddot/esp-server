cmake_minimum_required(VERSION 3.16)
project(esp_server)

include($ENV{IDF_PATH}/tools/cmake/idf.cmake)
idf_build_process(esp32s2)
idf_component_register(SRCS ${CMAKE_CURRENT_LIST_DIR}/src/esp_server.c INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/include)