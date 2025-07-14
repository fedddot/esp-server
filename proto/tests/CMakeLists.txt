add_executable(
	thermostat_proto_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_api_request_parser.cpp
)

target_link_libraries(
	thermostat_proto_tests
	PRIVATE
    api_request_parser
	
	pb_encode
	gtest
)