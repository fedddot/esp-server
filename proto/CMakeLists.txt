add_library(pb_encode INTERFACE)
target_sources(pb_encode INTERFACE $ENV{NANOPB_SRC_PATH}/pb_encode.c)
target_include_directories(pb_encode INTERFACE $ENV{NANOPB_SRC_PATH}/include)

add_library(pb_decode INTERFACE)
target_sources(pb_decode INTERFACE $ENV{NANOPB_SRC_PATH}/pb_decode.c)
target_include_directories(pb_decode INTERFACE $ENV{NANOPB_SRC_PATH}/include)

add_library(pb_common INTERFACE)
target_sources(pb_common INTERFACE $ENV{NANOPB_SRC_PATH}/pb_common.c)
target_include_directories(pb_common INTERFACE $ENV{NANOPB_SRC_PATH}/include)

set(SERVICE_API_PROTO_PATH ${CMAKE_CURRENT_LIST_DIR}/resourses)
set(SERVICE_API_PROTO_NAME service_api.proto)
set(GENERATED_PROTO_PATH ${CMAKE_BINARY_DIR}/proto_generated)
add_custom_command(
	OUTPUT ${GENERATED_PROTO_PATH}/service_api.pb.h ${GENERATED_PROTO_PATH}/service_api.pb.c
	COMMAND mkdir -p ${GENERATED_PROTO_PATH} && $ENV{NANOPB_SRC_PATH}/generator/protoc -I ${SERVICE_API_PROTO_PATH} --nanopb_out=${GENERATED_PROTO_PATH} ${SERVICE_API_PROTO_NAME}
	DEPENDS ${SERVICE_API_PROTO_PATH}/${SERVICE_API_PROTO_NAME}
)

add_custom_target(
	generate_service_api
	DEPENDS ${GENERATED_PROTO_PATH}/service_api.pb.h ${GENERATED_PROTO_PATH}/service_api.pb.c
)

add_library(service_api STATIC)
target_sources(service_api PRIVATE ${GENERATED_PROTO_PATH}/service_api.pb.c)
target_include_directories(service_api PUBLIC ${GENERATED_PROTO_PATH} $ENV{NANOPB_SRC_PATH})
add_dependencies(service_api generate_service_api)

add_library(api_request_parser INTERFACE)
target_include_directories(api_request_parser INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
	api_request_parser
	INTERFACE
	ipc_data
	ipc_instance
	pb_common
	pb_decode
	service_api
	thermostat_api_request
)

add_library(api_response_serializer INTERFACE)
target_include_directories(api_response_serializer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
	api_response_serializer
	INTERFACE
	ipc_data
	pb_common
	pb_encode
	service_api
	thermostat_api_response
)

add_subdirectory(tests EXCLUDE_FROM_ALL)