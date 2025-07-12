cmake_minimum_required(VERSION 3.20)

set(TARGET esp32s2)
set(CMAKE_TOOLCHAIN_FILE $ENV{IDF_PATH}/tools/cmake/toolchain-${TARGET}.cmake)

project(esp_mcu_server C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include($ENV{IDF_PATH}/tools/cmake/idf.cmake)

idf_build_set_property(MINIMAL_BUILD ON)
idf_build_process(
    ${TARGET}
    COMPONENTS freertos esp-tls nvs_flash esp_netif esp_http_server esp_wifi esp_eth
    SDKCONFIG ${CMAKE_CURRENT_LIST_DIR}/sdkconfig
    BUILD_DIR ${CMAKE_BINARY_DIR}
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT DEFINED NETWORK_SSID)
    message(FATAL_ERROR "NETWORK_SSID is not defined. Please pass it to the cmake command")
endif()

if(NOT DEFINED NETWORK_PASSWORD)
    message(FATAL_ERROR "NETWORK_PASSWORD is not defined. Please pass it to the cmake command")
endif()

if(NOT DEFINED SERVICE_PORT)
    set(SERVICE_PORT 5555)
endif()

if(NOT DEFINED SERVICE_ROUTE)
    set(SERVICE_ROUTE "/test_route")
endif()

add_executable(${CMAKE_PROJECT_NAME}.elf ${CMAKE_CURRENT_LIST_DIR}/src/esp-server.cpp)
target_link_libraries(
    ${CMAKE_PROJECT_NAME}.elf
    PRIVATE
    idf::esp_driver_gpio

    api_request_parser
    api_request_reader_builder
    api_response_serializer
    api_response_writer_builder
    host
    http_server
    ipc_instance
    manager_instance
    relay_controller
    temperature_sensor_controller
    thermostat_api_request
    thermostat_api_response
    thermostat_manager
    thermostat_vendor
    timer_scheduler
    vendor
    vendor_instance
    wifi_station_guard
    esp_timer_scheduler
)
idf_build_executable(${CMAKE_PROJECT_NAME}.elf)
target_compile_definitions(
    ${CMAKE_PROJECT_NAME}.elf
    PRIVATE
    NETWORK_SSID=\"${NETWORK_SSID}\"
    NETWORK_PASSWORD=\"${NETWORK_PASSWORD}\"
    SERVICE_PORT=${SERVICE_PORT}
    SERVICE_ROUTE=\"${SERVICE_ROUTE}\"
    RELAY_GPIO_PIN=GPIO_NUM_5
    TEMP_SENSOR_GPIO_PIN=GPIO_NUM_4
)

add_library(wifi_access_point_guard INTERFACE)
target_include_directories(wifi_access_point_guard INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    wifi_access_point_guard
    INTERFACE
    idf::nvs_flash
    idf::esp_netif
    idf::esp_wifi
)

add_library(wifi_station_guard INTERFACE)
target_include_directories(wifi_station_guard INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    wifi_station_guard
    INTERFACE
    idf::nvs_flash
    idf::esp_netif
    idf::esp_wifi
)

add_library(http_server INTERFACE)
target_include_directories(http_server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    http_server
    INTERFACE
    wifi_access_point_guard
    wifi_station_guard
    idf::esp_http_server
)

add_library(esp_timer_scheduler INTERFACE)
target_include_directories(esp_timer_scheduler INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    esp_timer_scheduler
    INTERFACE
    idf::freertos
    idf::esp_timer
    timer_scheduler
)

add_subdirectory(proto)
add_subdirectory($ENV{MCU_SERVER_PATH} ${CMAKE_BINARY_DIR}/_deps/mcu_server-build)