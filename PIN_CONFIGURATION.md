# Hardware Connection Information (Table II & Pin Map)

This document describes the hardware components and their corresponding connections to the ESP32 microcontroller.

## 1. ESP32 Pin Configuration

| Component Name | Model / Specification | ESP32 GPIO Pin | Signal Type / Notes |
|---|---|---|---|
| Power Regulation | LM2596 DC-DC Buck Converter | VIN / 5V Rail | Regulated 5.0V DC supply |
| SIM800L GSM Module | SIM800L Cellular Module | TX: GPIO 16, RX: GPIO 17 | UART Serial (Serial1), 9600 Baud |
| Ultrasonic Sensor | HC-SR04 | Trig: GPIO 5, Echo: GPIO 18 | Digital Pulse Input/Output |
| IR Obstacle Sensor | FC-51 / Generic IR | GPIO 13 | Digital Input (INPUT_PULLUP) |
| Flame Sensor | 4-pin IR Flame Sensor | GPIO 14 | Digital Input (INPUT_PULLUP) |
| MQ-2 Gas Sensor | MQ-2 Analog Module | GPIO 34 (ADC1_CH6) | Analog Input (Gas and Smoke Detection) |
| Rain Sensor | Rain Detection Module | GPIO 27 | Digital Input (INPUT_PULLUP) |
| Door Servo Motor | SG90 Servo Motor | GPIO 2 | PWM Output |
| Rain Servo Motor | SG90 Servo Motor | GPIO 4 | PWM Output |
| Buzzer | Active Piezo Buzzer | GPIO 25 | PWM Output |

## 2. SIM800L UART Connections

The SIM800L GSM module communicates with the ESP32 through UART serial communication using Serial1 at 9600 baud.

| SIM800L Pin | ESP32 Pin |
|---|---|
| TX | GPIO 16 (RX) |
| RX | GPIO 17 (TX) |
| GND | Common GND |

## 3. Rain Sensor and Servo Connections

The rain sensor is connected to GPIO 27 and operates as a digital input. When rain is detected, the ESP32 activates the rain servo motor connected to GPIO 4.

| Component | Connection |
|---|---|
| Rain Sensor DO | ESP32 GPIO 27 |
| Rain Sensor VCC | Suitable supply according to module specification |
| Rain Sensor GND | Common GND |
| Rain Servo Signal | ESP32 GPIO 4 |
| Rain Servo VCC | Suitable regulated servo power supply |
| Rain Servo GND | Common GND |

## 4. Important Electrical Notes

1. The SIM800L requires a separate, stable 3.7–4.2 V high-current power supply.
2. The SIM800L and ESP32 must share a common ground.
3. The HC-SR04 Echo output can be 5 V. Use a suitable voltage divider or logic-level shifter before connecting it to the ESP32.
4. Servo motors should receive an appropriate external regulated power supply capable of providing sufficient current.
5. Check the voltage and current requirements of each component before powering the circuit.
6. Ensure that the ESP32, sensors, servo power supply, and GSM module share a common ground where required.
7. Verify all physical connections against the actual hardware before operating the system.

## 5. Additional Notes

- GPIO 34 is an input-only pin and is used for analog readings from the MQ-2 sensor.
- The SIM800L communicates with the ESP32 through UART serial communication.
- The door servo motor is controlled through GPIO 2.
- The rain servo motor is controlled through GPIO 4.
- The rain sensor provides digital input through GPIO 27.
- The buzzer is controlled through GPIO 25 using the ESP32 LEDC PWM functionality.
- The rain detection feature controls the rain servo; the current firmware does not generate a separate Blynk or GSM rain alert.

