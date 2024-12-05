add_library(esp_gpi INTERFACE)
target_include_directories(esp_gpi INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(esp_gpi INTERFACE gpi esp_stdlib)

add_library(esp_gpo INTERFACE)
target_include_directories(esp_gpo INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(esp_gpo INTERFACE gpo esp_stdlib)

add_subdirectory(tests EXCLUDE_FROM_ALL)