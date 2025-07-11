#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>

#include "esp_err.h"
#include "esp_http_server.h"

#include "wifi_station_guard.hpp"

namespace mcu_server {
    class HttpServer {
    public:
        using RequestCallback = std::function<std::vector<char>(const std::vector<char>& payload)>;
        HttpServer(const WifiStationGuard& sta_guard, const RequestCallback& callback): m_sta_guard(sta_guard), m_callback(callback), m_server_handle(nullptr) {
            if (!m_callback) {
                throw std::invalid_argument("invalid callback function received");
            }
            httpd_config_t config = HTTPD_DEFAULT_CONFIG();
            httpd_handle_t server = nullptr;
            throw std::runtime_error("TODO: make the route configurable");
            httpd_uri_t post_handler {
                .uri = "/post_route",
                .method = HTTP_POST,
                .handler = HttpServer::post_handler,
                .user_ctx = this
            };
            ESP_ERROR_CHECK(httpd_start(&server, &config));
            if (httpd_register_uri_handler(server, &post_handler) != ESP_OK) {
                ESP_ERROR_CHECK(httpd_stop(&server));
                throw std::runtime_error("register request handler");
            }
            m_server_handle = server;
        }
        HttpServer(const HttpServer&) = delete;
        HttpServer& operator=(const HttpServer&) = delete;
        virtual ~HttpServer() noexcept {
            httpd_stop(m_server_handle);
            m_server_handle = nullptr;
        }
    private:
        WifiStationGuard m_sta_guard;
        RequestCallback m_callback;
        httpd_handle_t m_server_handle;

        static esp_err_t post_handler(httpd_req_t *request) {
            try {
                HttpServer* server = static_cast<HttpServer *>(request->user_ctx);
                if (server == nullptr) {
                    return ESP_FAIL;
                }
                const auto& callback = server->m_callback;
                const auto payload = read_payload(request);
                const auto report_body = callback(payload);
                if (const auto res = httpd_resp_set_status(request, HTTPD_200); res != ESP_OK) {
                    return res;
                }
                if (const auto res = httpd_resp_set_type(request, HTTPD_TYPE_OCTET); res != ESP_OK) {
                    return res;
                }
                return httpd_resp_send(request, report_body.data(), report_body.size());
            } catch (...) {
                return ESP_FAIL;
            }
        }

        static std::vector<char> read_payload(httpd_req_t *request) {
            if (!request) {
                throw std::invalid_argument("invalid request pointer received");
            }
            std::unique_ptr<char> payload(new char[request->content_len]);
            if (request->content_len != httpd_req_recv(request, payload.get(), request->content_len)) {
                throw std::runtime_error("failed to read request payload");
            }
            return std::vector<char>(payload.get(), payload.get() + request->content_len);
        }
    };

} // namespace mcu_server

#endif // HTTP_SERVER_HPP