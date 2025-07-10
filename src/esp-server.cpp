#include "freertos/FreeRTOS.h"

#include "driver/gpio.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"

#include "wifi_station_guard.hpp"
#include "http_server.hpp"

#ifndef NETWORK_SSID
#  error "NETWORK_SSID must be defined"
#endif

#ifndef NETWORK_PASSWORD
#  error "NETWORK_PASSWORD must be defined"
#endif

using namespace mcu_server;

static void blink_loop();

extern "C" {
    void app_main(void) {
        WifiStationGuard sta_guard(NETWORK_SSID, NETWORK_PASSWORD);
        HttpServer server(sta_guard);
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