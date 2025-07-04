#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <string>
#include <stdexcept>

namespace mcu_server {
    class HttpServer {
    public:
        HttpServer(const std::string& addr) {
            throw std::runtime_error("HttpServer is not implemented yet");
        }
        HttpServer(const HttpServer&) = delete;
        HttpServer& operator=(const HttpServer&) = delete;
        virtual ~HttpServer() noexcept = default;
    private:
        
    };
} // namespace mcu_server

#endif // HTTP_SERVER_HPP