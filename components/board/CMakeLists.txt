set(COMPONENT_REQUIRES
    audio_hal audio_sal mbedtls esp_peripherals esp_dispatcher display_service)
set(COMPONENT_PRIV_REQUIRES
    display_service)

if(CONFIG_ESP32_ADF_REVC_BOARD)
    message(STATUS "Current board name is " CONFIG_ESP32_ADF_REVC_BOARD)
    list(APPEND COMPONENT_ADD_INCLUDEDIRS
        ./olimex_esp32_adf_c
        ./es8388
        $ENV{ADF_PATH}/components/audio_hal/driver/es8388
    )
    set(COMPONENT_SRCS
        ./olimex_esp32_adf_c/board.c
        ./olimex_esp32_adf_c/board_pins_config.c
        ./es8388/es8388.c
        $ENV{ADF_PATH}/components/audio_hal/driver/es8388/es8388.c
    )
endif()

register_component()

if (IDF_VERSION_MAJOR GREATER 3)
    idf_component_get_property(audio_board_lib audio_board COMPONENT_LIB)
    set_property(TARGET ${audio_board_lib}
        APPEND PROPERTY INTERFACE_LINK_LIBRARIES ${COMPONENT_LIB})
elseif (IDF_VERSION_MAJOR EQUAL 3)
    set_property(TARGET idf_component_audio_board
        APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
        $<TARGET_PROPERTY:${COMPONENT_TARGET},INTERFACE_INCLUDE_DIRECTORIES>)
endif (IDF_VERSION_MAJOR GREATER 3)
