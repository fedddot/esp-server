#include "http_server.hpp"

using namespace mcu_server;

extern "C" {
    void app_main(void) {
        HttpServer server;
        while (true) {
            // Keep the server running
            vTaskDelay(pdMS_TO_TICKS(1000)); // Sleep for 1 second
        }
    }
}
