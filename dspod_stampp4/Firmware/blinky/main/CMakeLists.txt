idf_component_register(SRCS "blinky_main.c"
                       PRIV_REQUIRES spi_flash esp_driver_gpio esp_psram
                       INCLUDE_DIRS "")
