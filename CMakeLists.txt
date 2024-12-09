cmake_minimum_required(VERSION 3.16)
project(esp_server C CXX)

include($ENV{IDF_PATH}/tools/cmake/idf.cmake)

# Create the project executable and plainly link the newlib component to it using
# its alias, idf::newlib.
# add_executable(${CMAKE_PROJECT_NAME}.elf ${CMAKE_CURRENT_LIST_DIR}/src/main.c)
# target_link_libraries(${CMAKE_PROJECT_NAME}.elf idf::newlib)

# Let the build system know what the project executable is to attach more targets, dependencies, etc.
# idf_build_executable(${CMAKE_PROJECT_NAME}.elf)

# idf_component_register(SRCS ${CMAKE_CURRENT_LIST_DIR}/src/esp_server.c INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/include)