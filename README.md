# ESP32 Input Firmware

This project demonstrates a simple event-driven input stack for the ESP32.
It polls hardware inputs and reports button and potentiometer changes over the
serial port.

See [docs/architecture.md](docs/architecture.md) for an overview of the event
flow and polling loop.
This project demonstrates a simple ESP-IDF application that reads buttons and a potentiometer, drives an indicator LED, and reports input events over the serial port. It is intended for small hardware bring-up or for learning basic ESP-IDF GPIO and ADC usage.

## Hardware

Connect the following peripherals to your ESP32 according to `main/Config.h`:

- **BTN1** (ID 1): push button on GPIO42
- **LED1**: indicator LED on GPIO5
- **Four-button block** (IDs 11–14): buttons on GPIO41, GPIO40, GPIO39, GPIO38
- **Potentiometer** (ID 2): ADC1 channel 6 input

## Requirements

- [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/) installed and configured in your environment
- An ESP32 board wired with the peripherals above

## Building and Running

1. Set up the ESP-IDF environment variables (e.g. `source export.sh`).
2. From the repository root, build, flash, and start a serial monitor:
   ```bash
   idf.py build flash monitor
   ```

## Components

- **Button** – polls a GPIO with software debouncing and posts button edge events
- **Led** – minimal wrapper to drive a GPIO output
- **Potentiometer** – reads the ADC and reports values bucketized into eight levels
- **SerialMessenger** – FreeRTOS task that prints events to `stdout`

### Serial Output

Events are emitted on the UART in comma-separated form:

- `BTN,<id>,DOWN` or `BTN,<id>,UP`
- `POT,<id>,<1-8>`

Example session:
```
BTN,1,DOWN
BTN,1,UP
POT,2,5
```

These messages can be viewed with `idf.py monitor` or any serial terminal at 115200 baud.

## Goals

- Illustrate basic GPIO input/output handling with debouncing
- Show ADC sampling and coarse quantization
- Provide a reusable event-driven structure for simple input projects

