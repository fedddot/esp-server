#include <iostream>
#include <string>

#include "cnc_server.hpp"
#include "test_ipc_connection.hpp"

extern "C" {
    void app_main(void) {
        std::cout << "ahaha" << std::endl;
   // GIVEN
    const std::string test_id("test_cnc_server");
    ipc_uts::TestIpcConnection<std::string> connection(
        [](const Response&)-> void {
            throw std::runtime_error("NOT IMPLEMENTED");
        }
    );

    // WHEN
    TestServer *instance_ptr(nullptr);

    // THEN
    ASSERT_NO_THROW(
        instance_ptr = new TestServer(
            &connection,
            test_id,
            [](const Data&)-> Gpio * {
                throw std::runtime_error("NOT IMPLEMENTED");
            },
            [](const double) {
                throw std::runtime_error("NOT IMPLEMENTED");
            }
        )
    );
    ASSERT_NE(nullptr, instance_ptr);
    
    ASSERT_NO_THROW(instance_ptr = nullptr);
    }
}