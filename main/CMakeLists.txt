cmake_minimum_required(VERSION 3.5)

file(GLOB SRCS_C "*.c")
file(GLOB SRCS_CPP "*.cpp")

file(GLOB SRCS_HTTP_HANDLERS_C "http_handlers/*.c")
file(GLOB SRCS_HTTP_HANDLERS_CPP "http_handlers/*.cpp")

idf_component_register(SRCS 
                            ${SRCS_C}
                            ${SRCS_CPP}
                            ${SRCS_HTTP_HANDLERS_C}
                            ${SRCS_HTTP_HANDLERS_CPP}
                        INCLUDE_DIRS
                            "."
                            "http_handlers"
                        REQUIRES
                            ota_ws_update
                            esp_http_server
                            mdns
                            json
                            esp_wifi
                            esp32-wifi-manager
                            esp_app_format
                            app_update
                        EMBED_FILES
                            ${EMBED_MINIFIED_HTML}
)