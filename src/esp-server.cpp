#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "esp_adc/adc_oneshot.h"
#include "esp_timer_scheduler.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h" // Added for FreeRTOS timer support

#include "driver/gpio.h"
#include "freertos/task.h"
#include "hal/adc_types.h"
#include "hal/gpio_types.h"

#include "api_request_parser.hpp"
#include "api_request_reader_builder.hpp"
#include "api_response_serializer.hpp"
#include "api_response_writer_builder.hpp"
#include "host.hpp"
#include "http_server.hpp"
#include "ipc_instance.hpp"
#include "manager_instance.hpp"
#include "relay_controller.hpp"
#include "temperature_sensor_controller.hpp"
#include "thermostat_api_request.hpp"
#include "thermostat_api_response.hpp"
#include "thermostat_manager.hpp"
#include "thermostat_vendor.hpp"
#include "timer_scheduler.hpp"
#include "vendor.hpp"
#include "vendor_instance.hpp"
#include "wifi_station_guard.hpp"

#include "gpio_relay_controller.hpp"
#include "pt100_sensor_controller.hpp"

#ifndef NETWORK_SSID
#  error "NETWORK_SSID must be defined"
#endif

#ifndef NETWORK_PASSWORD
#  error "NETWORK_PASSWORD must be defined"
#endif

#ifndef SERVICE_PORT
#  error "SERVICE_PORT must be defined"
#endif

#ifndef SERVICE_ROUTE
#  error "SERVICE_ROUTE must be defined"
#endif

#ifndef RELAY_GPIO_PIN
#  error "RELAY_GPIO_PIN must be defined"
#endif

#ifndef TEMP_SENSOR_GPIO_PIN
#  error "TEMP_SENSOR_GPIO_PIN must be defined"
#endif

using namespace mcu_server;
using namespace host;
using namespace ipc;
using namespace vendor;
using namespace manager;

using RawData = std::vector<char>;
using ApiRequest = ThermostatVendorApiRequest;
using ApiResponse = ThermostatVendorApiResponse;

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

static void blink_loop();
static ThermostatVendor::ThermostatManagerInstance create_thermostat_manager_instance();

extern "C" {
    void app_main(void) {
        auto data_buffer = RawData();
        ApiRequestReaderBuilder<ApiRequest, RawData> reader_builder;
        reader_builder
            .set_api_request_parser(ApiRequestParser())
            .set_raw_data_reader(ipc::Instance<IpcDataReader<RawData>>(new RawDataReader(&data_buffer)));
        const auto api_reader = reader_builder.build();
        ApiResponseWriterBuilder<ApiResponse, RawData> writer_builder;
        writer_builder
            .set_raw_data_writer(ipc::Instance<IpcDataWriter<RawData>>(new RawDataWriter(&data_buffer)))
            .set_api_response_serializer(ApiResponseSerializer());
        const auto api_writer = writer_builder.build();

        const auto thermostat_manager = create_thermostat_manager_instance();
        const auto thermostat_vendor = vendor::Instance<Vendor<ApiRequest, ApiResponse>>(new ThermostatVendor(thermostat_manager));
        Host<ApiRequest, ApiResponse> host(
            api_reader,
            api_writer,
            thermostat_vendor,
            [](const auto& e) -> ApiResponse {
                return ApiResponse(
                    ApiResponse::Result::FAILURE,
                    "unexpected error: " + std::string(e.what())
                );
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
            },
            SERVICE_ROUTE,
            SERVICE_PORT
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

inline ThermostatVendor::ThermostatManagerInstance create_thermostat_manager_instance() {
    const auto relay_controller = manager::Instance<RelayController>(new esp::GpioRelayController(RELAY_GPIO_PIN));
    const auto unit_cfg = adc_oneshot_unit_init_cfg_t {
        .unit_id = ADC_UNIT_1,
        .clk_src = ADC_RTC_CLK_SRC_DEFAULT,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    const auto channel = ADC_CHANNEL_0;
    const auto chan_cfg = adc_oneshot_chan_cfg_t {
        .atten = ADC_ATTEN_DB_0,
        .bitwidth = ADC_BITWIDTH_12,
    };
    const auto temp_sensor_controller = manager::Instance<TemperatureSensorController>(new esp::Pt100SensorController(unit_cfg, channel, chan_cfg));
    const auto timer_scheduler = manager::Instance<TimerScheduler>(new esp::EspTimerScheduler());
    return ThermostatVendor::ThermostatManagerInstance(
        new ThermostatManager(
            relay_controller,
            temp_sensor_controller,
            timer_scheduler
        )
    );
}