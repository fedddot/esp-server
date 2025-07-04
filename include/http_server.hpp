#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <stdexcept>

#include "esp_http_server.h"
#include "http_parser.h"

namespace mcu_server {
    class HttpServer {
    public:
        HttpServer() {
            httpd_config_t config = HTTPD_DEFAULT_CONFIG();
            httpd_handle_t server = nullptr;
            httpd_uri_t post_handler {
                .uri = "post_route",
                .method = HTTP_POST,
                .handler = HttpServer::post_handler,
                .user_ctx = this
            };
            if (httpd_start(&server, &config) != ESP_OK) {
                throw std::runtime_error("failed to create http server");
            }
            if (httpd_register_uri_handler(server, &post_handler) != ESP_OK) {
                httpd_stop(&server);
                throw std::runtime_error("register request handler");
            }
            m_server_handle = server;
        }
        HttpServer(const HttpServer&) = delete;
        HttpServer& operator=(const HttpServer&) = delete;
        virtual ~HttpServer() noexcept = default;
    private:
        httpd_handle_t m_server_handle;
        static esp_err_t post_handler(httpd_req_t *request);
    };
} // namespace mcu_server

#endif // HTTP_SERVER_HPP