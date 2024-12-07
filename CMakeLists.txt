cmake_minimum_required(VERSION 3.16)
project(esp_server C CXX)

include($ENV{IDF_PATH}/tools/cmake/idf.cmake)

idf_build_process(esp32s2)

add_executable(esp_server_app ${CMAKE_CURRENT_LIST_DIR}/src/esp_server.cpp)
target_link_libraries(esp_server_app PRIVATE idf::newlib)

idf_build_executable(esp_server_app)