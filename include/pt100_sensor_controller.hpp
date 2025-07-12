#ifndef ESP_PT100_SENSOR_CONTROLLER_HPP
#define ESP_PT100_SENSOR_CONTROLLER_HPP

#include "driver/gpio.h"
#include "hal/gpio_types.h"

#include "temperature_sensor_controller.hpp"

namespace esp {

    class Pt100SensorController: public manager::TemperatureSensorController {
    public:
        Pt100SensorController(const gpio_num_t& gpio_pin): m_gpio_pin(gpio_pin) {
            gpio_config_t io_conf = {};
            io_conf.intr_type = GPIO_INTR_DISABLE;
            io_conf.mode = GPIO_MODE_INPUT;
            io_conf.pin_bit_mask = (1ULL << m_gpio_pin);
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
            ESP_ERROR_CHECK(gpio_config(&io_conf));
        }
        Pt100SensorController(const Pt100SensorController&) = delete;
        Pt100SensorController& operator=(const Pt100SensorController&) = delete;

        double read_temperature() const override {
            // Placeholder for actual temperature reading logic
            return 25.0; // Return a dummy temperature value
        }
    private:
        gpio_num_t m_gpio_pin;
    };
}

#endif // ESP_PT100_SENSOR_CONTROLLER_HPP
