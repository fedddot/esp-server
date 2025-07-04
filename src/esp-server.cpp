#include "http_server.hpp"

using namespace mcu_server;

extern "C" {
    void app_main(void) {
        HttpServer server;
    }
}
