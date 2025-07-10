#include "http_server.hpp"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"

using namespace mcu_server;

extern "C" {
    void app_main(void) {
        HttpServer server;
        gpio_config_t io_conf = {};
        io_conf.intr_type = GPIO_INTR_DISABLE;
        io_conf.mode = GPIO_MODE_OUTPUT;
        io_conf.pin_bit_mask = (1ULL << GPIO_NUM_15);
        io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
        io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
        gpio_config(&io_conf);

        bool led_on = true;
        while (true) {
            gpio_set_level(GPIO_NUM_15, led_on);
            led_on = !led_on;
            vTaskDelay(pdMS_TO_TICKS(500)); // 500ms delay
        }
    }
}
