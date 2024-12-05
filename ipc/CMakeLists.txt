add_library(esp_ipc_connection INTERFACE)
target_include_directories(esp_ipc_connection INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    esp_ipc_connection
    INTERFACE
    buffered_ipc_connection
    ipc_connection
    request
    response
)

add_subdirectory(tests EXCLUDE_FROM_ALL)