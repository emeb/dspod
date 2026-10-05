idf_component_register(SRCS "main.c" "eb_nau88c22.c" "eb_i2s.c"
						"audio.c" "multicore_audio.c"
                       PRIV_REQUIRES spi_flash esp_driver_gpio esp_psram esp_timer
					    esp_driver_i2c esp_driver_i2s
					   INCLUDE_DIRS ".")
