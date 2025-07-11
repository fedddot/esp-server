#include <stdexcept>
#include <string>
#include <vector>

#include "freertos/FreeRTOS.h"

#include "driver/gpio.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"

#include "api_request_reader_builder.hpp"
#include "api_response_writer_builder.hpp"
#include "host.hpp"
#include "http_server.hpp"
#include "ipc_instance.hpp"
#include "vendor.hpp"
#include "vendor_instance.hpp"
#include "wifi_station_guard.hpp"

#ifndef NETWORK_SSID
#  error "NETWORK_SSID must be defined"
#endif

#ifndef NETWORK_PASSWORD
#  error "NETWORK_PASSWORD must be defined"
#endif

using namespace mcu_server;
using namespace host;
using namespace ipc;
using namespace vendor;

using AxesConfig = std::string;
using RawData = std::vector<char>;
using ApiRequest = std::string;
using ApiResponse = std::string;

class RawDataReader : public IpcDataReader<RawData> {
public:
    RawDataReader(RawData *raw_data): m_raw_data(raw_data) {
        if (m_raw_data == nullptr) {
            throw std::invalid_argument("invalid raw_data pointer received");
        }
    }
    std::optional<ipc::Instance<RawData>> read() override {
        if (m_raw_data->empty()) {
            return std::nullopt;
        }
        const auto read_result = ipc::Instance<RawData>(new RawData(*m_raw_data));
        m_raw_data->clear();
        return read_result;
    }
private:
    RawData *m_raw_data;
};

class RawDataWriter : public IpcDataWriter<RawData> {
public:
    RawDataWriter(RawData *raw_data): m_raw_data(raw_data) {
        if (m_raw_data == nullptr) {
            throw std::invalid_argument("invalid raw_data pointer received");
        }
    }
    void write(const RawData& data) const override {
        m_raw_data->assign(data.begin(), data.end());
    }
private:
    RawData *m_raw_data;
};

class SimpleVendor: public Vendor<ApiRequest, ApiResponse> {
public:
    SimpleVendor() = default;
    SimpleVendor(const SimpleVendor&) = delete;
    SimpleVendor& operator=(const SimpleVendor&) = delete;
    ApiResponse run_api_request(const ApiRequest& request) override {
        const auto msg = std::string("received request: ") + request;
        return ApiResponse(msg.begin(), msg.end());
    }
};

static void blink_loop();

extern "C" {
    void app_main(void) {
        auto data_buffer = RawData();
        ApiRequestReaderBuilder<ApiRequest, RawData> reader_builder;
        reader_builder
            .set_api_request_parser(
                [](const RawData& raw_data) -> ipc::Instance<ApiRequest> {
                    return ipc::Instance<ApiRequest>(new ApiRequest(raw_data.begin(), raw_data.end()));
                }
            )
            .set_raw_data_reader(ipc::Instance<IpcDataReader<RawData>>(new RawDataReader(&data_buffer)));
        const auto api_reader = reader_builder.build();
        ApiResponseWriterBuilder<ApiResponse, RawData> writer_builder;
        writer_builder
            .set_raw_data_writer(ipc::Instance<IpcDataWriter<RawData>>(new RawDataWriter(&data_buffer)))
            .set_api_response_serializer(
                [](const ApiResponse& response) {
                    return RawData(response.begin(), response.end());
                }
            );
        const auto api_writer = writer_builder.build();
        Host<ApiRequest, ApiResponse> host(
            api_reader,
            api_writer,
            vendor::Instance<Vendor<ApiRequest, ApiResponse>>(new SimpleVendor()),
            [](const auto& e) {
                return ApiResponse(std::string("error in host: ") + std::string(e.what()));
            }
        );

        WifiStationGuard sta_guard(NETWORK_SSID, NETWORK_PASSWORD);
        auto data_buff_ptr = &data_buffer;
        auto host_ptr = &host;
        HttpServer server(
            sta_guard,
            [data_buff_ptr, host_ptr](const auto& payload) {
                data_buff_ptr->assign(payload.begin(), payload.end());
                host_ptr->run_once();
                return std::vector<char>(data_buff_ptr->begin(), data_buff_ptr->end());
            }
        );
        blink_loop();
    }
}

inline void blink_loop() {
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << GPIO_NUM_15);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    int led_on_time = 100;
    int led_off_time = 1900;
    while (true) {
        gpio_set_level(GPIO_NUM_15, true);
        vTaskDelay(pdMS_TO_TICKS(led_on_time));
        gpio_set_level(GPIO_NUM_15, false);
        vTaskDelay(pdMS_TO_TICKS(led_off_time));
    }
}