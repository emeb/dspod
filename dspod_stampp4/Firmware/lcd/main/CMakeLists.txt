idf_component_register(SRCS "main.c" "gfx.c" "st7789_drvr.c" "int_math.c"
                       PRIV_REQUIRES spi_flash esp_driver_gpio esp_psram esp_timer
						esp_driver_i2c esp_driver_i2s
                    INCLUDE_DIRS "")
