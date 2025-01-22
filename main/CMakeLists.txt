idf_component_register(SRCS "main.c"
                        INCLUDE_DIRS
                            "."
                        REQUIRES
                            ota_ws_update
                            esp_http_server
                            mdns
                            esp_wifi
                            esp32-wifi-manager
                        EMBED_FILES 
                            example_echo_ws_server.html
)