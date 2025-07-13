#ifndef API_REQUEST_PARSER_HPP
#define API_REQUEST_PARSER_HPP

#include <optional>
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
    };

    inline Instance<vendor::ThermostatVendorApiRequest> ApiRequestParser::operator()(const RawData& raw_data) const {
        auto istream = pb_istream_from_buffer(
            (const pb_byte_t *)raw_data.data(),
            raw_data.size()
        );
        service_api_ThermostatApiRequest decoded_request = service_api_ThermostatApiRequest_init_default;
	    if (!pb_decode(&istream, service_api_ThermostatApiRequest_fields, &decoded_request)) {
            throw std::runtime_error("Failed to decode ThermostatApiRequest from raw data: " + std::string(PB_GET_ERROR(&istream)));
        }
        switch (decoded_request.request_type) {
        case service_api_RequestType_STOP:
            return Instance<vendor::ThermostatVendorApiRequest>(
                new vendor::ThermostatVendorApiRequest(vendor::ThermostatVendorApiRequest::RequestType::STOP)
            );
        case service_api_RequestType_START:
            return Instance<vendor::ThermostatVendorApiRequest>(
                new vendor::ThermostatVendorApiRequest(
                    vendor::ThermostatVendorApiRequest::RequestType::START,
                    decoded_request.set_temperature,
                    decoded_request.time_resolution_ms
                )
            );
        case service_api_RequestType_GET_TEMP:
            return Instance<vendor::ThermostatVendorApiRequest>(
                new vendor::ThermostatVendorApiRequest(vendor::ThermostatVendorApiRequest::RequestType::GET_TEMP)
            );
        default:
            throw std::runtime_error("unsupported request type in ThermostatApiRequest");
        }
    }
}

#endif // API_REQUEST_PARSER_HPP