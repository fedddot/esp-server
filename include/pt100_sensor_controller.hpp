#ifndef ESP_PT100_SENSOR_CONTROLLER_HPP
#define ESP_PT100_SENSOR_CONTROLLER_HPP

#include <stdexcept>

#include "esp_adc/adc_oneshot.h"
#include "hal/adc_types.h"

#include "temperature_sensor_controller.hpp"

namespace esp {

    class Pt100SensorController: public manager::TemperatureSensorController {
    public:
        Pt100SensorController(
            const adc_oneshot_unit_init_cfg_t& unit_cfg,
            const adc_channel_t& channel,
            const adc_oneshot_chan_cfg_t& chan_cfg
        ) {
            adc_oneshot_unit_handle_t adc_handle;
            if (ESP_OK != adc_oneshot_new_unit(&unit_cfg, &adc_handle)) {
                throw std::runtime_error("Failed to create ADC oneshot unit");
            }
            m_handle = adc_handle;
            m_chan = channel;
        }
        Pt100SensorController(const Pt100SensorController&) = delete;
        Pt100SensorController& operator=(const Pt100SensorController&) = delete;
        ~Pt100SensorController() noexcept override {
            if (m_handle) {
                adc_oneshot_del_unit(m_handle);
                m_handle = nullptr;
            }
        }

        double read_temperature() const override {
            int read_result = 0;
            if (ESP_OK != adc_oneshot_read(m_handle, m_chan, &read_result)) {
                throw std::runtime_error("Failed to read ADC value");
            }
            return static_cast<double>(read_result);
        }
    private:
        adc_oneshot_unit_handle_t m_handle;
        adc_channel_t m_chan;
    };
}

#endif // ESP_PT100_SENSOR_CONTROLLER_HPP
