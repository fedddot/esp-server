#include "gtest/gtest.h"
#include "pb.h"
#include "pb_encode.h"

#include "ipc_data.hpp"
#include "api_request_parser.hpp"
#include "service_api.pb.h"
#include "thermostat_api_request.hpp"

using namespace ipc;
using namespace vendor;

static RawData serialize_thermostat_request(const ThermostatVendorApiRequest& request);

TEST(ut_api_request_parser, sanity) {
	// GIVEN
	const auto request = ThermostatVendorApiRequest(
		ThermostatVendorApiRequest::RequestType::START,
		25.0,
		1000
	);
	const auto raw_data = serialize_thermostat_request(request);
	
	// WHEN
	const auto instance = ApiRequestParser();

	// THEN
	ASSERT_NO_THROW({
		const auto result = instance(raw_data);
		ASSERT_EQ(result.get().type(), request.type());
		ASSERT_EQ(result.get().temperature(), request.temperature());
		ASSERT_EQ(result.get().time_resolution_ms(), request.time_resolution_ms());
	});
}

inline RawData serialize_thermostat_request(const ThermostatVendorApiRequest& request) {
	const auto request_type_mapping = std::map<ThermostatVendorApiRequest::RequestType, service_api_RequestType> {
		{ ThermostatVendorApiRequest::RequestType::START, service_api_RequestType_START },
		{ ThermostatVendorApiRequest::RequestType::STOP, service_api_RequestType_STOP },
		{ ThermostatVendorApiRequest::RequestType::GET_TEMP, service_api_RequestType_GET_TEMP }
	};
	auto casted_temp = float(0.0);
	if (request.temperature()) {
		casted_temp = static_cast<float>(request.temperature().value());
	}
	auto casted_time_resolution = std::size_t(0);
	if (request.time_resolution_ms()) {
		casted_time_resolution = request.time_resolution_ms().value();
	}
	const auto pb_request = service_api_ThermostatApiRequest {
		.request_type = request_type_mapping.at(request.type()),
		.set_temperature = casted_temp,
		.time_resolution_ms = casted_time_resolution,
	};

	pb_byte_t buffer[256];
	auto ostream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	if (!pb_encode(&ostream, service_api_ThermostatApiRequest_fields, &pb_request)) {
		throw std::runtime_error("Failed to encode ThermostatVendorApiRequest to raw data");
	}
	return RawData((const char *)buffer, (const char *)(buffer + ostream.bytes_written));
}