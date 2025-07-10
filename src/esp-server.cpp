#include "http_server.hpp"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"
#include "wifi_station_guard.hpp"

using namespace mcu_server;

extern "C" {
    void app_main(void) {
        WifiStationGuard sta_guard("Idan", "25800852");
        HttpServer server(sta_guard);
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
}
