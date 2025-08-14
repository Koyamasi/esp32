# CMakeLists.txt - build description for the main application component
#
# Lists all source files that make up this component and declares the ESP-IDF
# libraries it depends on.  The build system compiles these sources and links
# them into the final firmware image.

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
