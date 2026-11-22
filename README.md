# IoT-Based Smart Home Security and Automation System Using ESP32 and Blynk

## Project Overview

This project is a smart home security and automation prototype built on an ESP32 microcontroller, the Blynk IoT platform and a SIM800L GSM module.

It monitors fire, gas and smoke, and intrusion. When an event starts, it sounds a local buzzer and sends one Blynk notification and one SMS to an administrator number. The prototype also has an ultrasonic-sensor door and a rain-controlled clothesline, which work locally and send no alerts.

This is an academic prototype. It is not a certified safety or security device.

## Features

- Intrusion detection with an FC-51 IR sensor.
- Fire detection with an HW-072 flame sensor.
- Gas and smoke detection with an MQ-2 sensor. The firmware gives one combined alert and does not separate gas from smoke.
- Local alarm with an active piezo buzzer.
- Blynk push notifications over Wi-Fi.
- SMS alerts through the SIM800L GSM module.
- Door servo that opens when the HC-SR04 reads less than 20 cm. This is a convenience feature and not access control.
- Clothesline servo that moves when the rain sensor reports rain.

## Hardware Components

- ESP32 DevKit (ESP32-WROOM-32)
- HC-SR04 ultrasonic sensor
- FC-51 IR sensor
- HW-072 flame sensor
- MQ-2 gas and smoke sensor
- Rain sensor module
- Two SG90 servo motors (door and clothesline)
- Active piezo buzzer
- SIM800L GSM module
- LM2596 buck converter
- Three 18650 Li-ion cells, breadboards and jumper wires

## Software and Libraries

- Arduino IDE (or a compatible environment) with the ESP32 board package
- Blynk library
- ESP32Servo library
- TinyGSM library

## Blynk Event Configuration

Create the following three events in your Blynk template. The event codes must match the firmware.

| Event name | Event code |
|---|---|
| Intruder Alert | intruder_alert |
| Fire Alert | fire_alert |
| Gas Alert | gas_alert |

The Blynk dashboard only receives notifications. It sends no commands to the board.

## How It Works

- The firmware repeats one loop with a 100 ms delay. It reads the ultrasonic, rain, IR, flame and MQ-2 inputs one after another.
- A gas event is declared when the MQ-2 ADC value is above 1500 and below 4095. This is an uncalibrated ADC level and not a gas concentration.
- On a new event the firmware sends one Blynk notification and one SMS, and then the buzzer sounds while the condition lasts. A flag for each hazard prevents repeated messages during the same event. There is no cooldown.
- Sending an SMS blocks the loop for the duration of the transmission.

## Hardware Connections

The pin assignment is in [PIN_CONFIGURATION.md](PIN_CONFIGURATION.md). The circuit diagram is in the `circuit` folder.

## Power Supply Considerations

- Two 18650 cells in series (7.4 V) feed the LM2596 buck converter, set to 5 V.
- The SIM800L needs its own 3.7-4.2 V supply that can deliver current peaks of about 2 A. Do not power it from the ESP32.
- Connect the grounds of the ESP32, the sensors and the SIM800L together.
- The cells are charged outside the system. The prototype has no charging circuit.
- Use a voltage divider or level shifter for the HC-SR04 Echo signal and for the MQ-2 analog output in a final design.

## Source Code

The ESP32 firmware is in the `code` folder.

Before using it, enter your own Wi-Fi credentials, Blynk authentication token and administrator phone number. Keep them in a separate file that is not committed (for example `secrets.h` listed in `.gitignore`). Never publish personal credentials in a public repository.

## Known Limitations

- The door and clothesline sensors in the prototype later failed, so these two functions are not evaluated in the paper.
- Blynk uses the non-SSL library, so traffic is not encrypted.
- The SIM800L works on 2G networks only.
- The MQ-2 threshold is not calibrated, and the flame sensor gave two false alerts in a 10-minute idle test.
- Power consumption and battery life were not measured.

## Disclaimer

This repository is provided for academic and research purposes. Actual performance depends on sensor calibration, power supply stability, network availability and environmental conditions.