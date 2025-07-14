#ifndef API_RESPONSE_SERIALIZER_HPP
#define API_RESPONSE_SERIALIZER_HPP

#include <map>
#include <stdexcept>

#include "pb.h"
#include "pb_encode.h"

#include "ipc_data.hpp"
#include "thermostat_api_response.hpp"
#include "service_api.pb.h"

namespace ipc {
    class ApiResponseSerializer {
    public:
        ApiResponseSerializer() = default;
        ApiResponseSerializer(const ApiResponseSerializer&) = default;
        ApiResponseSerializer& operator=(const ApiResponseSerializer&) = default;
        virtual ~ApiResponseSerializer() noexcept = default;

        RawData operator()(const vendor::ThermostatVendorApiResponse& response) const;
    private:
        static bool encode_string(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);
    };

    inline RawData ApiResponseSerializer::operator()(const vendor::ThermostatVendorApiResponse& response) const {
        using namespace vendor;
        using VendorStatus = vendor::ThermostatVendorApiResponse::Result;
        const auto status_mapping = std::map<VendorStatus, service_api_StatusCode> {
            { VendorStatus::SUCCESS, service_api_StatusCode_SUCCESS },
            { VendorStatus::FAILURE, service_api_StatusCode_FAILURE },
        };
        const auto pb_status = status_mapping.at(response.result());
        const auto pb_msg = response.message() ? response.message().value() : std::string("");
        const auto pb_temperature = response.temperature() ? static_cast<float>(response.temperature().value()) : 0.0f;
        const auto pb_response = service_api_ThermostatApiResponse {
            .status = pb_status,
            .message = pb_callback_t {
                .funcs = {
                    .encode = &encode_string,
                },
                .arg = const_cast<std::string *>(&pb_msg),
            },
            .current_temperature = pb_temperature
        };

        enum: int { BUFF_SIZE = 512UL };
        pb_byte_t buffer[BUFF_SIZE];
        pb_ostream_t ostream = pb_ostream_from_buffer(
            buffer,
            BUFF_SIZE
        );
        if (!pb_encode(&ostream, service_api_ThermostatApiResponse_fields, &pb_response)) {
            throw std::runtime_error("failed to encode ThermostatVendorApiResponse into protocol buffer: " + std::string(PB_GET_ERROR(&ostream)));
        }
        return RawData((const char *)buffer, (const char *)buffer + ostream.bytes_written);
    }

    inline bool ApiResponseSerializer::encode_string(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {
        if (!arg || !*arg) {
            throw std::runtime_error("encode_string called with null arg");
        }
        const auto str = *static_cast<const std::string *>(*arg);
        if (!pb_encode_tag_for_field(stream, field)) {
            return false;
        }
        return pb_encode_string(stream, (const pb_byte_t *)(str.c_str()), str.size());
    }
}

#endif // API_RESPONSE_SERIALIZER_HPP