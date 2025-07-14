#ifndef ESP_GPIO_RELAY_CONTROLLER_HPP
#define ESP_GPIO_RELAY_CONTROLLER_HPP

#include "driver/gpio.h"
#include "hal/gpio_types.h"

#include "relay_controller.hpp"

namespace esp {
    class GpioRelayController: public manager::RelayController {
    public:
        GpioRelayController(const gpio_num_t& gpio_pin): m_gpio_pin(gpio_pin) {
            gpio_config_t io_conf = {};
            io_conf.intr_type = GPIO_INTR_DISABLE;
            io_conf.mode = GPIO_MODE_OUTPUT;
            io_conf.pin_bit_mask = (1ULL << m_gpio_pin);
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
            ESP_ERROR_CHECK(gpio_config(&io_conf));
        }
        GpioRelayController(const GpioRelayController&) = delete;
        GpioRelayController& operator=(const GpioRelayController&) = delete;

        void set_relay_state(const bool state) override {
            ESP_ERROR_CHECK(gpio_set_level(m_gpio_pin, state));
        }
    private:
        gpio_num_t m_gpio_pin;
    };
}

#endif // ESP_GPIO_RELAY_CONTROLLER_HPP
