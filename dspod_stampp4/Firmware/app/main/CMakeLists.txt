idf_component_register(SRCS "main.c" "gfx.c" "st7789_drvr.c"
					"encoder.c" "eb_nau88c22.c" "eb_adc.c" "eb_i2s.c"
					"menu.c" "audio.c" "multicore_audio.c" "dsp_lib.c"
					"debounce.c" "widgets.c" "int_math.c" "pot_widget.c"
					"fx.c"
					"fx_vca.c"
					"fx_filters.c" "ifilter_mg4_v1.c"
					"fx_cdl.c"
					PRIV_REQUIRES spi_flash esp_driver_gpio esp_psram esp_timer
					esp_driver_i2c esp_driver_i2s esp_adc
                    INCLUDE_DIRS "")
