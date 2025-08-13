idf_component_register(
    SRCS
        "app_main.cpp"
        "Led.cpp"
        "Button.cpp"
        "Potentiometer.cpp"
        "SerialMessenger.cpp"
        "EventQueue.cpp"
        "FourButtons.cpp"
    INCLUDE_DIRS "."
    REQUIRES esp_driver_gpio esp_adc
)
