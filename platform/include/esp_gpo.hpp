#ifndef	ESP_GPO_HPP
#define	ESP_GPO_HPP

#include <stdexcept>

#include "hardware/gpio.h"

#include "gpio.hpp"
#include "gpo.hpp"

namespace esp_mcu_platform {

	class EspGpo: public manager::Gpo {
	public:
		EspGpo(int id);
		EspGpo(const EspGpo& other) = default;
		EspGpo& operator=(const EspGpo& other) = delete;
		State state() const override;
		void set_state(const State& state) override;
		manager::Gpio *clone() const override;
	private:
		uint m_id;
	};

	inline EspGpo::EspGpo(int id): m_id(static_cast<uint>(id)) {
		if ((0 == m_id) || (1 == m_id)) {
			throw std::invalid_argument("GPIOs id = 0, 1 are reserved for UART0");
		}
		gpio_init(m_id);
        gpio_set_dir(m_id, GPIO_OUT);
	}

	inline EspGpo::State EspGpo::state() const {
		return gpio_get(m_id) ? State::HIGH : State::LOW;
	}

	inline void EspGpo::set_state(const State& state) {
		if (state != this->state()) {
			gpio_put(m_id, (State::HIGH == state) ? 1 : 0);
		}
	}

	inline manager::Gpio *EspGpo::clone() const {
		return new EspGpo(*this);
	}
}

#endif // ESP_GPO_HPP