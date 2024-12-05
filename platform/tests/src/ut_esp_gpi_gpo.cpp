#include <stdexcept>

#include "esp_gpi.hpp"
#include "esp_gpo.hpp"

using namespace esp_mcu_platform;

template <typename T>
inline void assert_eq(const T& one, const T& other) {
	if (one != other) {
		throw std::runtime_error("equality assertion failed");
	}
}

template <typename T>
inline void assert_ne(const T& one, const T& other) {
	if (one == other) {
		throw std::runtime_error("inequality assertion failed");
	}
}

int main(void) {
	// GIVEN
	const int gpi_id(2);
	const int gpo_id(2);
	
	// WHEN
	EspGpi *gpi_ptr(nullptr);
	EspGpo *gpo_ptr(nullptr);

	// THEN
	try {
		gpi_ptr = new EspGpi(gpi_id);
		assert_ne(static_cast<EspGpi *>(nullptr), gpi_ptr);
		assert_eq(EspGpi::Direction::IN, gpi_ptr->direction());
		assert_eq(EspGpi::State::LOW, gpi_ptr->state());
		delete gpi_ptr;
		gpi_ptr = nullptr;
	} catch (...) {
		return -1;
	}

	return 0;
}
