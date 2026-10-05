idf_component_register(SRCS "main.c" "eb_adc.c"
                       PRIV_REQUIRES spi_flash esp_driver_gpio esp_psram esp_timer
					    esp_driver_i2c esp_driver_i2s esp_adc
                       INCLUDE_DIRS ".")
