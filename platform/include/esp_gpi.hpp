#ifndef	ESP_GPI_HPP
#define	ESP_GPI_HPP

#include <stdexcept>

#include "hardware/gpio.h"

#include "gpio.hpp"
#include "gpi.hpp"

namespace esp_mcu_platform {

	class EspGpi: public manager::Gpi {
	public:
		EspGpi(int id);
		EspGpi(const EspGpi& other) = default;
		EspGpi& operator=(const EspGpi& other) = delete;
		State state() const override;
		manager::Gpio *clone() const override;
	private:
		int m_id;
	};

	inline EspGpi::EspGpi(int id): m_id(id) {
		if ((0 == m_id) || (1 == m_id)) {
			throw std::invalid_argument("GPIOs id = 0, 1 are reserved for UART0");
		}
		gpio_init(m_id);
        gpio_set_dir(m_id, GPIO_IN);
		gpio_pull_up(m_id);
	}

	inline EspGpi::State EspGpi::state() const {
		if (gpio_get(m_id)) {
			return State::HIGH;
		}
		return State::LOW;
	}

	inline manager::Gpio *EspGpi::clone() const {
		return new EspGpi(*this);
	}
}

#endif // ESP_GPI_HPP