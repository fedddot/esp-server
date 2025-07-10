#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <stdexcept>

#include "esp_http_server.h"
#include "http_parser.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"

namespace mcu_server {
    class HttpServer {
    public:
        HttpServer() {
            // Initialize NVS
            esp_err_t ret = nvs_flash_init();
            if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
                ESP_ERROR_CHECK(nvs_flash_erase());
                ESP_ERROR_CHECK(nvs_flash_init());
            }

            // Initialize TCP/IP and event loop
            ESP_ERROR_CHECK(esp_netif_init());
            ESP_ERROR_CHECK(esp_event_loop_create_default());

            // Create default WiFi AP
            esp_netif_create_default_wifi_ap();

            wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
            ESP_ERROR_CHECK(esp_wifi_init(&cfg));

            wifi_config_t ap_config = {};
            strcpy((char*)ap_config.ap.ssid, "ESP32-AP");
            ap_config.ap.ssid_len = strlen("ESP32-AP");
            strcpy((char*)ap_config.ap.password, "12345678");
            ap_config.ap.max_connection = 4;
            ap_config.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;
            if (strlen("12345678") == 0) {
                ap_config.ap.authmode = WIFI_AUTH_OPEN;
            }

            ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
            ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));
            ESP_ERROR_CHECK(esp_wifi_start());

            // Start HTTP server
            httpd_config_t config = HTTPD_DEFAULT_CONFIG();
            httpd_handle_t server = nullptr;
            httpd_uri_t post_handler {
                .uri = "/post_route",
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
        virtual ~HttpServer() noexcept {
            httpd_stop(m_server_handle);
            m_server_handle = nullptr;
        }
    private:
        httpd_handle_t m_server_handle;
        static esp_err_t post_handler(httpd_req_t *request) {
            HttpServer* server = static_cast<HttpServer *>(request->user_ctx);
            if (server == nullptr) {
                return ESP_FAIL;
            }
            char buffer[256];
            int ret = httpd_req_recv(request, buffer, sizeof(buffer));
            if (ret <= 0) {
                return ESP_FAIL;
            }
            buffer[ret] = '\0';
            httpd_resp_send(request, "Data received", HTTPD_RESP_USE_STRLEN);
            return ESP_OK;
        }
    };
} // namespace mcu_server

#endif // HTTP_SERVER_HPP