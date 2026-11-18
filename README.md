# IoT-Based Smart Home Security and Automation System Using ESP32 and Blynk

## Project Overview

This project presents an IoT-based smart home security and automation system using an ESP32 microcontroller, Blynk IoT platform, and SIM800L GSM module.

The system monitors different security and environmental conditions, including unauthorized intrusion, fire, gas leakage, and smoke. It also uses an ultrasonic sensor for automatic door control.

When an alert condition is detected, the system can send notifications through the Blynk IoT platform and SMS messages through the SIM800L GSM module, depending on network availability.

## Features

- Automatic door control using an ultrasonic sensor and servo motor.
- Intrusion detection using an IR obstacle sensor.
- Fire detection using a flame sensor.
- Gas leakage detection using an MQ-2 sensor.
- Smoke detection using an MQ-2 sensor.
- Blynk IoT notifications.
- GSM-based SMS alerts using the SIM800L module.
- Audible warning using a buzzer.

## Hardware Components

- ESP32 Development Board
- HC-SR04 Ultrasonic Sensor
- FC-51 IR Obstacle Sensor
- Flame Sensor
- MQ-2 Gas and Smoke Sensor
- SG90 Servo Motor
- SIM800L GSM Module
- Buzzer
- LM2596 DC-DC Buck Converter
- Battery and connecting wires

## Software and Libraries

- Visual Studio Code (for source-code management)
- Arduino IDE or another compatible Arduino development environment (for compiling and uploading)
- ESP32 Board Package
- Blynk Library
- ESP32Servo Library
- TinyGSM Library

## Blynk Event Configuration

Create the following five events in your Blynk Template. The event codes must match the code used in the ESP32 program.

| Event Name | Event Code |

| Door Alert | door_alert |
| Intruder Alert | intruder_alert|
| Fire Alert | fire_alert |
| Gas Alert | gas_alert |
| Smoke Alert | smoke_alert |

## Repository Structure

esp32-smart-home-security-automation/
│
├── code/
│   └── smart_home_security.ino
│
├── circuit/
│   └── circuit_diagram.png
│
├── README.md
│
└── PIN_CONFIGURATION.md

## Hardware Connections

The hardware components are connected to the ESP32 according to the pin configuration provided in [PIN_CONFIGURATION.md](PIN_CONFIGURATION.md).

The circuit diagram is available in the `circuit` folder.

## Power Supply Considerations

- Provide a regulated supply suitable for the ESP32 board.
- The SIM800L requires a separate stable 3.7–4.2 V high-current power supply.
- Connect the grounds of the ESP32 and SIM800L together.
- Do not power the SIM800L directly from the ESP32 3.3 V pin.
- Use an appropriate voltage divider or logic-level shifter for the HC-SR04 Echo signal before connecting it to an ESP32 GPIO.

## Source Code

The ESP32 source code is available in the `code` folder.

Before using the code, configure your own Wi-Fi credentials, Blynk authentication token, and administrator phone number. Do not publish personal credentials or private information in a public repository.

## Disclaimer

This repository is provided for academic and research purposes. Actual hardware performance may depend on sensor calibration, power supply stability, network availability, and environmental conditions.