#ifndef API_REQUEST_PARSER_HPP
#define API_REQUEST_PARSER_HPP

#include <stdexcept>

#include "pb_decode.h"

#include "ipc_data.hpp"
#include "ipc_instance.hpp"
#include "service_api.pb.h"
#include "thermostat_api_request.hpp"

namespace ipc {
    class ApiRequestParser {
    public:
        ApiRequestParser() = default;
        ApiRequestParser(const ApiRequestParser&) = default;
        ApiRequestParser& operator=(const ApiRequestParser&) = default;
        virtual ~ApiRequestParser() noexcept = default;

        Instance<vendor::ThermostatVendorApiRequest> operator()(const RawData& raw_data) const;
    private:
    };

    inline Instance<vendor::ThermostatVendorApiRequest> ApiRequestParser::operator()(const RawData& raw_data) const {
        auto istream = pb_istream_from_buffer(
            (const pb_byte_t *)raw_data.data(),
            raw_data.size()
        );
        service_api_MovementApiRequest decoded_request = service_api_MovementApiRequest_init_default;
	    if (!pb_decode(&istream, service_api_MovementApiRequest_fields, &decoded_request)) {
            throw std::runtime_error("Failed to decode MovementApiRequest from raw data: " + std::string(PB_GET_ERROR(&istream)));
        }
        switch (decoded_request.which_request) {
        case service_api_MovementApiRequest_config_request_tag:
            throw std::runtime_error("Config request parsing not implemented yet");
        case service_api_MovementApiRequest_linear_movement_request_tag:
            throw std::runtime_error("Config request parsing not implemented yet");
        default:
            throw std::runtime_error("unsupported request type in MovementApiRequest");
        }
    }
}

#endif // API_REQUEST_PARSER_HPP