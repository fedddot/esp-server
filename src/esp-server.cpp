#include "http_server.hpp"

using namespace mcu_server;

extern "C" {
    void app_main(void) {
        HttpServer server("127.0.0.1:5555");
    }
}
