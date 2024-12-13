#include <iostream>
#include <string>

#include "cnc_server.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "response.hpp"
#include "test_ipc_connection.hpp"

using namespace manager;
using namespace server;
using namespace cnc_server;
using namespace ipc_uts;

extern "C" {
    void app_main(void) {
        std::cout << "ahaha" << std::endl;
        const std::string test_id("test_cnc_server");
        TestIpcConnection<std::string> connection(
            [](const Response&)-> void {
                throw std::runtime_error("NOT IMPLEMENTED");
            }
        );
        CncServer<std::string> server(
            &connection,
            test_id,
            [](const Data&)-> Gpio * {
                throw std::runtime_error("NOT IMPLEMENTED");
            },
            [](const double) {
                throw std::runtime_error("NOT IMPLEMENTED");
            }
        );
    }
}